#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

// Use the wiringPi numberingScheme
#define YAW_PIN 1
#define TILT_PIN 4
#define ROLL_PIN 5
#define TOTAL_DELAY 20000 //Microseconds for a servo signal loop

#include <pthread.h>  // for threading, don't forget -lpthread
#include <unistd.h> //for sleep
#include <wiringPi.h> // For using IO-pins. Don't forget to have on compiler path and include wiringPi library


// Struct to hold how long to hold roll yaw and tilt values
    struct servoDataStruct {
        int rollHold;
        int yawHold;
        int tiltHold;    
};

    struct servoThreadData {
        int* holdValuePtr;
        int pin;
    };

class ServoController
{
public:
    ServoController();
    void updateAngles(int rollAngle, int yawAngle, int tiltAngle);
    ~ServoController();
private:
    struct servoDataStruct values;
    static void* RunServo(void* threadData);
};

#endif // SERVOCONTROLLER_H
