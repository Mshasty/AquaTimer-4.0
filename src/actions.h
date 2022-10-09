void action() {
  // был клик по компоненту
  if (portal.click()) {
    // проверяем компоненты и обновляем переменные
    
    // 1. переписали вручную
    if (portal.click("t1d1")) {
      t1d1 = portal.getBool("t1d1");
      Serial.print("Таймер №1 в понедельник : ");
      Serial.println(t1d1);
    }

    // Переключатели
    if (portal.clickBool("sw", valSwitch)) {
      Serial.print("Switch: ");
      Serial.println(valSwitch);
      if (mode == "MAN") {
        relay_on = valSwitch;
        relayState(RELAY, valSwitch, "relay");
      }
    }

    if (portal.clickBool("eff_clock", eff_clock)) {
      Serial.print("Показ эффекта перед часами: ");
      Serial.println(eff_clock);
      eff_time = eff_clock;
    }

    // Строки
    if (portal.clickString("ntps", valNTPs)) {
      Serial.print("Text: ");
      Serial.println(valNTPs);
    }

    if (portal.clickInt("tz", valTZ)) {
      Serial.print("Number: ");
      Serial.println(valTZ);
    }

    // if (portal.clickFloat("fnum", valFloat)) {
    //   Serial.print("Float: ");
    //   Serial.println(valFloat);
    // }

    // if (portal.clickStr("pass", valPass)) {
    //   Serial.print("Password: ");
    //   Serial.println(valPass);
    // }

    // Слайдеры
    if (portal.clickInt("sld", valNTPreq)) {
      Serial.print("***Slider. Частота запросов: ");
      Serial.println(valNTPreq);
    } 

    if (portal.clickInt("prg", prg)) {
      Serial.print("***Slider. Порог срабатывания реле: ");
      Serial.print(prg);
      Serial.println("°С");
      p_tem = prg;
    }

    if (portal.clickInt("time_view", time_view)) {
      Serial.print("***Slider. Период показа времени: ");
      Serial.println(time_view);
      YearTime = time_view;
    } 

    if (portal.clickInt("data_view", data_view)) {
      Serial.print("***Slider. Период показа данных: ");
      Serial.println(data_view);
      YearView = data_view;
    } 

    if (portal.clickInt("impuls", impuls)) {
      Serial.print("***Slider. Длина импульса: ");
      Serial.println(impuls);
    }

    if (portal.clickInt("led_light", led_light)) {
      Serial.print("***Slider. Яркость LED, %: ");
      Serial.println(led_light);
      led_intens = led_light/100;
      lc.setIntensity(0, led_intens);
    }

    // if (portal.clickDate("date", Timer1start)) {
    //   Serial.print("Date: ");
    //   Serial.println(Timer1start.encode());
    //   Serial.println(Timer1start.day );
    // }

    if (portal.clickTime("timer1start", Timer1start)) {
      Serial.print("Time: ");
      Serial.println(Timer1start.encode());
      Serial.println(Timer1start.hour);
    }

    if (portal.clickColor("col", valCol)) {
      Serial.print("Color: ");
      Serial.println(valCol.encode());
    }

    if (portal.clickInt("sel", valSelect)) {
      Serial.print("Режим работы реле 1: ");
      if (valSelect==1) {
        Serial.println("Температура");
        mode = "TEM";
      } else if (valSelect==2) {
        Serial.println("Ручной");
        mode = "MAN";
      } else {
        Serial.println("Таймер 1");
        mode = "TMR";
      }
    }

    if (portal.click("btn")) {
      Serial.println("Button click");
      ESP.restart();
    }
    if (portal.clickUp("btn")) Serial.println("Button up");
    if (portal.clickDown("btn")) Serial.println("Button down");
  }

  if (portal.update()) {
    portal.updateString("temp", temp);

    if (portal.updateSub("led")) {   // начинается с led
      int ss;
      ss = random(1);
      if (ss) portal.answer("#");
      else portal.answer("");
    }
    if (portal.updateSub("lbl")) {   // начинается с lbl
      // формируем ответ вида "lbl #0: 123"
      String s;
      s += "lbl #";
      s += portal.updateNameSub(1);
      s += ":";
      s += random(10);
      portal.answer(s);
    }
  }
}
