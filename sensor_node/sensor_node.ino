#include "sensor_node.h"

void setPins(){
  //  ADC_MODE(ADC_VCC);
    //pinMode(SENSOR, INPUT);
}

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

int sendTagToServer(String content){
    Serial.println(content);
    //String host = SERVER_APP;
    http.begin(SERVER_APP, PORT_APP, API_ENDPOINT, false, "");
    //http.addHeader("plotly-convertTimestamp",TIMEZONE);
    http.addHeader("Content-Type", "application/json");
    //http.addHeader("plotly-streamtoken",tokens[0]);
    int httpCode = http.POST(content);
    http.end();
    Serial.println(httpCode);
    return httpCode;
}

boolean sendMessage(String data){
    if(connect()){
        Serial.println("Connected");
        Serial.println(WiFi.localIP());
        ESP8266Connect(SERVER_APP, PORT_APP);
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

String collectData(){
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Voltage battery: ");
    Serial.println(ESP.getVcc()/10);
    String result ="{\"temperature\":";
    result.concat(String(t));
    result.concat("}");
    return result;
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
    setPins();
   // digitalWrite(BUZZER,LOW);
    //playBuzzer(SHORT_BUZZ);
    Serial.println("Start");
    WiFi.printDiag(Serial);
    dht.begin();
}

void loop ( void ) {

    sendMessage(collectData());
    delay(2000);
    //ESP.deepSleep(2 * 1000000, WAKE_RF_DEFAULT);
}