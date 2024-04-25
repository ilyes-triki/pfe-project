#include "db-data.h"


Variables args ;

int localArray[200];

//  fetching array from db
ArrayInfo getArray(FirebaseData &fbdo) {
    ArrayInfo result;
   
    if (Firebase.RTDB.getArray(&fbdo, "/options/boards/")) {
        FirebaseJsonArray &arr = fbdo.jsonArray();
        result.arraySize = arr.size();
        args.localarraySize = result.arraySize;
        result.arrayPtr = new int[result.arraySize];

        for (size_t i = 0; i < arr.size(); i++) {
            FirebaseJsonData jsonData;
            arr.get(jsonData, i);
            result.arrayPtr[i] = jsonData.intValue;
          
             size_t copySize = min(result.arraySize, sizeof(localArray) / sizeof(localArray[0]));
        memcpy(localArray, result.arrayPtr, copySize * sizeof(int));
        }
        
      
    delete[] result.arrayPtr;
 
        result.arrayPtr = nullptr;
        result.arraySize = 0;
    } else {
        result.arraySize = 0;
        result.arrayPtr = nullptr;
    }
    return result;
}


// sending urat message

void sendLocalMessage(int mode , int status)
    
 { 
  String msg ;
 DynamicJsonDocument doc(1024);
 JsonArray board_status = doc.createNestedArray("board_status");
  for (size_t i = 0; i < args.localarraySize; i++) {
     board_status.add(localArray[i]);
        }
        doc["mode"] = mode ;
        // doc["status"] = status ;

  serializeJson(doc , msg) ;
  Serial2.println(msg);
  Serial.println(msg);





}



// reading urat message

void recieveLocalMessage(){

if (Serial2.available() > 0) {
    String message = Serial2.readString();  
    Serial.println("Received message: " ); Serial.print(message);
  }


}



//  fetching mode from db


int getMode(FirebaseData &fbdo){
 if (Firebase.RTDB.getInt(&fbdo , "options/mode" )){ 
if (fbdo.dataType() == "integer"){
  
 return fbdo.intData();
  
  }}
};



//  fetching status on-off from db

// int getStatus(FirebaseData &fbdo){
//  if (Firebase.RTDB.getInt(&fbdo , "options/on-off" )){ 
// if (fbdo.dataType() == "integer"  ){
  
//   return fbdo.intData();

//   }
//   }
// };

//  fetching updating status from db
 bool getupdated(FirebaseData &fbdo){
 if (Firebase.RTDB.getBool(&fbdo , "options/updated" )){ 
if (fbdo.dataType() == "boolean"  ){
  
  return fbdo.boolData();

  }
  }
};

