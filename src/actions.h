void action() {
  // был клик по компоненту
  if (portal.click()) {
    // проверяем компоненты и обновляем переменные
    
    // Переключатели
    if (portal.clickBool("sw", valSwitch)) {
      if (DBG_portal) {
        Serial.print("***Switch. Ручное управление 1-ым реле: ");
        Serial.println(valSwitch);
      }
      if (mode == "MAN") {
        // relay_on = valSwitch;
        relayState(0, valSwitch, "switch manual");
      }
    }

    if (portal.clickBool("eff_clock", eff_clock)) {
      if (DBG_portal) {
        Serial.print("***Switch. Показ эффекта перед часами: ");
        Serial.println(eff_clock);
      }
    }

    if (portal.clickBool("beep", BeepClockOn)) {
      if (DBG_portal) {
        Serial.print("***Switch. Бипер каждый час: ");
        Serial.println(BeepClockOn);
      }
    }

    // переключение режима закат/рассвет  
    if (portal.clickBool("zakat_sw", zakat_sw)) {
      if (DBG_portal) {
        Serial.print("***Switch. Режим закат/рассвет: ");
        Serial.println(zakat_sw);
      }
      RelayZakatAfter();    
    }

    if (portal.clickBool("zakat_inv", zakat_inv)) {
      if (DBG_portal) {
        Serial.print("***Switch. Инверсия закат/рассвет: ");
        Serial.println(zakat_inv);
      }
      RelayZakatAfter();
    }

    if (portal.clickBool("invrs", RelayUp)) {
      if (DBG_portal) {
        Serial.print("***Switch. Инверсия реле: ");
        Serial.println(RelayUp);
      }
      for (uint8_t i=0; i < relay_num; i++) {
        relayState(i, !ChOnOff[i], "inverted by menu");
        delay(100);
        relayState(i, ChOnOff[i], "change polarity");
      }
    }

    if (portal.clickBool("motor", VibroUp)) {
      if (DBG_portal) {
        Serial.print("***Switch. Инверсия управления мотором: ");
        Serial.println(VibroUp);
      }
      if (VibroUp) digitalWrite(VibroPin, HIGH);
      else digitalWrite(VibroPin, LOW);
    }

    // Строки
    if (portal.clickStr("ntps", ntp_srv)) {
      if (DBG_portal) {
        Serial.print("***String. NTP server: ");
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

    if (portal.clickInt("zakat_del", zakat_del)) {
      if (DBG_portal) {
        Serial.print("***Slider. Длительность рассвета, сек: ");
        Serial.println(zakat_del);
      }
    }

    if (portal.clickInt("zakat_min", zakat_min)) {
      if (DBG_portal) {
        Serial.print("***Slider. Начальное значение ШИМ: ");
        Serial.println(zakat_min);
      }
      if (zakat_inv) analogWrite(relays[zakat_rel], 1023 - zakat_min);
      else analogWrite(relays[zakat_rel], zakat_min);
    }

    if (portal.clickInt("zakat_max", zakat_max)) {
      if (DBG_portal) {
        Serial.print("***Slider. Конечное значение ШИМ: ");
        Serial.println(zakat_max);
      }
      if (zakat_inv) analogWrite(relays[zakat_rel], 1023 - zakat_max);
      else analogWrite(relays[zakat_rel], zakat_max);
    }

    if (portal.clickBool("night_sw", my_light.Night_sw)) {
      if (DBG_portal) {
        Serial.print("***Switch. Ночной режим: ");
        Serial.println(my_light.Night_sw);
      }
    }

    if (portal.clickInt("led_light", led_light)) {
      if (DBG_portal) {
        Serial.print("***Slider. Яркость LED днём: ");
        Serial.println(led_light);
      }
      if (!NightMode) {
        led_intens = led_light;
#ifdef disp_led7seg
        lc.setIntensity(0, led_intens);
#else
        disp.brightness(led_intens);
        Serial.print("***Slider. Дневная яркость LED: ");
        Serial.println(led_intens);
#endif

      }
    }

    if (portal.clickInt("led_dark", led_dark)) {
      if (DBG_portal) {
        Serial.print("***Slider. Яркость LED ночью: ");
        Serial.println(led_dark);
      }
      if (NightMode) {
        led_intens = led_dark;
#ifdef disp_led7seg
        lc.setIntensity(0, led_intens);
#else
        disp.brightness(led_intens);
        Serial.print("***Slider. Ночная яркость LED: ");
        Serial.println(led_intens);
#endif
      }
    }

    // таймеры
    for (uint8_t i = 0; i < timers_num; i++) {
      if (portal.clickTime((String("tmr_start") + i), my_timer[i].Timer_start)) {
        if (DBG_portal) {
          Serial.print((String("***Time. Таймер старт №") + (i + 1) + ": "));
          Serial.println(my_timer[i].Timer_start.encode());
        }
      }
      if (portal.clickTime((String("tmr_stop") + i), my_timer[i].Timer_stop)) {
        if (DBG_portal) {
          Serial.print((String("***Time. Таймер стоп №") + (i + 1) + ": "));
          Serial.println(my_timer[i].Timer_stop.encode());
        }
      }
      if (portal.clickInt((String("tmr_days") + i), my_timer[i].Timer_days)) {
        my_timer[i].Timer_week = weekday_set[my_timer[i].Timer_days];
        if (DBG_portal) {
          Serial.print((String("***Time. Таймер №") + (i + 1) + " работает по дням недели 0x"));
          Serial.println(my_timer[i].Timer_week, HEX);
        }
      }
      if (portal.clickInt((String("tmr_relays") + i), my_timer[i].Timer_relay)) {
        if (DBG_portal) {
          Serial.print((String("***Time. Таймер №") + (i + 1) + " теперь управляет реле "));
          Serial.println(my_timer[i].Timer_relay+1);
        }
      }
    }
    for (uint8_t i = 0; i < 2; i++) {
      if (portal.clickTime((String("feed_time") + i), Feeds[i].feed_start)) {
        if (DBG_portal) {
          Serial.print((String("***Time. Кормление ") + (i + 1) + " стартует в "));
          Serial.println(Feeds[i].feed_start.encode());
        }
      }
      if (portal.clickBool((String("feed_sw") + i), Feeds[i].feed_sw)) {
        // Feeds[i].feed_sw = portal.getBool((String("feed_sw") + i));
        if (DBG_portal) {
          Serial.print(strTimeNow() + String(" Кормление ") + (i + 1));
          if (Feeds[i].feed_sw) Serial.println(" включено");
          else Serial.println(" выключено");
        }
      }
    }

    if (portal.clickTime("night_on", my_light.Night_on)) {
      if (DBG_portal) {
        Serial.print("***Time. Старт ночного режима: ");
        Serial.println(my_light.Night_on.encode());
      }
    }

    if (portal.clickTime("night_off", my_light.Night_off)) {
      if (DBG_portal) {
        Serial.print("***Time. Конец ночного режима: ");
        Serial.println(my_light.Night_off.encode());
      }
    }

    // if (portal.clickColor("col", valCol)) {
    //   Serial.print("Color: ");
    //   Serial.println(valCol.encode());
    // }

    if (portal.clickInt("zakat_rel", zakat_rel)) {
      if (DBG_portal) {
        Serial.print("***Select. Реле заката/рассвета: ");
        Serial.println(zakat_rel + 1);
      }
    }
    
    if (portal.clickInt("sel", valSelect)) {
      if (DBG_portal) Serial.print("***Select. Режим работы реле 1: ");
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
      if (DBG_portal) Serial.println(strTimeNow() + " Feed button click");
      ShowFeeding();
    } 

    if (portal.click("load_conf")) {
      if (DBG_portal) Serial.println("Load button click");
      if (eeprom_read()) {
        if (DBG_portal) Serial.println(strTimeNow() + " Конфигурация загружена");
        //portal.start();
        ShowDownload();
        portal.show();
      }
      else Serial.println(strTimeNow() + " Ошибка загрузки конфигурации");
    }

    if (portal.click("save_conf")) {
      if (DBG_portal) Serial.println(strTimeNow() + " Конфигурация сохранена");
      eeprom_write();
      ShowUpload();
    }
    
    if (portal.click("btn")) {
      if (DBG_portal) Serial.println("Reset click");
      ESP.restart();
    }
    if (portal.clickUp("btn")) Serial.println("Button up");
    if (portal.clickDown("btn")) Serial.println("Button down");
  }

  if (portal.update()) {
    if (portal.update("temp")) portal.answer(String(tem, 2) + "°С");

    if (portal.update("tnow")) portal.answer("  " + day_week(NTP.weekDay()) + " " + lz(NTP.day()) + "." + lz(NTP.month()) + "." + lz(NTP.year()-2000) + " " + lz(NTP.hour()) + ":" + lz(NTP.minute()) );

    if (portal.updateSub("led")) portal.answer(random(2));
    if (portal.updateSub("ldt1")) portal.answer(tempOK);
    if (portal.updateSub("ldt2")) portal.answer(!tempOK);
    for (uint8_t i=0; i < relay_num; i++) {
      if (portal.updateSub(String("rled")+i)) portal.answer(ChOnOff[i]);
    }
  }
}
