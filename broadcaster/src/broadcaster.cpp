#include "painlessMesh.h"

#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "send.h"




// Serial2 pins of ESP32
#define TX_PIN 19  // GPIO16 (TX2)
#define RX_PIN 18  // GPIO17 (RX2)

// WiFi Credentials for Mesh Networking
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555


// Variables




 

int nodesReceived = 0;
DynamicJsonDocument receivedDoc(1024) , receivedDocRec(1024) , doc(1024) ;
String recievedMessage , msg , receivedDocRecString   ;
Scheduler userScheduler; 
painlessMesh  mesh;
 



 
void sendMessage() ; 


Task taskSendMessage( TASK_SECOND * 2 , TASK_FOREVER, &sendMessage );




void sendMessage()
{

  
   if ((nodesReceived < 1 && !recievedMessage.isEmpty()) ) {
              mesh.sendSingle(532235501 , recievedMessage);
 Serial.print("Sending message - "); Serial.println(recievedMessage);
 Serial.print("nodes recieved - "); Serial.println(nodesReceived);

        }else {
          Serial.println("brodcasting stopped.");
        }


 
}



void receivedCallback( uint32_t from, String &msg )
{
 if (msg == "ACKFROMNEXT") { 
        nodesReceived++;
    }else if (from == 532235501 )
    {
       receivedDocRecString = msg.c_str();
            DeserializationError error = deserializeJson(receivedDocRec, receivedDocRecString);
            Serial.print("Mesh Receiver - ");
            Serial.println(receivedDocRecString);
   mesh.sendSingle(from, "ACKFROMPREV");}
    }
   




void newConnectionCallback(uint32_t nodeId) {
  Serial.print("reciever 1 id - "); Serial.println(nodeId);
 
}

void changedConnectionCallback() {
 
}

void nodeTimeAdjustedCallback(int32_t offset) {
  
}

void setup() {
  Serial.begin(115200);  
   Serial2.begin(115200, SERIAL_8N1, TX_PIN, RX_PIN); 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  
  taskSendMessage.enable();
 Serial.println("UART Sender/Receiver 1 Initialized");
  
}

void loop()
{

    if (Serial2.available() > 0) {
        nodesReceived = 0;
   recievedMessage = Serial2.readString().c_str();  
  DeserializationError error = deserializeJson(receivedDoc, recievedMessage);
   Serial.print("board 1 Reciever - "); Serial.println(recievedMessage);
  }
     if (!receivedDocRecString.isEmpty() )
   {

 if (receivedDocRec.containsKey("boards_info")) {
        doc["boards_info"] = receivedDocRec["boards_info"];
        serializeJson(doc , msg) ;
  Serial2.println(msg);
  Serial.println("sending message thru URAT");
  Serial.println(msg);
    }

   }
      mesh.update();
   
   
   

  
  
  delay(1500);
  
}




