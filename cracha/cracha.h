#ifndef _CRACHA_H
#define _CRACHA_H
/*
Leitor RFID wifi - By Lauro Gama
10/05/2016
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "RfidHandler.h"

extern "C" {
  #include "user_interface.h"
}
#define VERSION             "0.0.1"
#define ESP_BAUDRATE        74880
#define CONNECTION_TIMEOUT  15000
#define BUZZER              2

#define SERVER_APP          "192.168.1.116"
#define APP_PORT            8000
#define API_ENDPOINT        "/cracha/"


enum TIMING{
    SHORT_BUZZ=500,
    CLOCK=1,
    SLEEP_DELAY_IN_SECONDS=2,
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

RfidHandlerClass rfidHandler;
String prepareTagContent(String tag);
boolean sendTagToServer(String tag);
void sendMessage(String tag);
void playBuzzer(int duration);
boolean connect();
#endif