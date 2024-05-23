
#include "message.h"




MessageArguments args ;

void addArrayToMessage(JsonDocument& receivedDoc) {
  if (receivedDoc.containsKey("boards_info") ) {
    JsonArray boards_info = receivedDoc["boards_info"].as<JsonArray>();
    bool boardFound = false;
    for (JsonObject board : boards_info) {
      if (board["board_number"] == args.boardnum) {
        boardFound = true;
        break;
      }
    }
    if (!boardFound) {
      JsonObject newBoard = boards_info.createNestedObject();
      newBoard["board_number"] = args.boardnum;
      newBoard["led_status"] =  "La lampe de cette carte n'est pas allumé";
    }
  } else {
    JsonArray boards_info = receivedDoc.createNestedArray("boards_info");
    JsonObject newBoard = boards_info.createNestedObject();
    newBoard["board_number"] = args.boardnum;
    newBoard["led_status"] = "La lampe de cette carte n'est pas allumé";
  }
}


void testModes (JsonDocument& receivedDoc ) {
//  Nedded data
int ldrValue = analogRead(args.ldrpin);
int ldrLampeValue = analogRead(args.ldrpinLampe);

args.mode = receivedDoc["mode"].as<int>() ;



// Serial messages
Serial.print("ldr value : ");
  Serial.println(ldrValue);
  Serial.print("ldrlampe value : ");
  Serial.println(ldrLampeValue);
Serial.print("mode: ");
  Serial.println(args.mode);
Serial.println("board number is : ") ;
 Serial.println(args.boardnum);
 

// Mode Monotone = 1
        
         if (args.mode == 1 && ldrValue < 1000)
       {
        digitalWrite(args.led , 1);
      
          if (ldrLampeValue < 1000)
            {
             args.working = false;
            }else {
                args.working = true;
            }
        
       }
       else {
 digitalWrite(args.led , 0);
       }
     // Mode all-on = 5

         if (args.mode == 5) {
           digitalWrite(args.led , 1) ;
        
            if (ldrLampeValue < 1000 && ldrValue<1000)
            {
             args.working = false;
            }else {
                args.working = true;
            }
      
          }




// Mode all-off = 6

       if (args.mode == 6)
       {   
        
         digitalWrite(args.led , 0) ;
      
      
            } 

  
 


// Conditions
if (receivedDoc.containsKey("board_status")) {
      JsonArray board_status = receivedDoc["board_status"].as<JsonArray>();  




// Mode Specific-monotone = 2

      if (args.mode == 2 ){
        bool found = false;
        for (JsonVariant value : board_status) {
            if (value.is<int>() && value.as<int>() == args.boardnum) {
                found = true;
                break;
            }
        }
          if (found && ldrValue<1000)
          {
              digitalWrite(args.led , 1) ; 


            if (ldrLampeValue < 1000)
            {
             args.working = false;
            }else {
                args.working = true;
            }
            
          
          }else {
          
             digitalWrite(args.led , 0) ; 
          }}


// Mode Specific-on = 3

        if (args.mode == 3){
        bool found = false;
        for (JsonVariant value : board_status) {
            if (value.is<int>() && value.as<int>() == args.boardnum) {
                found = true;
                break;
            }
        } if (found )
          {
             digitalWrite(args.led , 1) ; 

              if (ldrLampeValue < 1000 && ldrValue<1000)
            {
             args.working = false;
            }else {
                args.working = true;
            }
            
          }else {
             
             digitalWrite(args.led , 0) ;  }}




// Mode Specific-off = 4

         if (args.mode == 4){
        bool found = false;
        for (JsonVariant value : board_status) {
            if (value.is<int>() && value.as<int>() == args.boardnum) {
                found = true;
                break;
            }
        } if (found)
          {
             digitalWrite(args.led , 0) ; 

             
           
          }else {
             
             digitalWrite(args.led , 1) ; }}




}
}




String checkIfWorking ( JsonDocument& brodDoc ) {
if (args.working == 0)
{
  addArrayToMessage( brodDoc) ; 
    serializeJson(brodDoc, args.jsonBrodError);
 
}
else
{
 
  args.jsonBrodError = "pas de panne détectée";

}

return args.jsonBrodError;

}