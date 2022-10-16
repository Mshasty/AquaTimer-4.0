boolean DBG_relay = false;

void relayState(uint8_t r_num, boolean state, String mess){

    if (!XOR(ChOnOff[r_num], state)) return;
    digitalWrite(relays[r_num], XOR(state, RelayUp));
    ChOnOff[r_num] = state;
    String umes = "Relay " + String(r_num+1) + " " + mess;
    if (state) umes += " is switch on";
    else  umes += " is switch off";
    Serial.println(umes);

}

void relay(){
  static unsigned long i;
  
  if(i + 1000 > millis()) return;
  i = millis();
  tem = ds_tem;
  
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
  int wd = weekday() - 1;
  
  if (!wd) wd = 7;
  if (wd > 1) bitDay <<= (wd-1);

  for (uint8_t i=0; i < relay_num; i++) { // инициализируем статусы
    relay_state[i] = false;
  }

  if (del_timer + deltime < millis()) { // Если интервал уже истек
    time_now = hour() * 60 + minute();
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
        relayState(0, relay_state[0], "in mode TMR controlled by a timers");
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
    time_now = hour() * 60 + minute();
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