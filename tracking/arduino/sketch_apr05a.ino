/* Ping))) Sensor
 */
 #include "array.h"
 #include "sensor.h"

//Pins for sensors
const int pingPin1 = 7;
const int pingPin2 = 8;
const int echoPin1 = 9;

//Skippes allowed, determines buffer size.
const int skipsAllowed = 100;

//Buffers for hit data
bool * a;
BoolArray h1Buffer;
BoolArray h2Buffer;

//State to send to arduino, 
//0 : do not move, 1: move right, 2: move left ???
int state = 0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  initArray(&h1Buffer,5);
  initArray(&h2Buffer,5);
}

void loop() {
  long d1, d2;

  delayMicroseconds(20);
  d1 = ping(pingPin2,pingPin2); // read
  insertFirstRoll(&h1Buffer,isHit(d1)); //stores if its a hit in hitbuffer
  delayMicroseconds(20);
  d2 = ping(pingPin1,echoPin1);
  insertFirstRoll(&h2Buffer,isHit(d2));
  
  bool isHitLeft = orArray(&h1Buffer);
  bool isHitRight = orArray(&h2Buffer);

  if(isHitLeft && isHitRight){
    state = 0;
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
  } else if (isHitLeft){
    state = 1;
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
  } else if (isHitRight){
    state = 2;
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
  } else {
    state = 0;
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
  }
  
  Serial.print("State: ");
  Serial.print(state);
  Serial.print(", left hit ");
  Serial.print(isHitLeft);
  Serial.print(", mesuring: ");
  Serial.print(d1);
  Serial.print(", hit array: ");
  printArray(&h1Buffer);
  Serial.print(", right hit ");
  Serial.print(isHitRight);
  Serial.print(", mesuring: ");
  Serial.print(d2);
  Serial.print(", hit array: ");
  printArray(&h2Buffer);
  Serial.print(", skips allowed: ");
  Serial.println(skipsAllowed);

  /*if(d1 - d2 > 50){
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
  } else if (d2 - d1 > 50){
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
  } else if (abs(d1 - d2) <40){
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH;
  } else {
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
  }*/

  /*if(d1<200){
    digitalWrite(12, HIGH);
  } else {
    digitalWrite(12, LOW);
  }
  if(d2<200){
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }

  Serial.print("Left: ");
  Serial.print(d1);
  Serial.print("cm, ");
  Serial.print("Right: ");
  Serial.print(d2);
  Serial.print("cm");
  Serial.println();*/

  //delay(1000);
}

bool isHit(long value){
  return value < 200;
}


