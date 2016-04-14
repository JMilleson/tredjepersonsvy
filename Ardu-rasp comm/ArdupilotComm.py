# By Martin and Jacob 2016




#HOW TO USE:
# from ArdupilotComm import *
# x = ArdupiloComm()
# x.sendTargets("200","300") // Always send targets as ints in centimeters
# x.sendPidSettings(rollPidid,"11.3456","524.156","3564.3536")     





import smbus
address = 0x04
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
        
    """For communicating with ardupilot"""
    bus = smbus.SMBus(1)


    def __init__(self):
        pass

    def read(self): #Not yet defined how this should work
        pass
        
        #Send pid settings (floats in strings)
    def sendPidSettings(self,pidtype,kp,ki,kd):
        self.sendString(startChar)  # Indicate start of message
        
        self.sendString(pidtype) #Send a number indicating which pid parameters to send. 1 for ground 2 for distance and 3 for angle to user
        self.sendChar(divChar)
        self.sendString( (kp)+divChar)
        self.sendString( (ki)+divChar)
        self.sendString( (kd)+divChar)
        
        self.sendChar(endChar)



        #Use this to send compassdata and two values for tracking (ints in string)
    def sendTrackData(self,compassAngle,angleUser,currentHeight,userDistance):
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


    def sendString(self,str):
        for c in str:
            self.bus.write_byte(address,int.from_bytes(bytes(c,'utf-8'),'little'))
            
    def sendChar(self,charaa):
        self.bus.write_byte(address,int.from_bytes(bytes(charaa,'utf-8'),'little'))

    def sendTargets(self,targetHeight,targetDistance):
        self.sendChar(startChar)  # Indicate start of message
        self.sendChar(targetValuesid)
        self.sendChar(divChar)
        self.sendString(targetHeight)
        self.sendChar(divChar)
        self.sendString(targetDistance)
        self.sendChar(divChar)
        self.sendChar(endChar)

