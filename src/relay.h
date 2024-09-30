boolean DBG_relay = false;

void relayState(uint8_t r_num, boolean state, String mess){

    if (!XOR(ChOnOff[r_num], state)) return;
    if ((r_num == zakat_rel) && (zakat_sw)) {
      float interrupt_period = float(zakat_del) * zakat_step / (zakat_max - zakat_min);
      Serial.print("Период прерывания: ");
      Serial.println(interrupt_period, 3);
      if (state) {
        zakat_cur = zakat_min;
        blinker.attach(interrupt_period, daybreakIsr);
        Serial.println(strTimeNow() + " Старт рассвета");
      } else {
        zakat_cur = zakat_max;
        blinker.attach(interrupt_period, sundownIsr);
        Serial.println(strTimeNow() + " Старт заката");
      }
    } else {
      digitalWrite(relays[r_num], XOR(state, RelayUp));
    }
    ChOnOff[r_num] = state;
    String umes = strTimeNow() + " Relay " + String(r_num+1) + " " + mess;
    if (state) umes += " is switch on";
    else  umes += " is switch off";
    Serial.println(umes);

}

void relay(){
  static unsigned long i;
  
  if(i + 1000 > millis()) return;
  i = millis();
  // tem = ds_tem;
  
  if (tem > p_tem + ( h_tem / 2)){
    if (mode == "TEM") {
      relayState(0, false, "by temperature");
      ChOnOff[0] = false;
    }
    tempOK = true;
  }
  if (tem < p_tem - ( h_tem / 2)){
    if (mode == "TEM") {
      relayState(0, true, "by temperature");
      ChOnOff[0] = true;
    }
    tempOK = false;
  }
  
  if (mode == "MAN") {
    relayState(0, valSwitch, "change manual");
    ChOnOff[0] = valSwitch;
  }
  if (mode == "TMR") {
    
  }
}

void timer_handle(unsigned int deltime) {
  static unsigned long del_timer;
  unsigned int time_now;
  unsigned int time_on;
  unsigned int time_off;
  boolean relay_state[relay_num];
  uint8_t bitDay = 1;
  Datime dt(NTP);
    int wd = dt.weekDay - 1;
  
  if (!wd) wd = 7;
  if (wd > 1) bitDay <<= (wd-1);

  for (uint8_t i=0; i < relay_num; i++) { // инициализируем статусы
    relay_state[i] = false;
  }

  if (del_timer + deltime < millis()) { // Если интервал уже истек
    time_now = dt.hour * 60 + dt.minute;
    for (uint8_t i=0; i < timers_num; i++) {
      uint8_t my_timer_week = weekday_set[my_timer[i].Timer_days];
      if (DBG_relay) {
        String strDays = "Для таймера " + String(i+1) + " биты дней: " + String(my_timer_week, HEX) + ", а реле: " + String(my_timer[i].Timer_relay + 1); 
        Serial.println(strDays);
      }
      if (my_timer_week & bitDay) {
        if (DBG_relay) {
          Serial.print("Проходной бит дня таймера ");
          Serial.print(String(i+1) + " равен 0x");
          Serial.println((my_timer[i].Timer_week & bitDay), HEX);
        }
        time_on = my_timer[i].Timer_start.hour * 60 + my_timer[i].Timer_start.minute;
        time_off = my_timer[i].Timer_stop.hour * 60 + my_timer[i].Timer_stop.minute;
        if (time_on == time_off) break;
        if (time_off > time_on) {// таймер внутри суток
          if ((time_now >= time_on) && (time_now < time_off)) 
            relay_state[my_timer[i].Timer_relay] = true;
        } else { 
          if ((time_now >= time_on) || (time_now < time_off))
            relay_state[my_timer[i].Timer_relay] = true;
        }
      } 
    }
    if (DBG_relay) {
      String strState = "";
      for (uint8_t i=0; i < relay_num; i++)
        strState += String(relay_state[i]) + ", ";
      strState += "статусы по таймерам";
      Serial.println(strState);
      strState = "";
      for (uint8_t i=0; i < relay_num; i++)
        strState += String(ChOnOff[i]) + ", ";
      strState += "статусы по реле";
      Serial.println(strState);
    }
    if (mode == "TMR") 
      if (XOR(ChOnOff[0], relay_state[0])) 
        relayState(0, relay_state[0], "in timer controlled TMR mode");
    for (uint8_t i=1; i < relay_num; i++) 
      relayState(i, relay_state[i], "controlled by a timers");
    del_timer = millis();
  }
}

void ShowFeeding();

void feed_handle(unsigned int delfeed) {
  static unsigned long del_feeder;
  unsigned int time_now;
  unsigned int feeding;

  if (del_feeder + delfeed < millis()) { // Если интервал уже истек
    time_now = NTP.hour() * 60 + NTP.minute();
    for (uint8_t i=0; i < 2; i++) {
      if (Feeds[i].feed_sw) {
        feeding = Feeds[i].feed_start.hour * 60 + Feeds[i].feed_start.minute;
        if ((time_now >= feeding) && (time_now < (feeding + 2))) {
          if (!feedOK) {
            ShowFeeding();
            feedOK = true;
          }
        } else {
          feedOK = false;
        }
      }  
    }
    del_feeder = millis();
  }
}

void RelayZakatAfter() {
  if (zakat_sw) {
    if (ChOnOff[zakat_rel]) { 
      Serial.println(strTimeNow() + " Возвращаем реле заката в 1");
      if (zakat_inv) {
          analogWrite(relays[zakat_rel], 1023 - zakat_max);
      } else {
        analogWrite(relays[zakat_rel], zakat_max);
      }
    } else {
      Serial.println(strTimeNow() + " Возвращаем реле заката в 0");
      digitalWrite(relays[zakat_rel], zakat_inv);
    }
  } else {
    digitalWrite(relays[zakat_rel], XOR(ChOnOff[zakat_rel], RelayUp));
    Serial.println(strTimeNow() + " Возвращаем реле заката в прежнюю позицию");
  }
}