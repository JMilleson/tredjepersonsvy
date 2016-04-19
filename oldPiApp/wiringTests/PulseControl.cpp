// Build with:  g++ -Wall -o test PulseControl.cpp -lwiringPi

#include <wiringPi.h>
#include <iostream>
#include<stdio.h>

#define tiltpin 1
#define yawpin 4
#define rollpin 5

int main (void)
{
  wiringPiSetup () ;
  pinMode (tiltpin, OUTPUT) ;
  pinMode (yawpin, OUTPUT) ;
  pinMode (rollpin, OUTPUT) ;

  
  
  for(int i = 1000;;i=i%2200 + 100) {
 
   digitalWrite (tiltpin, HIGH) ;
   digitalWrite (yawpin, HIGH) ;
   digitalWrite (rollpin, HIGH) ;
   
   delayMicroseconds(i) ;
    
   digitalWrite (tiltpin, LOW) ;
   digitalWrite (yawpin, LOW) ;
   digitalWrite (rollpin, LOW) ;
       
   delayMicroseconds(20000) ;
  
   digitalWrite (tiltpin, HIGH) ;
   digitalWrite (yawpin, HIGH) ;
   digitalWrite (rollpin, HIGH) ;
   
   delayMicroseconds(i) ;
    
   digitalWrite (tiltpin, LOW) ;
   digitalWrite (yawpin, LOW) ;
   digitalWrite (rollpin, LOW) ;
       
   delayMicroseconds(20000) ;

    
    printf("%i\n",i);
    
  }
  return 0 ;
}
