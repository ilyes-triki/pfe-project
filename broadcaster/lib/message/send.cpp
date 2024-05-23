
#include "send.h"





// sending urat message

void sendLocalMessage(DynamicJsonDocument& receivedDoc )
    
 { 
  String msg ;
 DynamicJsonDocument doc(1024);
 if (receivedDoc.containsKey("boards_info")) {
        doc["boards_info"] = receivedDoc["boards_info"];
        serializeJson(doc , msg) ;
  Serial2.println(msg);
  Serial.println("sending message thru URAT");
  Serial.println(msg);
    }
       
       

  
}


