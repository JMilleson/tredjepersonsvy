

#include <Wire.h>
 
#define SLAVE_ADDRESS 0x04

#define STX '\002'
#define ETX '\003'
//#define STX 'a'
//#define ETX 'b'

const int pingPin = 7;

int16_t compass, currentHeight, targetHeight, forward, rotate;



void setup() {
  delay(5000);
  compass = 0;
  currentHeight = 0;
  forward = 0;
  rotate = 0;  
  targetHeight = 0;
 Serial.begin(57600);
 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);
 
 // define callbacks for i2c communication
 Wire.onReceive(receiveDataFromPi);
 //Wire.onRequest(sendData);
}
 
void loop() {
  updateHeight();
  updatePosition();
  sendDataToApm();    
  delay(2000);
}

void sendDataToApm(){
  unsigned char values[21];
  getDataToSend(values);
  //values[21] = '\0';
  //Serial.print(compass);
  //Serial.print("\n");
  Serial.write(values, 21);  
}

void getDataToSend(unsigned char values[]){
  values[0] = STX;
  //convert compass to chars  
  values[1] = '0' + ((compass / 100) % 10);
  values[2] = '0' + ((compass / 10) % 10);
  values[3] = '0' + (compass % 10);
  //convert height to chars
  values[4] = '0' + ((currentHeight / 1000) % 10);
  values[5] = '0' + ((currentHeight / 100) % 10);
  values[6] = '0' + ((currentHeight / 10) % 10);
  values[7] = '0' + (currentHeight % 10);
  //convert forward to chars
  values[8] = '0' + ((forward / 1000) % 10);
  values[9] = '0' + ((forward / 100) % 10);
  values[10] = '0' + ((forward / 10) % 10);
  values[11] = '0' + (forward % 10);
  //convert rotate to chars
  values[12] = '0' + ((rotate / 1000) % 10);
  values[13] = '0' + ((rotate / 100) % 10);
  values[14] = '0' + ((rotate / 10) % 10);
  values[15] = '0' + (rotate % 10);
  //convert target height to chars
  values[16] = '0' + ((targetHeight / 1000) % 10);
  values[17] = '0' + ((targetHeight / 100) % 10);
  values[18] = '0' + ((targetHeight / 10) % 10);
  values[19] = '0' + (targetHeight % 10);
  
  values[20] = ETX;
  
}

void updatePosition(){
  forward = (forward + 1) % 100;
  rotate = (rotate + 2) % 100;  
}

void updateHeight(){
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  cm = duration / 29 / 2;
  currentHeight = cm;
}
 
// callback for received data
void receiveDataFromPi(int byteCount){
  
  char values[10];
  char counter = 0;
  int16_t compassValue = -1;
  int16_t heightValue = -1;
  while(Wire.available() >= 10){
    counter = 0;
    compassValue = -1;
    heightValue = -1;
    while(counter < 10){
      values[counter] = Wire.read();   
      counter = counter + 1;
    }    
    if(values[0] == STX && values[8] == ETX){
      compassValue = values[1] - '0';
      compassValue *= 100;
      compassValue += (values[2] - '0') * 10;
      compassValue += values[3] - '0';
      //compassValue -= 200;
      heightValue = values[4] - '0';
      heightValue *= 1000;
      heightValue += (values[5] - '0') * 100;
      heightValue += (values[6] - '0') * 10;
      heightValue += values[7] - '0';
      compass = compassValue;
      targetHeight = heightValue;
    }
    
  }
}
