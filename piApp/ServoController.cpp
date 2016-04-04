#include "ServoController.h"

ServoController::ServoController()
{
    
    // Set up pins
    wiringPiSetup () ;
    pinMode (TILT_PIN, OUTPUT) ;
    pinMode (YAW_PIN, OUTPUT) ;
    pinMode (ROLL_PIN, OUTPUT) ;

    
    updateAngles(0,0,0); // Initial angles before receiving data
    
    // Create thread for controlling the servo motors
    pthread_t RollThread;
    pthread_t YawThread;
    pthread_t TiltThread;

    struct servoThreadData RollThreadData = {&values.rollHold,ROLL_PIN};
    struct servoThreadData YawhTreadData = {&values.yawHold,YAW_PIN};
    struct servoThreadData TiltThreadData = {&values.tiltHold,TILT_PIN};

    pthread_create (&RollThread, NULL, RunServo, (void*) &RollThreadData);   
    pthread_create (&YawThread, NULL, RunServo, (void*) &YawhTreadData);   
    pthread_create (&TiltThread, NULL, RunServo, (void*) &TiltThreadData);   

}


void ServoController::updateAngles(int rollAngle, int yawAngle, int tiltAngle)
{
    values = {rollAngle, yawAngle, tiltAngle};     // THIS IS NOT HOW IT WILL WORK IN THE FUTURE
}

void* ServoController::RunServo(void* threadData)
{
    struct servoThreadData data = *((struct servoThreadData*) threadData);
    int pin = data.pin;
    int* delayPtr = data.holdValuePtr;
    
    while(1) {
        
    digitalWrite (pin, HIGH) ;
   
    delayMicroseconds(*delayPtr) ;
    
    digitalWrite (pin, LOW) ;
    
    delayMicroseconds(TOTAL_DELAY) ;
        
    }

    
    return 0;
}








ServoController::~ServoController()
{
}

