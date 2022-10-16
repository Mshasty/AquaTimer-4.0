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
    s += "rled";
    s += i;
    s += ',';
  }
  s += "temp,ldt1,ldt2,tnow";
  GP.UPDATE(s);
  if (DBG) {
    Serial.print("Строка обновления: ");
    Serial.println(s);
  }

  GP.TITLE("AquaTimer 4.0");
  GP.HR();

  GP.NAV_TABS_LINKS("/,/timers,/feeding,/setting", "Main,Timers,Feed,SetUp");
  
  // GP_MAKE_SPOILER(eff_speed
  //   "Spoiler",
  //   GP.LABEL("Hello!");
  // );

  if (portal.uri("/timers")) {  

    
    for (uint8_t i = 0; i < timers_num; i++) {
      GP_MAKE_BLOCK_THIN_TAB(
        (String("Таймер № ") + (i+1)),
        // GP.LABEL("Период включения");
        GP_MAKE_BOX(GP.LABEL("╘═╛");   GP.TIME((String("tmr_start") + i), my_timer[i].Timer_start);   GP.TIME((String("tmr_stop") + i), my_timer[i].Timer_stop);  );
        GP_MAKE_BOX(GP.LABEL("Дни"); GP.SELECT((String("tmr_days") + i), "√√√√√√√,√√√√√∙∙,∙∙∙∙∙√√,√∙√∙√∙√,∙√∙√∙√∙,.......", my_timer[i].Timer_days);  GP.LABEL("Реле"); GP.SELECT((String("tmr_relays") + i), "1,2,3,4", my_timer[i].Timer_relay); );
      );
    }

  } else if (portal.uri("/feeding")) {
    
    GP_MAKE_BOX(GP_CENTER, GP.TEXT("tnow", "Time"); );

    GP_MAKE_BLOCK_TAB(
      "Кормление",
      for (uint8_t i = 0; i < 2; i++) {
        GP_MAKE_BOX(GP.LABEL((String("▼ ") + (i+1)));  GP.CHECK((String("feed_sw") + i), Feeds[i].feed_sw);  GP.TIME((String("feed_time") + i), Feeds[i].feed_start);  );
      }
      //GP_MAKE_BOX(GP.LABEL("Первое:");  GP.CHECK("feed1ch", feed1ch);  GP.TIME("feed1start", feed1start);  );
      //GP_MAKE_BOX(GP.LABEL("Второе:");  GP.CHECK("feed2ch", feed2ch);  GP.TIME("feed2start", feed2start);  );
      GP.LABEL("Длительность, mS:");
      GP.SLIDER("impuls", FeedDelay, 300, 5000, 100);
      GP.BUTTON("btn_feed", "Покормить");
    );

  } else if (portal.uri("/setting")) {
    
    GP_MAKE_BLOCK_THIN_TAB(
      "NTP",
      GP_MAKE_BOX(GP.LABEL("Server:");     GP.TEXT("ntps", "text", ntp_srv); );
      GP_MAKE_BOX(GP.LABEL("TimeZone:");   GP.NUMBER("tz", "number", GMT_OFF); );
      GP_MAKE_BOX(GP.LABEL("Период:");   GP.SLIDER("sld", ntp_req, 15, 1080, 15);  );
    );

    GP_MAKE_BLOCK_THIN_TAB(
      "Часы",
      GP_MAKE_BOX(GP.LABEL("Показ времени:");   GP.SLIDER("time_view", YearTime, 5, 35);  );
      GP_MAKE_BOX(GP.LABEL("Показ данных:");   GP.SLIDER("data_view", YearView, 3, 20);  ); 
      GP_MAKE_BOX(GP.LABEL("Яркость:");   GP.SLIDER("led_light", led_light, 10, 100, 10);  ); 
      GP.BREAK();
      GP_MAKE_BOX(GP.LABEL("Эффект перед часами:"); GP.SWITCH("eff_clock", eff_clock); );
      GP_MAKE_BOX(GP.LABEL("Скорость эффекта:");   GP.SLIDER("eff_speed", eff_speed, 5, 45, 5);  );
    );

    GP_MAKE_BLOCK_THIN_TAB(
      "Инверсия",
      GP_MAKE_BOX(GP.LABEL("Реле:"); GP.SWITCH("invrs", RelayUp); );
      GP_MAKE_BOX(GP.LABEL("Мотор:"); GP.SWITCH("motor", VibroUp); );
    );
    
    GP_MAKE_BLOCK_THIN_TAB(
      "Конфигурация",
      GP_MAKE_BOX(GP_CENTER, GP.BUTTON("load_conf", "Load"); GP.BUTTON("save_conf", "Save"); );
    );

    GP.BUTTON("btn", "Reset ESP");
  } else {

    GP_MAKE_BLOCK_TAB(
      "Термостат",
      GP_MAKE_BOX(GP_CENTER, GP.LABEL("Температура"); GP.LED("ldt1"); GP.TEXT("temp", "Датчик", temp); GP.LED("ldt2"); );
      GP_MAKE_BOX(GP.LABEL("Порог:");   GP.SLIDER("p_tem", p_tem, 15, 39);  );
      GP_MAKE_BOX(GP.LABEL("Гистерезис:");   GP.SLIDER("h_tem", h_tem, 0, 7);  );
    );
    
    GP_MAKE_BLOCK_TAB(
      "Блок реле",
      GP_MAKE_BOX(GP.LABEL("Режим реле 1:");   GP.SELECT("sel", "Таймер 1,Температура,Ручной", valSelect);  );
      GP_MAKE_BOX(GP.LABEL("Ручное управление:"); GP.SWITCH("sw", valSwitch); );
      GP_MAKE_BOX(GP.LABEL("Статус реле:");
        // создаём индикаторы статуса реле с именами rled0,rled1...
        for (int i = 0; i < relay_num; i++) {
          GP.LED(String("rled") + i, 0);
        }
      );
    );

    GP_MAKE_BLOCK_TAB(
      "Процессы",
      GP_MAKE_BOX(GP_CENTER, GP.LABEL("Процессы:");
        // создаём лейблы с именами led/0,led/1...
        for (int i = 0; i < 5; i++) {
          GP.LED(String("led/") + i, "#");
        }
      );
    );

  }
  GP.BREAK();
  GP.NAV_TABS_LINKS("/,/timers,/feeding,/setting", "Main,Timers,Feed,SetUp");
  
  GP.BUILD_END();
}


