// почти все компоненты

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GyverPortal portal;

void build() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  //GP.THEME(GP_LIGHT);

  GP.TITLE("Mobile App Style");
  GP.HR();

  GP_MAKE_BLOCK(
    GP_MAKE_BOX(GP.LABEL("Some check 1"); GP.CHECK("");   );
    GP_MAKE_BOX(GP.LABEL("Some check 2"); GP.CHECK("");   );
    GP_MAKE_BOX(GP.LABEL("Some Switch 1"); GP.SWITCH(""); );
    GP_MAKE_BOX(GP.LABEL("Some Switch 2"); GP.SWITCH(""); );
  );

  GP_MAKE_BLOCK(
    GP_MAKE_BOX(GP.LABEL("SSID");     GP.TEXT("");  );
    GP_MAKE_BOX(GP.LABEL("Password"); GP.TEXT("");  );
    GP_MAKE_BOX(GP.LABEL("Host");     GP.TEXT("");  );
  );

  GP_MAKE_BLOCK(
    GP_MAKE_BOX(GP.LABEL("Upload settings");  GP.FILE_UPLOAD(""); );
    GP_MAKE_BOX(GP.LABEL("Upload photo");     GP.FILE_UPLOAD(""); );
    GP_MAKE_BOX(GP.LABEL("Upload data");      GP.FILE_UPLOAD(""); );
  );

  GP_MAKE_BLOCK(
    GP_MAKE_BOX(GP.LABEL("Color 1");  GP.SLIDER("");  );
    GP_MAKE_BOX(GP.LABEL("Color 2");  GP.SLIDER("");  );
    GP_MAKE_BOX(GP.LABEL("Color 3");  GP.SLIDER("");  );
  );

  GP.BUILD_END();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  // подключаем конструктор и запускаем
  portal.attachBuild(build);
  portal.attach(action);
  portal.start();
}

void action() {
}

void loop() {
  portal.tick();
}
