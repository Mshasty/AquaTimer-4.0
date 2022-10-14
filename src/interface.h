void build() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  //GP.THEME(GP_LIGHT);

  String s;
  // формируем список для UPDATE
  // вида "lbl/0,lbl/1..."
  for (int i = 0; i < 5; i++) {
    s += "led/";
    s += i;
    s += ',';
  }
  s += "temp,ldt1,ldt2";
  GP.UPDATE(s);
  if (DBG) {
    Serial.print("Строка обновления: ");
    Serial.println(s);
  }

  GP.TITLE("AquaTimer 4.0");
  GP.HR();

  GP.NAV_TABS_LINKS("/,/timers,/setting", "Main,Timers,Settings");
  
  // GP_MAKE_SPOILER(
  //   "Spoiler",
  //   GP.LABEL("Hello!");
  // );

  if (portal.uri("/timers")) {  

    
    for (uint8_t i = 0; i < timers_num; i++) {
      GP_MAKE_BLOCK_TAB(
        (String("Таймер № ") + (i+1)),
        // GP.LABEL("Период включения");
        GP_MAKE_BOX(GP.LABEL("╘═╛");   GP.TIME((String("tmr_start") + i), Timer_start[i]);   GP.TIME((String("tmr_stop") + i), Timer_stop[i]);  );
        GP_MAKE_BOX(GP.LABEL("Дни"); GP.SELECT((String("tmr_days") + i), "√√√√√√√,√√√√√∙∙,∙∙∙∙∙√√,√∙√∙√∙√,∙√∙√∙√∙", Timer_days[i]);  GP.LABEL("Реле"); GP.SELECT((String("tmr_relays") + i), "1,2,3,4,5", Timer_relay[i]); );
      );
    }

    // GP_MAKE_BLOCK_TAB(
    //   "Таймер № 1",
    //   // GP.LABEL("Период включения");
    //   GP_MAKE_BOX(GP.LABEL("☼˽");   GP.TIME("timer1start", Timer_start[0]);   GP.TIME("timer1stop", Timer_stop[0]);  );
    //   GP_MAKE_BOX(GP.LABEL("Дни"); GP.SELECT("days1", "√√√√√√√,√√√√√∙∙,∙∙∙∙∙√√,√∙√∙√∙√,∙√∙√∙√∙", days1);  GP.LABEL("Реле"); GP.SELECT("rt1", "1,2,3,4,5", rt1); );
    //   //GP_MAKE_BOX(GP.LABEL("Дни недели:"); GP.CHECK("t1d1", t1d1); GP.CHECK("t1d2", t1d2); GP.CHECK("t1d3", t1d3); GP.CHECK("t1d4", t1d4); GP.CHECK("t1d5", t1d5); GP.CHECK("t1d6", t1d6); GP.CHECK("t1d7", t1d7);   );
    //   // GP_MAKE_BOX(GP.LABEL("Реле таймера:"); GP.SELECT("rt1", "1,2,3,4,5", rt1);  );
    // );

    // GP_MAKE_BLOCK_TAB(
    //   "Таймер № 2",
    //   // GP.LABEL("Период включения");
    //   GP_MAKE_BOX(GP.LABEL("≈˽");   GP.TIME("timer2start", Timer2start);   GP.TIME("timer2stop", Timer2stop);  );
    //   GP_MAKE_BOX(GP.LABEL("Дни"); GP.SELECT("days2", "√√√√√√√,√√√√√∙∙,∙∙∙∙∙√√,√∙√∙√∙√,∙√∙√∙√∙", days2);  GP.LABEL("Реле"); GP.SELECT("rt2", "1,2,3,4,5", rt2); );
    // );

    // GP_MAKE_BLOCK_TAB(
    //   "Таймер № 3",
    //   // GP.LABEL("Период включения");
    //   GP_MAKE_BOX(GP.LABEL("֍˽");   GP.TIME("timer3start", Timer3start);   GP.TIME("timer3stop", Timer3stop);  );
    //   GP_MAKE_BOX(GP.LABEL("Дни"); GP.SELECT("days3", "√√√√√√√,√√√√√∙∙,∙∙∙∙∙√√,√∙√∙√∙√,∙√∙√∙√∙", days3);  GP.LABEL("Реле"); GP.SELECT("rt3", "1,2,3,4,5", rt3); );
    // );

    // GP_MAKE_BLOCK_TAB(
    //   "Таймер № 4",
    //   // GP.LABEL("Период включения");
    //   GP_MAKE_BOX(GP.LABEL("Ω˽");   GP.TIME("timer4start", Timer4start);   GP.TIME("timer4stop", Timer4stop);  );
    //   GP_MAKE_BOX(GP.LABEL("Дни"); GP.SELECT("days4", "√√√√√√√,√√√√√∙∙,∙∙∙∙∙√√,√∙√∙√∙√,∙√∙√∙√∙", days4);  GP.LABEL("Реле"); GP.SELECT("rt4", "1,2,3,4,5", rt4); );
    // );


  } else if (portal.uri("/setting")) {
    
    GP_MAKE_BLOCK_TAB(
      "NTP",
      GP.LABEL("NTP-сервер");
      GP_MAKE_BOX(GP.LABEL("Server:");     GP.TEXT("ntps", "text", valNTPs); );
      GP_MAKE_BOX(GP.LABEL("TimeZone:");   GP.NUMBER("tz", "number", valTZ); );
      GP_MAKE_BOX(GP.LABEL("Период:");   GP.SLIDER("sld", valNTPreq, 15, 1080, 15);  );
      //GP_MAKE_BOX(GP.LABEL("Password"); GP.PASS("pass", "pass", valPass); );
      //GP.AREA("", 3, "Text area");
    );

    GP_MAKE_BLOCK_TAB(
      "Clock",
      GP_MAKE_BOX(GP.LABEL("Показ времени:");   GP.SLIDER("time_view", time_view, 5, 45);  );
      GP_MAKE_BOX(GP.LABEL("Показ данных:");   GP.SLIDER("data_view", data_view, 3, 30);  ); 
      GP_MAKE_BOX(GP.LABEL("Яркость:");   GP.SLIDER("led_light", led_light, 10, 100, 10);  ); 
      GP.BREAK();
      GP_MAKE_BOX(GP.LABEL("Эффект перед часами:"); GP.SWITCH("eff_clock", eff_clock); );
      GP_MAKE_BOX(GP.LABEL("Скорость эффекта:");   GP.SLIDER("eff_speed", eff_speed, 5, 120, 5);  );
    );
    
    GP_MAKE_BLOCK_THIN_TAB(
      "Save configuration",
      // GP.LABEL("Upload File");
      // GP_MAKE_BOX(
      //   GP_CENTER,
      //   GP.FILE_UPLOAD("");
      //   GP.FOLDER_UPLOAD("");
      // );
      GP_MAKE_BOX(GP_CENTER, GP.LABEL("Load:"); GP.FILE_UPLOAD(""); GP.LABEL("Save:"); GP.FILE_UPLOAD(""); );
    );

    GP.BUTTON("btn", "Reset ESP");
  } else {

    GP_MAKE_BLOCK_TAB(
      "Термостат",
      GP_MAKE_BOX(GP_CENTER, GP.LABEL("Температура"); GP.LED("ldt1"); GP.TEXT("temp", "Датчик", temp); GP.LED("ldt2"); );
      //GP_MAKE_BOX(GP_CENTER, GP.LABEL("Температура"); GP.LED("led1", "#"); GP.LABEL_BLOCK("temp", "ttt"); GP.LED("led2"); );
      GP_MAKE_BOX(GP.LABEL("Порог:");   GP.SLIDER("prg", prg, 15, 39);  );
      GP_MAKE_BOX(GP.LABEL("Гистерезис:");   GP.SLIDER("gsr", gsr, 0, 15);  );
    );
    
    GP_MAKE_BLOCK_TAB(
      "Блок реле",
      GP_MAKE_BOX(GP.LABEL("Режим реле 1:");   GP.SELECT("sel", "Таймер 1,Температура,Ручной", valSelect);  );
      GP_MAKE_BOX(GP.LABEL("Ручное управление:"); GP.SWITCH("sw", valSwitch); );
      GP_MAKE_BOX(GP.LABEL("Статус реле:");
        // создаём лейблы с именами led/0,led/1...
        for (int i = 0; i < 5; i++) {
          GP.LED(String("led/") + i, "#");
        }
      );
    );
    // for (int i = 0; i < 5; i++) {
    //   GP.LABEL_BLOCK(String(i), String("lbl/") + i);
    //   GP.BREAK();
    // }

    GP_MAKE_BLOCK_TAB(
      "Кормление",
      GP_MAKE_BOX(GP.LABEL("Первое:");  GP.CHECK("feed1ch", feed1ch);  GP.TIME("feed1start", feed1start);  );
      GP_MAKE_BOX(GP.LABEL("Второе:");  GP.CHECK("feed2ch", feed2ch);  GP.TIME("feed2start", feed2start);  );
      GP.LABEL("Длительность, mS:");
      GP.SLIDER("impuls", impuls, 0, 5000, 100);
    );

  }
  GP.NAV_TABS_LINKS("/,/timers,/setting", "Main,Timers,Settings");
  
  GP.BUILD_END();
}


