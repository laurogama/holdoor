#ifndef _NODEMCU_DEFAULTS_H
#define _NODEMCU_DEFAULTS_H

#define ssid       "Salcomp_sys"
#define password   "salcomp@2016"

enum GPIO{
    BUZZER=16,
    LED1=14,
    LED2=4,
    BUTTON = 12,
    TIP=5
};
typedef enum {
    HTTP_NOT_RESPONSE = -1,
    HTTP_OK = 200,
    HTTP_NOT_AUTHORIZED=401,
    HTTP_NOT_FOUND=404,
    HTTP_INTERNAL_ERROR=500

}http_codes;
HTTPClient http;

#endif