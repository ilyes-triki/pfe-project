#include <Arduino.h>
#include <Firebase.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <ArduinoJson.h>

  #define WIFI_SSID "TT_91A8"
  #define WIFI_PASSWORD "imcnlqw733"
// #define WIFI_SSID "OPPO A54"
// #define WIFI_PASSWORD "1234567890"
// #define WIFI_SSID "OPPO A78"
// #define WIFI_PASSWORD "111111111"
#define PROJECT_API_KEY "AIzaSyBcEzCBBWwtkvcBlmrWu3cNl7MHOF1lYz0"
#define DB_URL "https://projet-pfe-68088-default-rtdb.firebaseio.com/"




#define TX_PIN 18  // GPIO17 (TX1)
#define RX_PIN 19 // GPIO16 (RX1)
#define led 12
#define led1 14
#define ldr 35

FirebaseData fbdo ;
FirebaseAuth auth ;
FirebaseConfig config ;
int board;
int pin;
int pin_status;
String message = "";
bool messageReady = false;



bool SignUp = false ; 
unsigned long sendDataPrevMillis = 0 ;
int ldrvalue = 0 ;
float voltage = 0 ;


int arraySize ;
int localArray[200] ;




 
  
 
void setup() {
  pinMode(led,OUTPUT);
  pinMode(led1,OUTPUT);
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
  SignUp = true ; 
}else {
  Serial.printf("%s\n" , config.signer.signupError.message.c_str());
}
config.token_status_callback = tokenStatusCallback;
Firebase.begin(&config , &auth);
Firebase.reconnectWiFi(true);
Serial.println("UART Sender/Receiver 2 Initialized");
 
}


void loop() {
 
if (Firebase.ready() && SignUp && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
if (Firebase.RTDB.getArray(&fbdo, "/options/boards/"))

{
        FirebaseJsonArray &arr = fbdo.jsonArray();
        arraySize = arr.size();
        for (size_t i = 0; i < arr.size(); i++)
        {FirebaseJsonData jsonData;
            arr.get(jsonData, i);
               localArray[i] = jsonData.intValue;
                Serial.println(localArray[i]); 
        }
       
  
  }
}
if (Serial2.available() > 0) {
    String message = Serial2.readString();  // Read data from Serial2 (RX1)
    Serial.println("Received message: " + message);
  }
  DynamicJsonDocument doc(1024);
 JsonArray board_status = doc.createNestedArray("board_status");
for (int i = 0; i < arraySize; i++)
 {
     board_status.add(localArray[i]);
 }
 
  String msg ;
  serializeJson(doc , msg) ;
  Serial.println(msg);
  Serial.println("aaray size :");

  Serial.println(arraySize);

  Serial2.println(msg);
delay(1000);
 }







//   if (Firebase.RTDB.getBool(&fbdo , "capteur/monotone" )){ 
// if (fbdo.dataType() == "boolean" && fbdo.boolData()==false){
  
//   Serial.println("led on");
  
//   }else {
//     Serial.println("led off");


//   }
//   }



//  for (int i = 0; i < arraySize; i++) {
//     Serial.print("Value at index ");
//     Serial.print(i);
//     Serial.print(": ");
//     Serial.println(localArray[i]);
// }



  
 










//   ldr data
// ldrvalue = analogRead(ldr);
//    voltage = (float)analogReadMilliVolts(ldr)/1000;





   // save data

// if (Firebase.RTDB.getBool(&fbdo , "capteur/monotone" ))
//    {
// if (fbdo.dataType() == "boolean" && fbdo.boolData()==true){

// if (Firebase.RTDB.setInt(&fbdo , "capteur/ldrValue" , ldrvalue)){
// Serial.println(ldrvalue) ; 
// Serial.println (" ldrValue Succesfully saved" );
// Serial.println(fbdo.dataType());
//   }if (Firebase.RTDB.setInt(&fbdo , "capteur/ldrVoltage" , voltage)){
// Serial.println(voltage) ; 
// Serial.println ("ldrVoltage Succesfully saved ");
// Serial.println(fbdo.dataType());
//   }
//    else {
//     Serial.println("failed");
//    }
//    if (Firebase.RTDB.getInt(&fbdo , "capteur/ldrValue" ))
//    {
//     if (fbdo.dataType() == "int" && fbdo.intData()>1000)
//     {
//       digitalWrite(led , HIGH);
      
//     }else {
//     digitalWrite(led , LOW);
      
//     }
// }
  
//   }


// } 
//   if (Firebase.RTDB.getBool(&fbdo , "capteur/monotone" )){ 
// if (fbdo.dataType() == "boolean" && fbdo.boolData()==false){
  
//   digitalWrite(led1 , HIGH);
//   digitalWrite(led , LOW);
  
//   }else {
//   digitalWrite(led1 , LOW);

//   }
//   }



//
   

//   }





//   for (int i = 0; i < arraySize; i++) {
//     String path = "/options/boards/" + String(i);
//     int element = localArray[i]  ;
// if (element != 0 )
// {
//   if ( Firebase.RTDB.setInt(&fbdo , path ,element ))
//    {
//     Serial.println(element) ; 
//    }
// }else {
//   break;
// }
//  }
  
//   for (int i = 0; i < arraySize; i++) {
//     Serial.print("Value at index ");
//     Serial.print(i);
//     Serial.print(": ");
//     Serial.println(localArray[i]);
//   }
// }