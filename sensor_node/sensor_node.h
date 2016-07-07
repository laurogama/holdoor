#ifndef _SENSOR_NODE_H 
#define _SENSOR_NODE_H
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "nodemcu_defaults.h"

#define VERSION             "1.1.0"
#define ESP_BAUDRATE        74880

#define userName   "laurogama"
#define APIKey     "duovq5z1jp"
#define fileName   "test"
#define fileopt    "overwrite"
#define nTraces    1
#define maxpoints  "30"
#define world_readable true
#define convertTimestamp true
#define timezone    "Australia/Melbourne"
#define SERVER_APP  "arduino.plot.ly"
#define PORT_APP    80
#define API_ENDPOINT "/clientresp"
char *tokens[nTraces] = {"u0oi8f2ho4"};
char stream_site[25] = {0};
WiFiClient client;


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
void ESP8266Connect(char* url, int port);
#endif