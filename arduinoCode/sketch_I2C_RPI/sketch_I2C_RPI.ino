

#include <Wire.h>
 
#define SLAVE_ADDRESS 0x04

#define STX '\002'
#define ETX '\003'
//#define STX '|'
//#define ETX '|'
//int number = 0;
//int state = 0;
 
//double temp;
 
void setup() {
 //pinMode(13, OUTPUT);
 
 Serial.begin(9600);
 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);
 
 // define callbacks for i2c communication
 Wire.onReceive(receiveData);
 //Wire.onRequest(sendData);
}
 
void loop() {
 delay(1000);
 //temp = GetTemp();
}
 
// callback for received data
void receiveData(int byteCount){
  
  char values[5];
  char counter = 0;
  int16_t compassValue = -1;
  while(Wire.available() >= 5){
    counter = 0;
    compassValue = -1;
    while(counter < 5){
      values[counter] = Wire.read();   
      counter = counter + 1;
    }
//    Serial.print("values: ");
//    Serial.print(values[0]);
//    Serial.print(values[1]);
//    Serial.print(values[2]);
//    Serial.print(values[3]);
//    Serial.print(values[4]);
//    Serial.print("\n");
    
    if(values[0] == STX && values[4] == ETX){
      compassValue = values[1] - '0';
      compassValue *= 100;
      compassValue += (values[2] - '0') * 10;
      compassValue += values[3] - '0';
      Serial.print("compass value: ");
      Serial.print(compassValue);
      Serial.print("\n");
    }else{
      Serial.print("Wrong start or end value");
      Serial.print("\n");
    }
    
  }
  
// while(Wire.available()) {
//  number = Wire.read();
// 
//  if (number == 1){
//   if (state == 0){
//    digitalWrite(13, HIGH); // set the LED on
//    state = 1;
//   } else{
//    digitalWrite(13, LOW); // set the LED off
//    state = 0;
//   }
//  }
// 
//  if(number==2) {
//   number = (int)temp;
//  }
// }
}
 
// callback for sending data
//void sendData(){
// Wire.write(number);
//}
 
// Get the internal temperature of the arduino
//double GetTemp(void)
//{
// unsigned int wADC;
// double t;
// ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
// ADCSRA |= _BV(ADEN); // enable the ADC
// delay(20); // wait for voltages to become stable.
// ADCSRA |= _BV(ADSC); // Start the ADC
// while (bit_is_set(ADCSRA,ADSC));
// wADC = ADCW;
// t = (wADC - 324.31 ) / 1.22;
// return (t);
//}
