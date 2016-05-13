 #include "firmware.h"

boolean notifyAppServer(MessageType message){
    String content = prepareContent(message);
    Serial.println(content);
    String host = SERVER_APP;
    http.begin(host.c_str(), APP_PORT, API_ENDPOINT, false, "");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(content);
    http.end();
    Serial.println(httpCode);
    return httpCode==http_codes::HTTP_OK;
}

void manageConnection(){
    if(WiFi.status()==WL_DISCONNECTED){
        connect();
    }
}

void turnOff(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 1);
}

String prepareContent(MessageType messageType){
    switch(messageType){
        case MessageType::CONNECT:
        return prepareConnectContent();
        break;
    }
}

String prepareConnectContent(){
    String message = "{\"mac\":\"";
    message.concat(WiFi.macAddress());
    message.concat("\"}");
    //message.concat("\",\"ip\":\"");
    //message.concat(WiFi.localIP().toString());
    //message.concat("\"}");
    return message;
}

boolean connect(){
    if(WiFi.status() != WL_CONNECTED ){
        WiFi.begin(ssid, password);
        unsigned long start = millis();
        while((millis()- start) <TIMING::CONNECT_TIMEOUT){
            if( WiFi.status() == WL_CONNECTED ) {
                return true;
            }
            delay(TIMING::WAIT_CONNECT);
        }
        return false;
    }
    return true;
}

void setup( void ) {
    Serial.begin(ESP_BAUDRATE);
// disable all output to save power
    turnOff(0);
    turnOff(2);
    turnOff(4);
    turnOff(5);
    turnOff(12);
    turnOff(13);
    turnOff(14);
    turnOff(15);
    pinMode(USERBUTTON, INPUT);
    //attachInterrupt(digitalPinToInterrupt(BUTTON), buttonHandler, CHANGE );
    Serial.println("Started");    
}

void loop ( void ) {
    if(connect()){
        Serial.println("Connected");
        if(notifyAppServer(MessageType::CONNECT)){
            Serial.println("Message Sent");
        }
    }
    //system_deep_sleep_set_option(0);
    //ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
    delay(100);
    //manageConnection();
    
}