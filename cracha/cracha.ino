 #include "cracha.h"

boolean sendTagToServer(String tag){
    String content = prepareTagContent(tag);
    Serial.println(content);
    String host = SERVER_APP;
    http.begin(host.c_str(), APP_PORT, API_ENDPOINT, false, "");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(content);
    http.end();
    Serial.println(httpCode);
    return httpCode==http_codes::HTTP_OK;
}

String prepareTagContent(String tag){
    String message = "{\"mac\":\"";
    message.concat(WiFi.macAddress());
    message.concat("\", \"rfid\":\""+tag+"\"}");
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

void sendMessage(String tag){
    if(connect()){
        Serial.println("Connected");
        if(sendTagToServer(tag)){
            Serial.println("Message Sent");
        }
    }
}

void setup( void ) {
    Serial.swap();
    Serial.begin(ESP_BAUDRATE);
    rfidHandler.init();
    pinMode(BUZZER,OUTPUT);
    digitalWrite(BUZZER,LOW);
    Serial.println("Start");
}

void playBuzzer(int duration){
    digitalWrite(BUZZER, HIGH);
    delay(duration);
    digitalWrite(BUZZER, LOW);
}

void loop ( void ) {

    if(rfidHandler.readrfidTag()){
        playBuzzer(TIMING::SHORT_BUZZ);
        sendMessage(rfidHandler.getrfidTag());
    }
    //Serial.print("Reading: ");
    Serial.println(rfidHandler.getrfidTag());
    delay(100);
}