

#include <Wire.h>

#define SLAVE_ADDRESS 0x04

#define STX '\002'
#define ETX '\003'

void setup() {
  delay(2000);
  Serial.begin(57600);
 // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

 // define callbacks for i2c communication
  Wire.onReceive(receiveDataFromPi);
// Wire.onRequest(sendData);
}

void loop() {
}



// callback for received data
void receiveDataFromPi(int byteCount){
  
  while(Wire.available()){
    
    Serial.write(Wire.read());
  }

}

