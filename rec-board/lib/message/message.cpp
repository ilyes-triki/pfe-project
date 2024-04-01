
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
args.mode = receivedDoc["mode"].as<String>() ;
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

// Mode Specific

      if (args.mode == "specific"){
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


// Mode Monotone
        
         if (args.mode == "monotone" && ldrValue < 1000)
       {
        digitalWrite(args.led , args.led_status);
       }



// Mode Specific-on

        if (args.mode == "specific-on"){
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

         if (args.mode == "specific-off"){
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

         if (args.mode == "all-on") {
      
            digitalWrite(args.led , 1) ; }




// Mode all-off

       if (args.mode == "all-off")
       {
      
            digitalWrite(args.led , 0) ; }

  
   }

}