// глобальные переменные для работы с ними в программе
unsigned int ds_int=5; // интервал замера ds18b20 в секундах 
unsigned int mqtt_int=30; // интервал отправки данных по MQTT в секундах 
float tem; // тут храним температуру
float p_tem=26; // температура включения реле
float h_tem=1; // гистерезис
String mode = "TMR"; // режим работы по температуре/ручной ("TEM"/"MAN/TMR")
String ntp_srv="ntp6.ntp-servers.net";
int tz=2;
boolean ch1_on;
boolean ch2_on;
boolean relay_on;
String tm[2];
uint8_t YearTime=12; // Период показа даты
uint8_t YearView=5;  // Время показа даты
int led_light;
unsigned int NTP_req=30; // Период запроса NTP-сервера
boolean eff_time = true; // показ эффекта перед часами
unsigned int td = 25; // скорость эффекта перед часами
boolean mqtt_enable = false;
boolean ntp_setup=false;
boolean time_set=false;
String temp;
float led_intens;

boolean DBG = true;