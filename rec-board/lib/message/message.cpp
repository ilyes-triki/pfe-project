
#include "message.h"

  
MessageArguments args ;

void testMessage(DynamicJsonDocument& receivedDoc) {
  if (receivedDoc.containsKey("boards_info")) {
    JsonArray boards_info = receivedDoc["boards_info"].as<JsonArray>();
    bool boardFound = false;
    for (JsonObject board : boards_info) {
      if (board["board_number"] == args.boardnum) {
        boardFound = true;
        board["led_status"] = args.led_status;
        break;
      }
    }
    if (!boardFound) {
      JsonObject newBoard = boards_info.createNestedObject();
      newBoard["board_number"] = args.boardnum;
      newBoard["led_status"] = args.led_status;
    }
  } else {
    JsonArray boards_info = receivedDoc.createNestedArray("boards_info");
    JsonObject newBoard = boards_info.createNestedObject();
    newBoard["board_number"] = args.boardnum;
    newBoard["led_status"] = args.led_status;
  }
}


void testModes (DynamicJsonDocument& receivedDoc ) {
//  Nedded data
int ldrValue = analogRead(args.ldrpin);
args.mode = receivedDoc["mode"].as<int>() ;
args.led_status = receivedDoc["status"];


// Serial messages
Serial.print("ldr value : ");
  Serial.println(ldrValue);
Serial.print("ledstatus: ");
  Serial.println(args.led_status);
Serial.print("mode: ");
  Serial.println(args.mode);
Serial.println("board number is : ") ;
 Serial.println(args.boardnum);



// Conditions
if (receivedDoc.containsKey("board_status")) {
      JsonArray board_status = receivedDoc["board_status"].as<JsonArray>();  


// Mode Monotone = 1
        
         if (args.mode == 1 && ldrValue < 1000)
       {
        digitalWrite(args.led , args.led_status);
       }


// Mode Specific-monotone = 2

      if (args.mode == 2 ){
        bool found = false;
        for (JsonVariant value : board_status) {
            if (value.is<int>() && value.as<int>() == args.boardnum) {
                found = true;
                break;
            }
        }
          if (found && ldrValue>1500)
          {
            digitalWrite(args.led , args.led_status) ; 
          }else {
             digitalWrite(args.led , 0) ; 
          }}


// Mode Specific-on

        if (args.mode == 3){
        bool found = false;
        for (JsonVariant value : board_status) {
            if (value.is<int>() && value.as<int>() == args.boardnum) {
                found = true;
                break;
            }
        } if (found)
          {
            digitalWrite(args.led , args.led_status) ; 
          }else {
             digitalWrite(args.led , 0) ;  }}




// Mode Specific-off

         if (args.mode == 4){
        bool found = false;
        for (JsonVariant value : board_status) {
            if (value.is<int>() && value.as<int>() == args.boardnum) {
                found = true;
                break;
            }
        } if (!found)
          {
            digitalWrite(args.led , args.led_status) ; 
          }else {
             digitalWrite(args.led , 0) ; }}



// Mode all-on

         if (args.mode == 5) {
      
            digitalWrite(args.led , 1) ; }




// Mode all-off

       if (args.mode == 6)
       {
      
            digitalWrite(args.led , 0) ; }

  
   }

}