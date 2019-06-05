
#include "avdweb_AnalogReadFast.h"
const int AnalogSensorPin = A5;

int on = B00000100;
int off = B00000000;
char stringData[] = "";
int clockCycle = 0;
const int maxClockCycle = 8;
bool receivingData = false;
/*
int testbyte[] = {0,0,1,0,0,0,0,1};
int syncbyte[] = {1,1,1,1,1,1,1,1};
int startbyte[] = {1,1,1,1,1,1,1,0};
int stopbyte[] = {1,1,1,1,1,1,0,1};
int nullbyte[] = {0,0,0,0,0,0,0,0};
int alternatingbyte[] = {0,1,0,1,0,1,0,1};
bool bytesent = false;
*/
enum stageprotocol {
  NULLBYTE, 
  RISEDGE, 
  STARTBYTE, 
  SYNCBYTE, 
  DATABYTE, 
  ENDBYTE, 
  RESTING
  };
  
//Create buffer stream to constantly check bytes
//byte databyte = B00000000;
byte bufferbyte = B00000000;
byte syncbyte = B11111111;
byte nullbyte = B00000000;
byte startbyte = B11111110;
byte stopbyte = B11111101;

enum stageprotocol state;
int bitval = 0;
void setup() {
Serial.begin(9600);
state = RESTING;
}

void loop() {
  
bitval = analogRead(AnalogSensorPin) >> 1;

delay(50);

if(bitval > 320){
  //Serial.println(1);
  bufferbyte = (bufferbyte << 1) | 1;
}else{
  bufferbyte = (bufferbyte << 1) & (syncbyte << 1);
  //Serial.println(0);
}


//Serial.println(bufferbyte,BIN);
//Serial.println(clockCycle);

switch(state){
    case RESTING:
        listenNullByte();
      break;
    case STARTBYTE:
        listenStartByte();
      break;
    case SYNCBYTE:
        listenSyncByte();
      break;
    case DATABYTE:
        listenDataByte();
      break;
    
    default:
      break;
}

if(receivingData)
  clockCycle++;

}

void resetClock(){
  clockCycle = 0;
}

void listenNullByte(){
  if(bufferbyte == nullbyte){
    state = STARTBYTE;
  }
}

void listenStartByte(){
  if(bufferbyte == startbyte){
    state = SYNCBYTE;
    //Serial.println("Start Byte!");
    receivingData = true;
    resetClock();
  }
}

void listenSyncByte(){
  if(bufferbyte == syncbyte && clockCycle == maxClockCycle){
    //Serial.println("Sync Byte!");
    state = DATABYTE;
    resetClock();
  }
}

void listenDataByte(){
  if(bufferbyte == stopbyte && clockCycle == maxClockCycle){
    //Serial.println("Stop Byte!");
    state = RESTING;
    receivingData = false;
    resetClock();
    bufferbyte = nullbyte;
  }else if(clockCycle == maxClockCycle){
    if(char(bufferbyte) == '.'){
    Serial.print(char(bufferbyte));
    Serial.println("");
    }else{
    Serial.print(char(bufferbyte));
    }
    state = SYNCBYTE;
    resetClock();
  }
}





//Receives empty array, populates it with binary version of char
void charToBinary(char a, int dataByte[]){
  for (int i = 0; i < 8; ++i) {
    dataByte[i] = (a >> (7 - i)) & 1;
  }
}
/*
//Iterates through each char in array, sends corresponding byte
void sendWord(char dataWord[], int len){
  sendByte(startbyte, sizeof(startbyte)/sizeof(startbyte[0]));
  for(int i = 0; i < len; i++){
    int arr[8];
    charToBinary(dataWord[i], arr);
    sendData(arr, sizeof(arr)/sizeof(arr[0]));
  }
  sendByte(stopbyte, sizeof(stopbyte)/sizeof(startbyte[0]));

  //Close data transmission
  bytesent = true;
  PORTD = off;
}
//Sends two bytes, sync byte, then data byte
void sendData(int data[],int len){
  sendByte(syncbyte, sizeof(syncbyte)/sizeof(syncbyte[0]));
  sendByte(data, len);
}
//Works by receiving array with length
//Looping through and depending on the value, setting a register HIGH or LOW
void sendByte(int data[],int len){
    //Serial.println("");
  for(int i = 0; i < len ; i++){
    delay(50);
    if(data[i]){
      PORTD = on;
      //Serial.print(1);
    }else{
      PORTD = off;
      //Serial.print(0);
    }
  }
  //Serial.println("");

}
*/
