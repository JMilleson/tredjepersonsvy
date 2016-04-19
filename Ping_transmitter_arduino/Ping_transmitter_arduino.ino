
#define delaymicros 50000
//50 000 <=> 20 Hz

void setup() {
  // put your setup code here, to run once:


//Serial.begin(57600);

}

int pingPin= 7;

void loop() {
  // put your main code here, to run repeatedly:

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(5);

  unsigned long nextPingTime = micros()+delaymicros;

while(true) {
      nextPingTime += delaymicros;
      while(micros() < nextPingTime) ;


      //  Uncomment the next line for using the ping when receiving serial data versus always sending
  //while(!(Serial.read()+1)) ;

 
  
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(3);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
 // pinMode(pingPin, INPUT);
  //pulseIn(pingPin, HIGH);
 // Serial.write('k');
 // delayMicroseconds(250);

 // pinMode(pingPin, OUTPUT);
  //digitalWrite(pingPin, LOW);
  //delayMicroseconds(5);
}

}
