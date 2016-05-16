#ifndef _RFIDHANDLER_h
#define _RFIDHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SoftwareSerial.h>
#include <stdlib.h>

#define RFID_BAUDRATE 	9600
#define MAX_RFIDMSG		12

#define STX             0x02    // Start of text control code
#define ETX             0x03    // End of text control code
#define RX 				13
#define TX 				15

class RfidHandlerClass
{
protected:
	String rfidTag; 	
	boolean convertTag(String rdmString);

public:
	~RfidHandlerClass();
	void init();
	void cleanSerial();
	boolean serialEvent();
	boolean readrfidTag();
	String getrfidTag();


};

extern RfidHandlerClass RfidHandler;
#endif //_RFIDHANDLER_h