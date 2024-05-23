#ifndef MESSAGE_H
#define MESSAGE_H

#include <ArduinoJson.h>
#include "painlessMesh.h"

struct MessageArguments {
	constexpr static int ldrpin = 34;
	constexpr static int ldrpinLampe = 32;
	constexpr static int led = 12;
	constexpr static int boardnum = 1;
	bool working = true;
	int mode;
	String jsonRec;
	String jsonBrod , jsonBrodError ;
};

String checkIfWorking ( JsonDocument& brodDoc ) ;
void addArrayToMessage(  JsonDocument& receivedDoc );
void testModes ( JsonDocument& receivedDoc ) ;

#endif