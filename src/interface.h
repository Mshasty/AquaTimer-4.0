void build() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  //GP.THEME(GP_LIGHT);

  String s;
  // формируем список для UPDATE
  // вида "lbl/0,lbl/1..."
  for (int i = 0; i < 7; i++) {
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

  GP.NAV_TABS_LINKS("/,/timers,/feeding,/clock,/setting", "Main,Timers,Feed,Clock,SetUp");
  
  if (portal.uri("/timers")) {  

    
    for (uint8_t i = 0; i < timers_num; i++) {
      GP_MAKE_BLOCK_THIN_TAB(
        (String("Таймер № ") + (i+1)),
        // GP.LABEL("Период включения");
        GP_MAKE_BOX(GP.LABEL("╘═╛"); GP.TIME((String("tmr_start") + i), my_timer[i].Timer_start); GP.TIME((String("tmr_stop") + i), my_timer[i].Timer_stop); );
        GP_MAKE_BOX(GP.LABEL("Дни"); GP.SELECT((String("tmr_days") + i), week_select, my_timer[i].Timer_days);  GP.LABEL("Реле"); GP.SELECT((String("tmr_relays") + i), "1,2,3,4", my_timer[i].Timer_relay); );
      );
    }

  } else if (portal.uri("/feeding")) {
    
    GP_MAKE_BOX(GP_CENTER, GP.TEXT("tnow", "Current time"); );

    GP_MAKE_BLOCK_TAB(
      "Кормление",
      for (uint8_t i = 0; i < 2; i++) {
        GP_MAKE_BOX(GP.LABEL((String("▼ ") + (i+1)));  GP.CHECK((String("feed_sw") + i), Feeds[i].feed_sw);  GP.TIME((String("feed_time") + i), Feeds[i].feed_start); );
      }
      GP.LABEL("Длительность, mS:");
      GP.SLIDER("impuls", FeedDelay, 300, 5000, 100);
      GP.BUTTON("btn_feed", "Покормить");
    );

  } else if (portal.uri("/clock")) {

    GP_MAKE_BLOCK_THIN_TAB(
      "NTP",
      GP_MAKE_BOX(GP.LABEL("Server:"); GP.TEXT("ntps", "text", ntp_srv); );
      GP_MAKE_BOX(GP.LABEL("TimeZone:"); GP.NUMBER("tz", "number", GMT_OFF); );
      GP_MAKE_BOX(GP.LABEL("Период:"); GP.SLIDER("sld", ntp_req, 15, 1080, 15); );
    );

    GP_MAKE_BLOCK_THIN_TAB(
      "Часы",
      GP_MAKE_BOX(GP.LABEL("Показ времени:"); GP.SLIDER("time_view", YearTime, 5, 35); );
      GP_MAKE_BOX(GP.LABEL("Показ данных:"); GP.SLIDER("data_view", YearView, 3, 20); ); 
      GP_MAKE_BOX(GP.LABEL("Эффект перед часами:"); GP.SWITCH("eff_clock", eff_clock); );
      GP_MAKE_BOX(GP.LABEL("Скорость эффекта:"); GP.SLIDER("eff_speed", eff_speed, 5, 45, 5); );
      GP_MAKE_BOX(GP.LABEL("Бип каждый час:"); GP.SWITCH("beep", BeepClockOn); );
      GP.BREAK();
      GP_MAKE_BOX(GP.LABEL("Ночной режим:"); GP.SWITCH("night_sw", my_light.Night_sw); );
      GP_MAKE_BOX(GP.LABEL("◌└┘"); GP.TIME("night_on", my_light.Night_on); GP.TIME("night_off", my_light.Night_off); );
      GP.BREAK(); GP.LABEL("Яркость:");
      GP_MAKE_BOX(GP.LABEL("Днём: "); GP.SLIDER("led_light", led_light, 10, 100, 10); );
      GP_MAKE_BOX(GP.LABEL("Ночью:"); GP.SLIDER("led_dark", led_dark, 5, 50, 5); );
    );

  } else if (portal.uri("/setting")) {
    
    GP_MAKE_BLOCK_THIN_TAB(
      "ШИМ",
      GP_MAKE_BOX(GP.LABEL("Закат/рассвет:"); GP.SWITCH("zakat_sw", zakat_sw); );
      GP_MAKE_BOX(GP.LABEL("Секунды: "); GP.SLIDER("zakat_del", zakat_del, 10, 360, 10); );
      String strZakatRelays = "1";
      for (uint8_t i = 1; i < relay_num; i++) strZakatRelays += (", " + String(i + 1));
      GP_MAKE_BOX(GP.LABEL("Канал закат/рассвет:"); GP.SELECT("zakat_rel", strZakatRelays, zakat_rel); );
      GP_MAKE_BOX(GP.LABEL("Инверсия канала:"); GP.SWITCH("zakat_inv", zakat_inv); );
      GP.BREAK();
      GP_MAKE_BOX(GP_CENTER, GP.LABEL("Крайние значения ШИМ"); );
      GP_MAKE_BOX(GP.LABEL("Min: "); GP.SLIDER("zakat_min", zakat_min, 3, 1023, 4); );
      GP_MAKE_BOX(GP.LABEL("Max: "); GP.SLIDER("zakat_max", zakat_max, 3, 1023, 4); );
    );

    GP_MAKE_BLOCK_THIN_TAB(
      "Инверсия",
      GP_MAKE_BOX(GP.LABEL("Каналов реле:"); GP.SWITCH("invrs", RelayUp); );
      GP_MAKE_BOX(GP.LABEL("Канала мотора:"); GP.SWITCH("motor", VibroUp); );
    );
    
    GP_MAKE_BLOCK_THIN_TAB(
      "Конфигурация",
      GP_MAKE_BOX(GP.BUTTON("load_conf", "Load"); GP.BUTTON("save_conf", "Save"); );
    );

    GP.BUTTON("btn", "Reset ESP");
  } else {

    GP_MAKE_BLOCK_TAB(
      "Термостат",
      GP_MAKE_BOX(GP_CENTER, GP.LABEL("Температура"); GP.LED("ldt1"); GP.TEXT("temp", "DS1820"); GP.LED("ldt2"); );
      GP_MAKE_BOX(GP.LABEL("Порог:"); GP.SLIDER("p_tem", p_tem, 15, 39); );
      GP_MAKE_BOX(GP.LABEL("Гистерезис:"); GP.SLIDER("h_tem", h_tem, 0, 7); );
    );
    
    GP_MAKE_BLOCK_TAB(
      "Блок реле",
      GP_MAKE_BOX(GP.LABEL("Режим реле 1:"); GP.SELECT("sel", "Таймер 1,Температура,Ручной", valSelect); );
      GP_MAKE_BOX(GP.LABEL("Ручное управление:"); GP.SWITCH("sw", valSwitch); );
      GP_MAKE_BOX(GP.LABEL("Статус реле:");
        // создаём индикаторы статуса реле с именами rled0,rled1...
        for (int i = 0; i < relay_num; i++) {
          GP.LED(String("rled") + i, 0);
        }
      );
    );

    GP_MAKE_BLOCK_THIN_TAB(
      "Процессы",
      GP_MAKE_BOX(GP_CENTER,
        // создаём лейблы с именами led/0,led/1...
        for (int i = 0; i < 7; i++) {
          GP.LED(String("led/") + i, "#");
        }
      );
    );

  }
  GP.BREAK();
  GP.NAV_TABS_LINKS("/,/timers,/feeding,/clock,/setting", "Main,Timers,Feed,Clock,SetUp");
  
  GP.BUILD_END();
}
