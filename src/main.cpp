// почти все компоненты

#define AP_SSID "Bikar6"
#define AP_PASS "htdjkzwbz1917"

#include <GyverPortal.h>
GyverPortal portal;

bool valCheck;
bool valSwitch;
String valText = "Hello";
int valNum=123;
float valFloat=3.14;
char valPass[10];
float valSpin=3;
int valSlider=5;
GPdate valDate;
GPtime valTime;
GPcolor valCol;
int valSelect;

void build() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  //GP.THEME(GP_LIGHT);

  GP.TITLE("AquaTimer");
  GP.HR();

  GP.NAV_TABS_LINKS("/,/home,/sett,/kek", "Home,Settings,Kek");
  
  GP_MAKE_SPOILER(
    "Spoiler",
    GP.LABEL("Hello!");
  );

  GP_MAKE_BLOCK(
    GP.LABEL("Checks & LED");
    GP.BREAK();
    GP.LABEL_BLOCK("label block");
    GP.LED("");
    GP.CHECK("ch", valCheck);
    GP.SWITCH("sw", valSwitch);
  );

  GP_MAKE_BLOCK_TAB(
    "Block Tab",
    GP.LABEL("Inputs");
    GP_MAKE_BOX(GP.LABEL("Number");   GP.NUMBER("num", "number", valNum);     );
    GP_MAKE_BOX(GP.LABEL("Float");    GP.NUMBER_F("numf", "numf", valFloat);  );
    GP_MAKE_BOX(GP.LABEL("Text");     GP.TEXT("txt", "text", valText);   );
    GP_MAKE_BOX(GP.LABEL("Password"); GP.PASS("pass", "pass", valPass);    );
    GP.AREA("", 3, "Text area");
  );

  GP_MAKE_BLOCK_THIN(
    GP_MAKE_BOX(GP.LABEL("Date");   GP.DATE("date", valDate);  );
    GP_MAKE_BOX(GP.LABEL("Time");   GP.TIME("time", valTime);  );
    GP_MAKE_BOX(GP.LABEL("Color");  GP.COLOR("col", valCol); );
  );

  GP_MAKE_BLOCK_THIN_TAB(
    "Thin Tab",
    GP.LABEL("Upload File/Folder");
    GP_MAKE_BOX(
      GP_CENTER,
      GP.FILE_UPLOAD("");
      GP.FOLDER_UPLOAD("");
    );
  );

  GP_MAKE_BOX(GP.LABEL("Select");   GP.SELECT("sel", "Some,Drop,List", valSelect);  );
  GP_MAKE_BOX(GP.LABEL("Slider");   GP.SLIDER("sld", valSlider, 0, 10);  );
  GP_MAKE_BOX(GP.LABEL("Spinner");  GP.SPINNER("spn", valSpin); );

  GP.BUTTON("btn", "Button");
  // GP.BUTTON_MINI("", "Btn Mini");

  GP.BUILD_END();
}

void action() {
  // был клик по компоненту
  if (portal.click()) {
    // проверяем компоненты и обновляем переменные
    
    // 1. переписали вручную
    if (portal.click("ch")) {
      valCheck = portal.getBool("ch");
      Serial.print("Check: ");
      Serial.println(valCheck);
    }

    // 2. автоматическое обновление переменной
    if (portal.clickBool("sw", valSwitch)) {
      Serial.print("Switch: ");
      Serial.println(valSwitch);
    }

    if (portal.clickString("txt", valText)) {
      Serial.print("Text: ");
      Serial.println(valText);
    }

    if (portal.clickInt("num", valNum)) {
      Serial.print("Number: ");
      Serial.println(valNum);
    }

    if (portal.clickFloat("fnum", valFloat)) {
      Serial.print("Float: ");
      Serial.println(valFloat);
    }

    if (portal.clickStr("pass", valPass)) {
      Serial.print("Password: ");
      Serial.println(valPass);
    }

    if (portal.clickFloat("spn", valSpin)) {
      Serial.print("Spinner: ");
      Serial.println(valSpin);
    }

    if (portal.clickInt("sld", valSlider)) {
      Serial.print("Slider: ");
      Serial.println(valSlider);
    }

    if (portal.clickDate("date", valDate)) {
      Serial.print("Date: ");
      Serial.println(valDate.encode());
      Serial.println(valDate.day );
    }

    if (portal.clickTime("time", valTime)) {
      Serial.print("Time: ");
      Serial.println(valTime.encode());
      Serial.println(valTime.hour);
    }

    if (portal.clickColor("col", valCol)) {
      Serial.print("Color: ");
      Serial.println(valCol.encode());
    }

    if (portal.clickInt("sel", valSelect)) {
      Serial.print("Select: ");
      Serial.println(valSelect);
    }

    if (portal.click("btn")) Serial.println("Button click");
    if (portal.clickUp("btn")) Serial.println("Button up");
    if (portal.clickDown("btn")) Serial.println("Button down");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("WiFi start");
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  Serial.println("AP connect");
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

void loop() {
  portal.tick();
}
