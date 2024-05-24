#include "painlessMesh.h"
#include <ArduinoJson.h>
#include "message.h"



// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT        5555

struct Nodes
{
  // 1
  unsigned long Nextnode = 3088934229 ;
  unsigned long PrevNode =3088937649 ;


// 2
  // unsigned long Nextnode = 0 ;
  // unsigned long PrevNode =532235501 ;
};
Nodes nodes ;
painlessMesh  mesh;
String msgDePanne  ;
int nextNodeReceived = 0 , prevNodeRecieved = 0 ;
MessageArguments arguments;
JsonDocument receivedDoc,brodDoc ;
Scheduler userScheduler; 
 




void sendMessage() ; 

Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, &sendMessage );

void sendMessage()
{
  if (receivedDoc.isNull() ) {
    
        Serial.println("No stored message to broadcast.");
        return;
    }
   if (prevNodeRecieved < 1  && msgDePanne!="pas de panne détectée") {
mesh.sendSingle(nodes.PrevNode, msgDePanne );
  Serial.print("Mesh Broadcast to the previous node - ");
    Serial.println(msgDePanne );

   } else {
          Serial.println("message de panne deliveré.");
   }
    if (nextNodeReceived < 1 && !arguments.jsonRec.isEmpty() && nodes.Nextnode != 0) {
mesh.sendSingle(nodes.Nextnode , arguments.jsonRec );
  Serial.print("Mesh Broadcast to the next node - ");
    Serial.println(arguments.jsonRec);

   } else {
          Serial.println("message de panne deliveré."); 
   }
        
         
   



   
}


void receivedCallback( uint32_t from, String &msg )
{
    if (msg == "ACKFROMNEXT") { 
        nextNodeReceived++;
    } else if (msg == "ACKFROMPREV") {
       prevNodeRecieved++;
    } else if (from == nodes.PrevNode){
        arguments.jsonRec = msg.c_str();
        
            nextNodeReceived = 0;
            deserializeJson(receivedDoc, arguments.jsonRec);
            Serial.print("Mesh Reciever from prev - ");
            Serial.println(arguments.jsonRec);
            mesh.sendSingle(from, "ACKFROMNEXT" );
        }else {
           arguments.jsonBrodError = msg.c_str();
            prevNodeRecieved = 0;
            deserializeJson(brodDoc,  arguments.jsonBrodError);
            Serial.print("Mesh Reciever from next- ");
            Serial.println(msgDePanne);
            mesh.sendSingle(from, "ACKFROMPREV" );
        }
}
void newConnectionCallback(uint32_t nodeId) {
  Serial.print("brodcaster id - "); Serial.println(nodeId);

}

void changedConnectionCallback() {
}

void nodeTimeAdjustedCallback(int32_t offset) {
  
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
  
 
  if (!receivedDoc.isNull())
  {
   testModes (receivedDoc) ; 
   msgDePanne =   checkIfWorking(brodDoc ) ;
     if (msgDePanne != "pas de panne détectée")
     {
      prevNodeRecieved = 0 ;
     }
     
   
  }

      mesh.update();
        
 

delay(500);
    }




