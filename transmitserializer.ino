
int on = B00000100;
int off = B00000000;
char stringData[] = "My name is Maximus Decimus Meridius, Commander of the Armies of the North, General of the Felix Legions, loyal servant to the true emperor, Marcus Aurelius. Father to a murdered son, husband to a murdered wife. And I will have my vengeance, in this life or the next.";
int testbyte[] = {0,0,1,0,0,0,0,1};
int syncbyte[] = {1,1,1,1,1,1,1,1};
int startbyte[] = {1,1,1,1,1,1,1,0};
int stopbyte[] = {1,1,1,1,1,1,0,1};
int nullbyte[] = {0,0,0,0,0,0,0,0};
int alternatingbyte[] = {0,1,0,1,0,1,0,1};
bool bytesent = false;

unsigned long startMillis;
unsigned long currentMillis;

void setup() {
  // put your setup code here, to run once:
DDRD = B00000100;
//Serial.begin(9600);
//pinMode(3, OUTPUT);
//digitalWrite(3,HIGH);
//Timer = micros();
//startMillis = millis();
delay(5000);
}
void loop() {
//currentMillis = millis();

//if(!bytesent){
sendWord(stringData, sizeof(stringData) / sizeof(stringData[0]) );
//}
//sendByte(syncbyte, sizeof(syncbyte)/sizeof(syncbyte[0]));
//sendByte(nullbyte, sizeof(nullbyte)/sizeof(syncbyte[0]));
//sendByte(alternatingbyte, sizeof(alternatingbyte)/sizeof(syncbyte[0]));
}
//Receives empty array, populates it with binary version of char
void charToBinary(char a, int dataByte[]){
  for (int i = 0; i < 8; ++i) {
    dataByte[i] = (a >> (7 - i)) & 1;
  }
}
//Iterates through each char in array, sends corresponding byte
void sendWord(char dataWord[], int len){
  sendByte(nullbyte, sizeof(nullbyte)/sizeof(nullbyte[0]));
  sendByte(startbyte, sizeof(startbyte)/sizeof(startbyte[0]));
  for(int i = 0; i < len; i++){
    int arr[8];
    charToBinary(dataWord[i], arr);
    sendData(arr, sizeof(arr)/sizeof(arr[0]));
  }
  sendByte(syncbyte, sizeof(syncbyte)/sizeof(syncbyte[0]));
  sendByte(stopbyte, sizeof(stopbyte)/sizeof(stopbyte[0]));
  sendByte(syncbyte, sizeof(syncbyte)/sizeof(syncbyte[0]));
  //Close data transmission
  bytesent = true;
  //PORTD = off;
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
