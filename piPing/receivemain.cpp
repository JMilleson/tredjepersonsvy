#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <stdlib.h>
#include <wiringPi.h>


using namespace std;

#define PORTNR 7730


const int sigPinStyr = 17;
const int sigPinBabord = 27;


void setup() {
  wiringPiSetupGpio();
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, LOW);
}
  
int getCM() {
  //Send trigger pulse, pin is OUTPUT
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(sigPin, LOW);
    
  //Wait for echo to start, pin is INPUT
  pinMode(sigPin, INPUT);
  while(digitalRead(sigPin) == LOW);
    
  //Wait for echo end
  long startTime = micros();
  while(digitalRead(sigPin) == HIGH);
  long travelTime = micros() - startTime;
    
  //Get distance in cm
  int distance = travelTime / 58;
    
  return distance;
    
}



int main(int argc, char **argv)
{

    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr; 

    setup();


    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORTNR); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10);
    
    printf("Waiting for connection\n;"); 
    sockaddr_in client;
    client.sin_family = AF_INET;
    socklen_t c_len = sizeof(client);
    
    connfd = accept(listenfd, (sockaddr*)&client, &c_len);
    
    printf("Connection from: %d\n",client.sin_addr.s_addr);
    
    char buffer[30];
    
        pinMode(sigPinBabord, OUTPUT);
        pinMode(sigPinStyr, OUTPUT);


    while(read(connfd,buffer,30)!=-1) {
        
          //Send trigger pulse, pin is OUTPUT
        

        digitalWrite(sigPinStyr, HIGH);
        digitalWrite(sigPinBabord, HIGH);

        delayMicroseconds(4);

        digitalWrite(sigPinStyr, LOW);
        digitalWrite(sigPinBabord, LOW);        
        

          //Wait for echo to start, pin is INPUT
  pinMode(sigPinStyr,INPUT);
  pinMode(sigPinBabord, INPUT);

  int BabordValue;
  int StyrbordValue;
  
  while(true) {
      
      Babord = digitalRead(sigPinBabord);
      Styrbord = digitalRead(sigPinStyr);
      
   if(Babord == LOW && Styrbord == LOW);
      
  }
      
      
  
  while(digitalRead(sigPin) == LOW && digitalRead(sigPin);
        
        
        pinMode(sigPinBabord, OUTPUT);
        pinMode(sigPinStyr, OUTPUT);
        
        
        printf("Distance is %dcm\n", getCM());    }
        
}


  while(true)
      

