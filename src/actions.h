void action() {
  // был клик по компоненту
  if (portal.click()) {
    // проверяем компоненты и обновляем переменные
    
    // Переключатели
    if (portal.clickBool("sw", valSwitch)) {
      if (DBG_portal) {
        Serial.print("Switch: ");
        Serial.println(valSwitch);
      }
      if (mode == "MAN") {
        relay_on = valSwitch;
        relayState(RELAY, valSwitch, "relay");
      }
    }

    if (portal.clickBool("eff_clock", eff_clock)) {
      if (DBG_portal) {
        Serial.print("Показ эффекта перед часами: ");
        Serial.println(eff_clock);
      }
    }

    if (portal.clickBool("invrs", RelayUp)) {
      if (DBG_portal) {
        Serial.print("Инверсия реле: ");
        Serial.println(RelayUp);
      }
    }

    // Строки
    if (portal.clickStr("ntps", ntp_srv)) {
      if (DBG_portal) {
        Serial.print("NTP server: ");
        Serial.println(ntp_srv);
      }
    }

    if (portal.clickInt("tz", GMT_OFF)) {
      if (DBG_portal) {
        Serial.print("Number: ");
        Serial.println(GMT_OFF);
      }
    }

    // if (portal.clickFloat("fnum", valFloat)) {
    //   Serial.print("Float: ");
    //   Serial.println(valFloat);
    // }

    // Слайдеры
    if (portal.clickInt("sld", ntp_req)) {
      if (DBG_portal) {
        Serial.print("***Slider. Частота запросов: ");
        Serial.println(ntp_req);
      }
    } 

    if (portal.clickInt("p_tem", p_tem)) {
      if (DBG_portal) {
        Serial.print("***Slider. Порог срабатывания реле: ");
        Serial.print(p_tem);
        Serial.println("°С");
      }
    }

    if (portal.clickInt("h_tem", h_tem)) {
      if (DBG_portal) {
        Serial.print("***Slider. Гистерезис срабатывания реле: ");
        Serial.print(h_tem);
        Serial.println("°С");
      }
    }

    if (portal.clickInt("time_view", YearTime)) {
      if (DBG_portal) {
        Serial.print("***Slider. Период показа времени: ");
        Serial.println(YearTime);
      }
    } 

    if (portal.clickInt("data_view", YearView)) {
      if (DBG_portal) {
        Serial.print("***Slider. Период показа данных: ");
        Serial.println(YearView);
      }
    } 

    if (portal.clickInt("impuls", FeedDelay)) {
      if (DBG_portal) {
        Serial.print("***Slider. Длина импульса: ");
        Serial.println(FeedDelay);
      }
    }

    if (portal.clickInt("eff_speed", eff_speed)) {
      if (DBG_portal) {
        Serial.print("***Slider. Задержка эффекта перед часами: ");
        Serial.println(eff_speed);
      }
    }

    if (portal.clickInt("led_light", led_light)) {
      if (DBG_portal) {
        Serial.print("***Slider. Яркость LED, %: ");
        Serial.println(led_light);
      }
      led_intens = led_light/100;
      lc.setIntensity(0, led_intens);
    }

    // таймеры
    for (uint8_t i = 0; i < timers_num; i++) {
      if (portal.clickTime((String("tmr_start") + i), my_timer[i].Timer_start)) {
        if (DBG_portal) {
          Serial.print((String("Таймер старт №") + (i + 1) + ": "));
          Serial.println(my_timer[i].Timer_start.encode());
        }
      }
      if (portal.clickTime((String("tmr_stop") + i), my_timer[i].Timer_stop)) {
        if (DBG_portal) {
          Serial.print((String("Таймер стоп №") + (i + 1) + ": "));
          Serial.println(my_timer[i].Timer_stop.encode());
        }
      }
      if (portal.clickInt((String("tmr_days") + i), my_timer[i].Timer_days)) {
        my_timer[i].Timer_week = weekday_set[my_timer[i].Timer_days];
        if (DBG_portal) {
          Serial.print((String("Таймер №") + (i + 1) + " роботает по дням недели "));
          Serial.println(my_timer[i].Timer_week);
        }
      }
      if (portal.clickInt((String("tmr_relays") + i), my_timer[i].Timer_relay)) {
        if (DBG_portal) {
          Serial.print((String("Таймер №") + (i + 1) + " теперь управляет реле "));
          Serial.println(my_timer[i].Timer_relay+1);
        }
      }
    }
    for (uint8_t i = 0; i < 2; i++) {
      if (portal.clickTime((String("feed_time") + i), Feeds[i].feed_start)) {
        if (DBG_portal) {
          Serial.print((String("Кормление ") + (i + 1) + " стартует в "));
          Serial.println(Feeds[i].feed_start.encode());
        }
      }
      if (portal.clickBool((String("feed_sw") + i), Feeds[i].feed_sw)) {
        // Feeds[i].feed_sw = portal.getBool((String("feed_sw") + i));
        if (DBG_portal) {
          Serial.print((String("Кормление ") + (i + 1)));
          if (Feeds[i].feed_sw) Serial.println(" включено");
          else Serial.println(" выключено");
        }
      }
    }

    // if (portal.clickColor("col", valCol)) {
    //   Serial.print("Color: ");
    //   Serial.println(valCol.encode());
    // }

    if (portal.clickInt("sel", valSelect)) {
      if (DBG_portal) Serial.print("Режим работы реле 1: ");
      if (valSelect==1) {
        if (DBG_portal) Serial.println("Температура");
        mode = "TEM";
      } else if (valSelect==2) {
        if (DBG_portal) Serial.println("Ручной");
        mode = "MAN";
      } else {
        if (DBG_portal) Serial.println("Таймер 1");
        mode = "TMR";
      }
    }

    // Кнопки
    if (portal.click("btn_feed")) {
      if (DBG_portal) Serial.println("Feed button click");
      ShowFeeding();
    } 

    if (portal.click("load_conf")) {
      if (DBG_portal) Serial.println("Load button click");
      if (eeprom_read()) {
        if (DBG_portal) Serial.println("Конфигурация загружена");
        //portal.start();
        portal.show();
      }
      else Serial.println("Ошибка загрузки конфигурации");
    }

    if (portal.click("save_conf")) {
      if (DBG_portal) Serial.println("Конфигурация сохранена");
      eeprom_write();
    }
    
    if (portal.click("btn")) {
      if (DBG_portal) Serial.println("Reset click");
      ESP.restart();
    }
    if (portal.clickUp("btn")) Serial.println("Button up");
    if (portal.clickDown("btn")) Serial.println("Button down");
  }

  if (portal.update()) {
    //portal.updateString("temp", temp);
    if (portal.update("temp")) portal.answer(String(ds_tem, 2) + "°С");

    //if (portal.update("tn")) portal.answer(String(hour())+":"+minute()+":"+second());
    if (portal.update("tnow")) portal.answer("  " + day_week(weekday()) + " " + lz(day()) + "." + lz(month()) + "." + lz(year()-2000) + " " + lz(hour()) + ":" + lz(minute()) );

    if (portal.updateSub("led")) portal.answer(random(2));
    if (portal.updateSub("ldt1")) portal.answer(tempOK);
    if (portal.updateSub("ldt2")) portal.answer(!tempOK);
    // if (portal.updateSub("lbl")) {   // начинается с lbl
    //   // формируем ответ вида "lbl #0: 123"
    //   String s;
    //   s += "lbl #";
    //   s += portal.updateNameSub(1);
    //   s += ":";
    //   s += random(2);
    //   portal.answer(s);
    // }
  }
}
