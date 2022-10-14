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
      eff_time = eff_clock;
    }

    // Строки
    if (portal.clickString("ntps", valNTPs)) {
      if (DBG_portal) {
        Serial.print("Text: ");
        Serial.println(valNTPs);
      }
    }

    if (portal.clickInt("tz", valTZ)) {
      if (DBG_portal) {
        Serial.print("Number: ");
        Serial.println(valTZ);
      }
    }

    // if (portal.clickFloat("fnum", valFloat)) {
    //   Serial.print("Float: ");
    //   Serial.println(valFloat);
    // }

    // Слайдеры
    if (portal.clickInt("sld", valNTPreq)) {
      if (DBG_portal) {
        Serial.print("***Slider. Частота запросов: ");
        Serial.println(valNTPreq);
      }
    } 

    if (portal.clickInt("prg", prg)) {
      if (DBG_portal) {
        Serial.print("***Slider. Порог срабатывания реле: ");
        Serial.print(prg);
        Serial.println("°С");
      }
      p_tem = prg;
    }

    if (portal.clickInt("time_view", time_view)) {
      if (DBG_portal) {
        Serial.print("***Slider. Период показа времени: ");
        Serial.println(time_view);
      }
      YearTime = time_view;
    } 

    if (portal.clickInt("data_view", data_view)) {
      if (DBG_portal) {
        Serial.print("***Slider. Период показа данных: ");
        Serial.println(data_view);
      }
      YearView = data_view;
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
      td = eff_speed;
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
      if (portal.clickTime((String("tmr_start") + i), Timer_start[i])) {
        if (DBG_portal) {
          Serial.print((String("Таймер старт №") + (i + 1) + ": "));
          Serial.println(Timer_start[i].encode());
        }
      }
      if (portal.clickTime((String("tmr_stop") + i), Timer_stop[i])) {
        if (DBG_portal) {
          Serial.print((String("Таймер стоп №") + (i + 1) + ": "));
          Serial.println(Timer_stop[i].encode());
        }
      }
      if (portal.clickInt((String("tmr_days") + i), Timer_days[i])) {
        Timers_week[i] = weekday_set[Timer_days[i]];
        if (DBG_portal) {
          Serial.print((String("Таймер №") + (i + 1) + " роботает по дням недели "));
          Serial.println(Timers_week[i]);
        }
      }
      if (portal.clickInt((String("tmr_relays") + i), Timer_relay[i])) {
        if (DBG_portal) {
          Serial.print((String("Таймер №") + (i + 1) + " теперь управляет реле "));
          Serial.println(Timer_relay[i]+1);
        }
      }
    }
    for (uint8_t i = 0; i < 2; i++) {
      if (portal.clickTime((String("feed_time") + i), feed_start[i])) {
        if (DBG_portal) {
          Serial.print((String("Кормление ") + (i + 1) + " стартует в "));
          Serial.println(feed_start[i].encode());
        }
      }
      if (portal.clickBool((String("feed_sw") + i), feed_sw[i])) {
        // feed_sw[i] = portal.getBool((String("feed_sw") + i));
        if (DBG_portal) {
          Serial.print((String("Кормление ") + (i + 1)));
          if (feed_sw[i]) Serial.println(" включено");
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
    if (portal.updateSub("lbl")) {   // начинается с lbl
      // формируем ответ вида "lbl #0: 123"
      String s;
      s += "lbl #";
      s += portal.updateNameSub(1);
      s += ":";
      s += random(2);
      portal.answer(s);
    }
  }
}
