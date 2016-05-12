 #include "firmware.h"

boolean notifyAppServer(Message message){
    String content = prepareContent();
    String host =SERVER_APP;
    http.begin(host.c_str(), APP_PORT, API_ENDPOINT, false, "");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(content);
    http.end();
    return httpCode==http_codes::HTTP_OK;
}

void manageConnection(){
    if(WiFi.status()==WL_DISCONNECTED){
        connect();
    }
}

String connectContent()
String prepareConnectContent(){
    String message = "{\"mac\":\"";
    message.concat(WiFi.macAddress());
    message.concat("\",\"ip\":\"");
    message.concat(WiFi.localIP().toString());
    message.concat("\"}");
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
    pinMode(GPIO5, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(BUTTON), buttonHandler, CHANGE );
    notifyAppServer(Message::CONNECT);
}

void loop ( void ) {
    //manageConnection();

}