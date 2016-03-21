#define STX '|'
#define ETX '|'

void setup(){
  Serial.begin(9600); 
}

char values[22];
int i = 0;

void loop(){
  if(i == 0){
    //search for start token
    while(Serial.available()){
       if(Serial.read() == STX){
         values[i] = STX;
          i = 1;
          break;
       } 
    }
  }
  while(i > 0 && i < 20 && Serial.available()){
     values[i] = Serial.read();
    i = i + 1; 
  }
  if(i == 20){
    while(Serial.available()){
       if(Serial.read() == ETX){
         values[i] = ETX;
          i = 21;
          break;
       }     
    }
  }
  
  if(i == 21){
     values[21] = '\0';
     Serial.print("received: ");
     Serial.print(values);
     Serial.print("\n");
     if(values[0] == STX && values[20] == ETX){
    	int16_t compass = values[1] - '0';
    	compass *= 10;
    	compass += values[2] - '0';
    	compass *= 10;
    	compass += values[3] - '0';
    	int16_t currentHeight = values[4] - '0';
    	currentHeight *= 10;
    	currentHeight += values[5] - '0';
    	currentHeight *= 10;    
    	currentHeight += values[6] - '0';
    	currentHeight *= 10;    
    	currentHeight += values[7] - '0';
    	int16_t forward = values[8] - '0';
    	forward *= 10;
    	forward += values[9] - '0';
    	forward *= 10;
    	forward += values[10] - '0';
    	forward *= 10;
    	forward += values[11] - '0';
    	int16_t rotate = values[12] - '0';
    	rotate *= 10;
    	rotate += values[13] - '0';
    	rotate *= 10;
    	rotate += values[14] - '0';
    	rotate *= 10;
    	rotate += values[15] - '0';
    	int16_t targetHeight = values[16] - '0';
    	targetHeight *= 10;
    	targetHeight += values[17] - '0';
    	targetHeight *= 10;
    	targetHeight += values[18] - '0';
    	targetHeight *= 10;
    	targetHeight += values[19] - '0';
        Serial.print("compass: ");
        Serial.print(compass);
        Serial.print("\n");
        Serial.print("current height: ");
        Serial.print(currentHeight);
        Serial.print("\n");
        Serial.print("forward: ");
        Serial.print(forward);
        Serial.print("\n");
        Serial.print("rotate: ");
        Serial.print(rotate);
        Serial.print("\n");
        Serial.print("target height: ");
        Serial.print(targetHeight);
        Serial.print("\n");
        i = 0;

    }else{
      Serial.print("wrong start or end token");
      Serial.print("\n");
    }
     
     
  } 
 
 delay(100); 
}
