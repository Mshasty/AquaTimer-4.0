void relayState(int pin, boolean state, String id){
    static boolean oldState = false;
    if(state != oldState){
        digitalWrite(pin, state);
        //if(id != "") jee.var(id, state? "true" : "false");
        oldState = state;
    } 
}

void relay(){
  static unsigned long i;
  static boolean st = false;
  if(!st){
    st = true;
    pinMode(RELAY, OUTPUT);
  }
  if(i + 1000 > millis()) return;
  i = millis();
  tem = ds_tem;
  
  if (tem > p_tem + ( h_tem / 2)){
    if (mode == "TEM") relayState(RELAY, true, "relay");
    tempOK = true;
  }
  if (tem < p_tem - ( h_tem / 2)){
    if (mode == "TEM") relayState(RELAY, false, "relay");
    tempOK = false;
  }
  
  if (mode == "MAN") {
    relayState(RELAY, relay_on, "relay");
  }
  if (mode == "TMR") {
    
  }
}

