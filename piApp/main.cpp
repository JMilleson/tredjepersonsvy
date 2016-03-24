#include <stdio.h>
#include "ServoController.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    
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
    
    }
    
    
    
    
}
