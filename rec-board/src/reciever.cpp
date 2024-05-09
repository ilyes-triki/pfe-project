#include "painlessMesh.h"
#include <ArduinoJson.h>
#include "message.h"



// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555







MessageArguments arguments;
DynamicJsonDocument receivedDoc(1024);
Scheduler userScheduler; 
painlessMesh  mesh;
int ldrLmapeValue = analogRead(arguments.ldrpinLampe);


void sendMessage() ; 

Task taskSendMessage( TASK_SECOND * 3, TASK_FOREVER, &sendMessage );

void sendMessage()
{
  
 


}


void receivedCallback( uint32_t from, String &msg )
{
  
      
   
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
  

  Serial.begin(115200);

  pinMode(arguments.led, OUTPUT);

  digitalWrite(arguments.led,LOW);

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
// testModes ( receivedDoc ) ; 
delay(3000);
    }




// recievecall back 



// arguments.jsonRec = msg.c_str();
  // DeserializationError error = deserializeJson(receivedDoc, arguments.jsonRec);
   

  // Serial.print("Mesh Reciever - "); Serial.println(arguments.jsonRec);
  // if (error)
  // {
  //   Serial.print("deserializeJson() failed: ");
  //   Serial.println(error.c_str());
  // }



  // send 

//   if (receivedDoc.isNull() ) {
  //     arguments.recieved = false ;
  //       Serial.println("No stored message to broadcast.");
  //       return;
  //   }
  //   arguments.recieved = true ;
  //   receivedDoc["message_received"] = arguments.recieved ;
  // if (ldrLmapeValue > 100)
  // {
  //  addArrayToMessage( receivedDoc) ; 
  //  serializeJson(receivedDoc, arguments.jsonBrod);
  //  arguments.recieved = false ; 
  //   mesh.sendBroadcast(arguments.jsonBrod);
  // }
  // else {
  //   serializeJson(receivedDoc, arguments.jsonBrod);
  //  arguments.recieved = false ; 
  //   mesh.sendBroadcast(arguments.jsonBrod);
  // }
   
   
  //   Serial.print("Mesh Broadcast - ");
  //   Serial.println(arguments.jsonBrod);
  //   taskSendMessage.setInterval((TASK_SECOND * 3));
    

