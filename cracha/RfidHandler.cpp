#include "RfidHandler.h"
SoftwareSerial rdmSerial(RX,TX);
void RfidHandlerClass::init(){
    cleanSerial();
    rfidTag = "";
    rdmSerial.begin(RFID_BAUDRATE);	
}

boolean RfidHandlerClass::serialEvent(){
    boolean isMessage;
    String inputString;
    while (rdmSerial.available()){
        char inChar = (char)rdmSerial.read();
        if (inChar==STX){
            rfidTag= "";
            inputString = "";
            isMessage = true;
        }
        if(isMessage && inChar !=STX && inChar!=ETX){
            inputString += inChar;
        }
        if (inChar==ETX){
            isMessage = false;
            cleanSerial();
            if (inputString.length()>=8){
                return convertTag(inputString);       
            }
            inputString = "";
            return false;
        }
    }
    inputString = "";
    rfidTag= "";
    isMessage = false;
    return false;
}

boolean RfidHandlerClass::convertTag(String rdmString){
    char _group1[5], _group2[5];
    rdmString.substring(2,7).toCharArray(_group1, 5);
    rdmString.substring(6,10).toCharArray(_group2, 5);
    rfidTag = String(strtoul(_group1,NULL,16))+String(strtoul(_group2,NULL,16));
    cleanSerial();
    if (rfidTag.length() > 0 && rfidTag != "00"){
        return true;
    }
    rfidTag = "";
    return false;
}

void RfidHandlerClass::cleanSerial(){
    while(rdmSerial.read() >= 0){
        ;
    }
}

boolean RfidHandlerClass::readrfidTag(){
    delay(200);
    return serialEvent();
}

String 	RfidHandlerClass::getrfidTag(){
	return rfidTag;
}

RfidHandlerClass::~RfidHandlerClass(){

}

RfidHandlerClass RfidHandler;