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


std::list<uint32_t> connectedNodesList , messageSenderNodes;  // List to store connected node IDs


 


DynamicJsonDocument receivedDoc(1024) , receivedDocRec(1024) ,  brodcast(1024);
String recievedMessage ,  prevMessage , recivedFromRec , sendedToReciever;

   
Scheduler userScheduler; 
painlessMesh  mesh;


 
void sendMessage() ; 


Task taskSendMessage( TASK_SECOND * 2 , TASK_FOREVER, &sendMessage );


void nodesConnected() {

  // Update the list of connected nodes
  auto nodeList = mesh.getNodeList();
  connectedNodesList.assign(nodeList.begin(), nodeList.end());

  // Print the list of connected nodes
  Serial.println("Connected Nodes:");
  for (auto it = connectedNodesList.begin(); it != connectedNodesList.end(); ++it) {
    Serial.println(*it);
  }
  Serial.println("Nodes that sent a message:");
  for (auto it = messageSenderNodes.begin(); it != messageSenderNodes.end(); ++it) {
    Serial.println(*it);
  }
}

void sendMessage()
{
  
  brodcast["data"] = recievedMessage;
 
  
}


void receivedCallback( uint32_t from, String &msg )
{
   
 messageSenderNodes.push_back(from);

    }




void newConnectionCallback(uint32_t nodeId) {
 
}

void changedConnectionCallback() {
  //Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
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
     
   recievedMessage = Serial2.readString().c_str();  
  DeserializationError error = deserializeJson(receivedDoc, recievedMessage);
   Serial.print("board 1 Reciever - "); Serial.println(recievedMessage);
      
  }
  nodesConnected();
   mesh.update();
  
  
  delay(2000);
  
}




// update condition 

  // if ( recievedMessage != "" && !recievedMessage.equals(prevMessage) && !message_received)
  // {
  //  if (message_received)
  //  {
  //  prevMessage = recievedMessage ;
  //  message_received = false ; 
  //    recievedMessage = "";
  //  }
   
   
  //   mesh.update();
 

  // }
  // else {
  //   Serial.println("No message to broadcast");
  // }
  