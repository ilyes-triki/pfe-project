#ifndef MESSAGE_H
#define MESSAGE_H

#include <ArduinoJson.h>
#include "painlessMesh.h"

struct MessageArguments {
	constexpr static int ldrpin = 34;
	constexpr static int ldrLampe = 32;

	constexpr static int led = 12;
	constexpr static int boardnum = 2;
	
	int mode;
	String jsonRec;
	String jsonBrod;
};


void testMessage(  DynamicJsonDocument& receivedDoc );
void testModes ( DynamicJsonDocument& receivedDoc ) ;

#endif