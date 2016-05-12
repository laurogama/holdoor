#ifndef _FIRMWARE_H
#define _FIRMWARE_H
#include <ESP8266WiFi.h>
//#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>

#define VERSION             "0.0.1"
#define ESP_BAUDRATE        19200
#define CONNECTION_TIMEOUT  15000

#define SERVER_APP          "esb-esdcontrol.fpftech.dev"
#define APP_PORT            11223
#define BUTTON              1             
#define API_ENDPOINT        "/button"
enum TIMING{
    CLOCK=1,
    WAIT_CONNECT=100,
    WAIT_STATUS=1000,
    WAIT_NOTIFY=2000
};

const char *ssid = "inventus";
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
}Message;

void manageConnection();
boolean connect();
boolean notifyAppServer();
#endif