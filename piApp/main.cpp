#include <stdio.h>
//#include "ServoController.h"
#include "piServer.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    int readlength;
    char buffer[521];
    buffer[520]=0;
    piServer server;
    while(1) {
    readlength=recv(server.connfd,buffer,520,0);
    
    printf ("%.*s\n",readlength, buffer);
    printf("\n");

    
    }
/*    
    int a;
    int b;
    int c;
 
    ServoController test;
    
    while(1) {
        cout << "Enter 3 values";
    cin >> a;
    cin >>b;
    cin >>c;
    
    test.updateAngles(a,b,c);
    
    } */
    
    
    
    
}
