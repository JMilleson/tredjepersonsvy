# By Martin Dahlgren 2016

import serial
from enum import Enum  #Requires python3
port = '/dev/ttyACM0'

PID = Enum('PID', 'ground userDistance userAngle')

class ArdupilotComm:
        
    """For communicating with ardupilot by USB"""
    ser = 0


    def __init__(self):
        ser = serial.Serial(port, 9600)

    def read(self): #Not yet defined how this should work
        pass

        
        #Settings could be PID.ground for ground PID, 2 for angle. 
    def sendPidSettings(self,pidtype,kp,ki,kd):
        ser.write('#')  # Indicate start of message
        
        if pidtype == PID.ground: #Send a number indicating which pid parameters to send. 1 for ground 2 for distance and 3 for angle to user
                    ser.write(1)
        elif pidtype==PID.userDistance:
                    ser.write(2)
        elif pidtyp==PID.userAngle:
                    ser.write(3)
        
        ser.write( str(kp)+' ')
        ser.write( str(ki)+' ')
        ser.write( str(kd)+' ')


        #Use this to send compassdata and two values for tracking
    def sendTrackData(self,compassAngle,angleUser):
        ser.write('#')  # Indicate start of message
        ser.write(6) # 6 indicates that track data will come
        ser.write( str(compassData) )
        ser.write(' ')
        ser.write( str(angleUser)   )
        ser.write(' ')
