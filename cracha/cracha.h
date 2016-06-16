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
#define VERSION             "1.0.0"
#define ESP_BAUDRATE        74880
#define CONNECTION_TIMEOUT  15000

#define SERVER_APP          "192.168.1.50"
#define APP_PORT            80
#define API_ENDPOINT        "/cracha/"

enum GPIO{
    BUZZER=2,
    LED1=5,
    LED2=4,
    LED3 =14,
    TIP=12
};

enum TIMING{
    SHORT_BUZZ=500,
    CLOCK=1,
    SLEEP_DELAY_IN_SECONDS=2,
    WAIT_CONNECT=100,
    WAIT_STATUS=1000,
    WAIT_NOTIFY=2000,
    WAIT_OPEN_ACCESS=3000,
    CONNECT_TIMEOUT=60000

};

const char *ssid = "Salcomp_sys";
const char *password = "salcomp@2016";
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
int sendTagToServer(String tag);
void sendMessage(String tag);
void playBuzzer(int duration);
void blinkLed(int led, int period);
void setPins();
void notifyOpenAccess();
void notifyBlockedAccess();
void openAccess(int duration);
void closeAccess();
boolean connect();
#endif