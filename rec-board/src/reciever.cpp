#include "painlessMesh.h"
#include <ArduinoJson.h>
#include "message.h"



// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT        5555



int nodesReceived = 0;
MessageArguments arguments;
DynamicJsonDocument receivedDoc(1024) ;
Scheduler userScheduler; 
painlessMesh  mesh;



void sendMessage() ; 

Task taskSendMessage( TASK_SECOND * 2, TASK_FOREVER, &sendMessage );

void sendMessage()
{if (receivedDoc.isNull() ) {
    
        Serial.println("No stored message to broadcast.");
        return;
    }
   if ((nodesReceived != mesh.getNodeList().size() &&  !mesh.getNodeList().empty())  ) { 
 String jsonBrod = checkIfWorking(receivedDoc);
    mesh.sendBroadcast(jsonBrod);
  Serial.print("Mesh Broadcast - ");
    Serial.println(jsonBrod);
        }else {
Serial.println("All nodes have received the message.");
        }
}


void receivedCallback( uint32_t from, String &msg )
{
   if (msg == "ACK") { 
        nodesReceived++;
    } else {
      
      int nodesReceived = 0;
      arguments.jsonRec = msg.c_str();
  DeserializationError error = deserializeJson(receivedDoc, arguments.jsonRec);
  Serial.print("Mesh Reciever - "); Serial.println(arguments.jsonRec);
   mesh.sendSingle(from, "ACK");}
  
   
  
}
void newConnectionCallback(uint32_t nodeId) {

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
   testModes ( receivedDoc ) ; 
  }
      mesh.update();
        
 

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
    

   // Enable wakeup source for LDR
            // esp_sleep_enable_ext0_wakeup(args.ldrpin, LOW);
            // esp_deep_sleep_start();

              // Check LDR value and put ESP32 to sleep if above 1000
        // int ldrValue = analogRead(args.ldrpin);
        // if (ldrValue > 1000) {
        //     esp_sleep_enable_timer_wakeup(5 * 1000000); 
        //     esp_deep_sleep_start();
        // } 