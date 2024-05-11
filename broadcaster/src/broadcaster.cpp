#include "painlessMesh.h"

#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>



// Serial2 pins of ESP32
#define TX_PIN 19  // GPIO16 (TX2)
#define RX_PIN 18  // GPIO17 (RX2)

// WiFi Credentials for Mesh Networking
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555


// Variables




 

int nodesReceived = 0;
DynamicJsonDocument receivedDoc(1024) , receivedDocRec(1024) ;
String recievedMessage , receivedDocRecString , prevMessage = "" ;
Scheduler userScheduler; 
painlessMesh  mesh;


 
void sendMessage() ; 


Task taskSendMessage( TASK_SECOND * 2 , TASK_FOREVER, &sendMessage );




void sendMessage()
{
   if ((nodesReceived != mesh.getNodeList().size() ||  mesh.getNodeList().empty()) && (recievedMessage != "" && recievedMessage != prevMessage) ) {
              mesh.sendBroadcast(recievedMessage);
 Serial.print("Sending messega - "); Serial.println(recievedMessage);
        }else {
          prevMessage = recievedMessage ; 
          Serial.println("All nodes have received the message.");
        }
 
  
  
 
}
 
  



void receivedCallback( uint32_t from, String &msg )
{
 if (msg == "ACK") { 
        nodesReceived++;
    }else {receivedDocRecString = msg.c_str();
  DeserializationError error = deserializeJson(receivedDocRec, receivedDocRecString);
  Serial.print("Mesh Reciever - "); Serial.println(receivedDocRecString);
   mesh.sendSingle(from, "ACK");}
    

}




void newConnectionCallback(uint32_t nodeId) {
 
}

void changedConnectionCallback() {
 
}

void nodeTimeAdjustedCallback(int32_t offset) {
  
}

void setup() {
  Serial.begin(115200);  // For Debugging purpose
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
     
      mesh.update();
   
   

  
  
  delay(3000);
  
}




// update condition 

  // if ( recievedMessage != "" && !recievedMessage.equals(prevMessage))
  // {
  // 
  //  {
  //  prevMessage = recievedMessage ;
  //     mesh.update();
  //    recievedMessage = "";
  //  }
   
   

 

  // }
  // else {
  //   Serial.println("No message to broadcast");
  // }
  