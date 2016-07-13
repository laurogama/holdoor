 #include "cracha.h"

 int sendTagToServer(String tag){
    String content = prepareTagContent(tag);
    Serial.println(content);
    String host = SERVER_APP;
    http.begin(host.c_str(), APP_PORT, API_ENDPOINT, false, "");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(content);
    http.end();
    Serial.println(httpCode);
    return httpCode;
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
        while((millis() - start) <TIMING::CONNECT_TIMEOUT){
            if( WiFi.status() == WL_CONNECTED ) {
                Serial.println(WiFi.localIP());
                return true;
            }
            delay(TIMING::WAIT_CONNECT);
        }
        return false;
    }
    Serial.println(WiFi.localIP());
    return true;
}

void notifyOpenAccess(){
    digitalWrite(LED1, LOW);
    digitalWrite(TIP,LOW);
    Serial.println("Access OK");
    blinkLed(LED2, WAIT_OPEN_ACCESS);
    //openAccess(WAIT_OPEN_ACCESS);
}

void notifyBlockedAccess(){
    playBuzzer(SHORT_BUZZ);
    Serial.println("Access Unauthorized");
}

void notifyServerNotResponding(){
    digitalWrite(LED1, LOW);
    playBuzzer(SHORT_BUZZ);
    digitalWrite(TIP,LOW);
    delay(WAIT_OPEN_ACCESS);
    Serial.println("Server Not responding");
}

void openAccess(int duration){
    digitalWrite(TIP,LOW);
    //delay(duration);
    //digitalWrite(TIP,LOW);
}

void closeAccess(){
    digitalWrite(TIP,HIGH);
}

void sendMessage(String tag){
    if(connect()){
        Serial.println("Connected");
        switch(sendTagToServer(tag)){
            case HTTP_OK:
            notifyOpenAccess();
            break;
            case HTTP_NOT_AUTHORIZED:
            notifyBlockedAccess();
            break;
            case HTTP_NOT_RESPONSE:
            notifyServerNotResponding();
            default:
            notifyOpenAccess();
        }
    }

}

void setPins(){
    pinMode(BUZZER,OUTPUT);
    pinMode(TIP, OUTPUT);
    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
    pinMode(BUTTON, INPUT);
}

void playBuzzer(int duration){
    digitalWrite(BUZZER, HIGH);
    delay(duration);
    digitalWrite(BUZZER, LOW);
}

void blinkLed(int led, int period){
    digitalWrite(led,HIGH); 
    delay(period);
    digitalWrite(led,LOW);
}
boolean checkButton(){
    boolean result = false;
    if (digitalRead(BUTTON) == HIGH && lastButtonState == LOW){
        Serial.println("pressed");
        delay(1);
        result = true;
    }
    lastButtonState= digitalRead(BUTTON);
    return result;
}

void setup( void ) {
    Serial.swap();
    Serial.begin(ESP_BAUDRATE);
    rfidHandler.init();
    setPins();
    digitalWrite(BUZZER,LOW);
    playBuzzer(SHORT_BUZZ);
    Serial.println("Start");
}

void loop ( void ) {
    if(checkButton()){
        notifyOpenAccess();
    }
    if(rfidHandler.readrfidTag()){
        playBuzzer(TIMING::SHORT_BUZZ);
        if(rfidHandler.getrfidTag().indexOf(MASTER_TAG)!=-1){
            notifyOpenAccess();
            }else{
                sendMessage(rfidHandler.getrfidTag());
            }
        }
        rfidHandler.cleanSerial();
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        digitalWrite(TIP,HIGH);
        delay(100);
    }