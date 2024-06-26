#ifndef MESSAGE_H
#define MESSAGE_H

#include <Arduino.h>
#include <Firebase.h>
#include <ArduinoJson.h>

//  status ,
struct Variables{
    int  localarraySize , mode ;
    bool SignUp , fetched;
    unsigned long sendDataPrevMillis;
};
struct ArrayInfo {
    int* arrayPtr;  
    size_t arraySize;
    bool updated;
   
};


ArrayInfo getArray(FirebaseData &fbdo);
void sendLocalMessage( int mode );
String recieveLocalMessage(DynamicJsonDocument& receivedDoc );
int getMode(FirebaseData &fbdo);
int getStatus(FirebaseData &fbdo);
bool getupdated(FirebaseData &fbdo) ;
void saveToFirestore( DynamicJsonDocument& receivedDoc , FirebaseData &fbdo);


#endif
