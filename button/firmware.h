#ifndef _FIRMWARE_H
#define _FIRMWARE_H
/*
Dash button - By Lauro Gama
10/05/2016
*/

#include <ESP8266WiFi.h>
//#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>

#define VERSION             "0.0.1"
#define ESP_BAUDRATE        19200
#define CONNECTION_TIMEOUT  15000

#define SERVER_APP          "192.168.1.116"
#define APP_PORT            8000
#define USERBUTTON          D0             
#define API_ENDPOINT        "/click/"


enum TIMING{
    CLOCK=1,
    SLEEP_DELAY_IN_SECONDS=30,
    WAIT_CONNECT=100,
    WAIT_STATUS=1000,
    WAIT_NOTIFY=2000,
    CONNECT_TIMEOUT=60000

};

const char *ssid = "inventus2";
const char *password = "salcsysqsxrt";
HTTPClient http;

typedef enum {
    HTTP_OK = 200,
    HTTP_NOT_AUTHORIZED=401,
    HTTP_NOT_FOUND=404,
    HTTP_INTERNAL_ERROR=500

}http_codes;

typedef enum {
    CONNECT = 1,
}MessageType;

void turnOff(int pin);
String prepareContent(MessageType messageType);
String prepareConnectContent();
void manageConnection();
boolean connect();
boolean notifyAppServer();
#endif