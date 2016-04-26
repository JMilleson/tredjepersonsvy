# By Martin and Jacob 2016




#HOW TO USE:
# from ArdupilotComm import *
# x = ArdupiloComm()
# x.sendTargets("





import serial
startChar = '\002'
endChar = '\003'
divChar = '|'


trackDataid = '1'
throttlePidid = '2'
yawPidid = '3'
rollPidid = '4'
pitchPidid = '5'
targetValuesid = '6'

class ArdupilotComm:
        
    """For communicating with ardupilot by USB"""
    ser = serial.Serial('/dev/ttyAMA0', 115200, timeout=1)

    if not ser.isOpen():
        ser.open()
    

    def __init__(self):
            self.Sending = False #A variable to make sure one is not trying to send different data at the same time. Not sure if needed


    def read(self): #Not yet defined how this should work
        pass
        
        #Settings could be PID.ground for ground PID, 2 for angle. 
    def sendPidSettings(self,pidtype,kp,ki,kd):
        if not self.Sending:
            self.Sending = True
            self.sendString(startChar)  # Indicate start of message
            
            self.sendString(pidtype) #Send a number indicating which pid parameters to send. 1 for ground 2 for distance and 3 for angle to user
            
            self.sendChar(divChar)
            self.sendString( (kp)+divChar)
            self.sendString( (ki)+divChar)
            self.sendString( (kd)+divChar)
            self.sendChar(endChar)
            self.Sending = False
            


        #Use this to send compassdata and two values for tracking
    def sendTrackData(self,compassAngle,angleUser,currentHeight,userDistance):
        if not self.Sending:
            self.Sending = True
            self.sendChar(startChar)  # Indicate start of message
            self.sendString(trackDataid) # indicates that track data will come
            self.sendChar(divChar)
            self.sendString(compassAngle)
            self.sendChar(divChar)
            self.sendString(angleUser)
            self.sendChar(divChar)
            self.sendString(currentHeight)
            self.sendChar(divChar)
            self.sendString(userDistance)
            self.sendChar(divChar)
            self.sendChar(endChar)
            self.Sending = False
        else:
            print("bus busy, skipped sending Track data")


    def sendString(self,str):
       #print(str)
        
        self.ser.write(bytes(str,'utf8'))
            
    def sendChar(self,charaa):
        #print(charaa)
        self.ser.write(bytes(charaa,'utf8'))

    def sendTargets(self,targetHeight,targetDistance):
        self.Sending = True

        self.sendChar(startChar)  # Indicate start of message
        self.sendChar(targetValuesid)
        self.sendChar(divChar)
        self.sendString(targetHeight)
        self.sendChar(divChar)
        self.sendString(targetDistance)
        self.sendChar(divChar)
        self.sendChar(endChar)
        self.Sending = False
