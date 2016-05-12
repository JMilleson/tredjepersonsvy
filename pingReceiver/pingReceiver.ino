
#define delaymicros 50000
//50 000 <=> 20 Hz

int heightOutPin= 6;
int heightInPin = 7;
int pinBabord = 8;
int pinStyrbord = 9;
int userDistance;
int userAngle;
unsigned long calibratedTime;

void pinsetup();
int getHeight();
void getRemotePing();
void getFixedRemote();


void setup() {


Serial.begin(57600);


pinMode(heightOutPin, OUTPUT);
pinMode(heightInPin, INPUT);

pinMode(pinStyrbord,OUTPUT);
pinMode(pinBabord,OUTPUT);

delayMicroseconds(4);


pinsetup();


}



void loop() {
  getFixedRemote();  
  getHeight();
  
  Serial.println(userDistance);
  Serial.println(userAngle);
  
  //Send data over serial(?)
  
}

int getHeight() {

    unsigned long sendtTime;
    
      digitalWrite(heightOutPin, HIGH);
            
        delayMicroseconds(10);
            
        digitalWrite(heightOutPin,LOW);
    
        sendtTime = micros();
        unsigned int signaloff=0;
        
        while ((digitalRead(heightInPin) == 0) && ( (signaloff = micros()) < sendtTime+6)) ;
       
        unsigned int signalon = 0;
        while (digitalRead(heightInPin) == 1) 
            signalon = micros();

        int distance = (signalon-signaloff) * 17000 / 1000000;
        
        //distance in cm
        return distance;
        
}


void getRemotePing() {

char BaboFlag = 2;
char StyrFlag = 2;

unsigned long BaboStartTime,BaboEndTime,StyrStartTime,StyrEndTime;


            
digitalWrite(pinStyrbord,HIGH);
digitalWrite(pinBabord,HIGH);

delayMicroseconds(4);
            

            
digitalWrite(pinStyrbord,LOW);
digitalWrite(pinBabord,LOW);


pinMode(pinStyrbord,INPUT);
pinMode(pinBabord,INPUT);

            while (BaboFlag != 0 || StyrFlag != 0) {
                    
                 if (BaboFlag==2 && digitalRead(pinBabord) == 1) {

                    BaboStartTime = micros() ;
                    BaboFlag -= 1;
                    
                 }

                if (BaboFlag==1 && digitalRead(pinBabord) == 0) {

                    BaboEndTime = micros() ;
                    BaboFlag -= 1;
                    
                 }

                 
                 if (StyrFlag==2 && digitalRead(pinStyrbord) == 1) {

                    StyrStartTime = micros() ;
                    StyrFlag -= 1;
                    
                 }

                if (StyrFlag==1 && digitalRead(pinStyrbord) == 0) {

                    StyrEndTime = micros() ;
                    StyrFlag -= 1;
                    
                 }

                    
pinMode(pinStyrbord,OUTPUT);
pinMode(pinBabord,OUTPUT);

            
digitalWrite(pinStyrbord,LOW);
digitalWrite(pinBabord,LOW);

            int distanceStyr = (StyrEndTime - StyrStartTime) * 34300 / 1000000.0;
            int distanceBabo = (BaboEndTime - BaboStartTime) * 34300 / 1000000.0;
            int distanceDiff = distanceStyr - distanceBabo;
            if (distanceDiff > 20)
                distanceDiff = 0;
            else if (distanceDiff <-20)
                distanceDiff = 0;
            
            userAngle = distanceDiff;
            userDistance = distanceStyr;
                    
} 
}
  

    
void getFixedRemote(void) {


            unsigned long temptime = micros();
            unsigned long target_time = temptime + delaymicros - ((temptime - calibratedTime) % delaymicros);
            
            while (target_time-micros() > 0) ;

            
            getRemotePing();
        
    }
    
void pinsetup() {

    unsigned long starttime;
    
            while (1) {
                starttime = micros();
                getRemotePing();
                if (userDistance >= 500)
                    continue;
                break;
            }
            calibratedTime = starttime + (userDistance - 300)/(34300.0/1000000.0);            
    
}
