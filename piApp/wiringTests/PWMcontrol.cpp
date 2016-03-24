#include <wiringPi.h>
#include <softPwm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<iostream>

using namespace std;

int main (void)
{
   //         int value;
    
    //       cout<<"starting!\n";


           
  wiringPiSetup() ;
  


 softPwmCreate (4, 0, 400) ;

 for(int i;;) {
 
 for(i=1;i<23;i++) {

  //     cout<<"Enter integer\n";
   //     cin >> value;
       sleep(1);
       
     softPwmWrite (4, i) ;
 }
 }
  return 0 ;
}
