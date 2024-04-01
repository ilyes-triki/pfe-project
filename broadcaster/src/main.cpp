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

int led_status = 1;
bool message_ready = true;
bool message_received = false;
String mode = "specific";
String message = "";
String json;

DynamicJsonDocument receivedDoc(1024);
String recievedMessage ; 
Scheduler userScheduler; 
painlessMesh  mesh;


// User stub
void sendMessage() ; 


Task taskSendMessage( TASK_SECOND * 20 , TASK_FOREVER, &sendMessage );


void sendMessage()
{
  DynamicJsonDocument doc(1024);
  doc["mode"] = mode ;
  doc["status"] =  led_status;
 JsonArray board_status = doc.createNestedArray("board_status");
  // board_status.add(1);
  board_status.add(2);
  String msg ;
  serializeJson(doc, msg);
  mesh.sendBroadcast( msg );
  Serial.print("Mesh Broadcast - "); Serial.println(msg);
 taskSendMessage.setInterval((TASK_SECOND * 20));
}


void receivedCallback( uint32_t from, String &msg )
{
  
  
    }




void newConnectionCallback(uint32_t nodeId) {
  //Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
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
  // timer.setInterval(1000L, send_request);
}

void loop()
{

    if (Serial2.available() > 0) {
   recievedMessage = Serial2.readString().c_str();  
  DeserializationError error = deserializeJson(receivedDoc, recievedMessage);
   Serial.print("board 1 Reciever - "); Serial.println(recievedMessage);
  }

  Serial2.println("Hello from ESP32-1");  
  
  
  mesh.update();
  delay(1000);
  //timer.run();
}