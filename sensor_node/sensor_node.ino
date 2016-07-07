#include "sensor_node.h"
boolean connect(){
    if(WiFi.status() != WL_CONNECTED ){
        WiFi.begin(ssid, password);
        unsigned long start = millis();
        while((millis() - start) <TIMING::CONNECT_TIMEOUT){
            if( WiFi.status() == WL_CONNECTED ) {
                return true;
            }
            delay(TIMING::WAIT_CONNECT);
        }
        return false;
    }
    return true;
}

String prepareTagContent(String tag){

    String message = "{\"x\":\"";
    message.concat(String(millis()));
    message.concat("\", \"y\":\""+tag+"\"}");
    //message.concat("\"}");
    return message;
}

int sendTagToServer(int tag){
    String content = prepareTagContent(String(tag));
    Serial.println(content);
    String host = SERVER_APP;
    http.begin(SERVER_APP, PORT_APP, API_ENDPOINT, false, "");
    http.addHeader("plotly-convertTimestamp","America/Manaus");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("plotly-streamtoken",tokens[0]);
    int httpCode = http.POST(content);
    http.end();
    Serial.println(httpCode);
    return httpCode;
}

boolean sendMessage(int data){
    if(connect()){
        Serial.println("Connected");
        Serial.println(WiFi.localIP());
        ESP8266Connect(SERVER_APP, 80);
        switch(sendTagToServer(data)){
            case HTTP_OK:
            return true;
            break;
            case HTTP_NOT_AUTHORIZED:
            return false;
            //notifyBlockedAccess();
            break;
            case HTTP_NOT_RESPONSE:
            return false;
            //notifyServerNotResponding();
        }
    }

}

int collectData(){
    return 0;
}

void ESP8266Connect(char* url, int port){
    Serial.print("Connecting to: \"");
    Serial.print(url);
    Serial.println("\" server...");
    if (!client.connect(url, port)) {
        Serial.println("*Connection Failed!*\r\n");
    }
    else{
        Serial.println("Connected!");
    }
}

void setup( void ) {
    Serial.swap();
    Serial.begin(ESP_BAUDRATE);
    //rfidHandler.init();
    //setPins();
    digitalWrite(BUZZER,LOW);
    //playBuzzer(SHORT_BUZZ);
    Serial.println("Start");
}

void loop ( void ) {
    sendMessage(collectData());
    ESP.deepSleep(2 * 1000000, WAKE_RF_DEFAULT);
}