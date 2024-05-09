#include <Arduino.h>
#include <WiFi.h>
#include <Firebase.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <ArduinoJson.h>
#include "db-data.h"

// #define WIFI_SSID "TT_91A8"
// #define WIFI_PASSWORD "imcnlqw733"
#define WIFI_SSID "OPPO A54"
#define WIFI_PASSWORD "1234567890"
// #define WIFI_SSID "OPPO A78"
// #define WIFI_PASSWORD "111111111"
#define PROJECT_API_KEY "AIzaSyBcEzCBBWwtkvcBlmrWu3cNl7MHOF1lYz0"
#define DB_URL "https://projet-pfe-68088-default-rtdb.firebaseio.com/"




#define TX_PIN 18  
#define RX_PIN 19 



FirebaseData fbdo ;
FirebaseAuth auth ;
FirebaseConfig config ;



ArrayInfo fetchedArray ;
Variables vars ;




 
  
 
void setup() {
  
 
 Serial.begin(115200);
 Serial2.begin(115200, SERIAL_8N1, TX_PIN, RX_PIN); 

 WiFi.begin(WIFI_SSID ,WIFI_PASSWORD ); 
 Serial.print("Connecting To Wifi"); 
 while ( WiFi.status() != WL_CONNECTED)
 {
  Serial.print(".");
  delay(300);
 }
 Serial.println();
 Serial.print("Connected To IP :") ;
 Serial.println(WiFi.localIP()) ;
 Serial.println();

config.api_key = PROJECT_API_KEY ;
config.database_url = DB_URL ; 

if (Firebase.signUp(&config , &auth , "" , ""))
{
  Serial.println("Signed Up");
  vars.SignUp = true ; 
}else {
  Serial.printf("%s\n" , config.signer.signupError.message.c_str());
}
config.token_status_callback = tokenStatusCallback;
Firebase.begin(&config , &auth);
Firebase.reconnectWiFi(true);
Serial.println("UART Sender/Receiver 2 Initialized");
 
}


void loop() {
  bool updated = getupdated(fbdo);
 delay(3000);
if (Firebase.ready() && vars.SignUp && (millis() - vars.sendDataPrevMillis > 1000 || vars.sendDataPrevMillis == 0)) {
    vars.sendDataPrevMillis = millis();
  if (updated)
  {
 fetchedArray = getArray(fbdo);
 vars.mode = getMode(fbdo);
 
 vars.fetched = true ;
  } 
  


}
recieveLocalMessage();



if (updated &&  vars.fetched )
{
  sendLocalMessage(vars.mode );
  if (vars.fetched ) {
  Firebase.RTDB.setBool(&fbdo , "options/updated" , false );
  
}
} else {
  Serial.println("Not updated");
}



    



 }









