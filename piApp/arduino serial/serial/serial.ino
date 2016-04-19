//Made by Martin Dahlgren 2016

// This program should probably use AP_HAL library to use usb instead of Serial. 


void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(Serial.read());
  
}
int oculusCompass, angleToUser;
double groundP, groundI, groundD, uLengthP, uLengthI, uLengthD, uAngleP, uAngleI, uAngleD;

char serialinputbuffer[15];

void readFromPi() {  //Antingen så skickas parameterreferensstruct eller liknande som argument.
                     //eller så har denna funktion alla parameterar i sitt scope

  while(Serial.available() && (Serial.read() != '#')) ; //Read until begin-character

  char r;
  if(Serial.available()) {
      r=Serial.read();
      switch (r) {
        case 1: // ground PID parameters
          readFloat(&groundP);
          readFloat(&groundI);
          readFloat(&groundD);
          break;
        case 2: //User distance PID parameters
          readFloat(&uLengthP);
          readFloat(&uLengthI);
          readFloat(&uLengthD);
          break;
        case 3: //User angle pid parameters 
          readFloat(&uAngleP);
          readFloat(&uAngleI);
          readFloat(&uAngleD);
            break;
        case 6: //track data is incoming. This is most common case
          readInt(&oculusCompass);
          readInt(&angleToUser);
          break;
    }

  }
  }


  void readInt(int* parameterP) {
    char read = 0;
    for(int i=0;i<15 && (serialinputbuffer[i]=Serial.read()) != ' ';i++) ;
    *parameterP = atoi(serialinputbuffer);
  }

    void readFloat(double* parameterP) {
    char read = 0;
    for(int i=0;i<15 && (serialinputbuffer[i]=Serial.read()) != ' ';i++) ;
    *parameterP = atof(serialinputbuffer);
  }
