#!/usr/bin/python


#Use once every 250 microseconds



import time
import RPi.GPIO as GPIO

def getTimeMillis():
    return time.perf_counter()*1000

class Pingers:

        
        
        def __init__(self):
            self.thing = 0;
            self.calibStartup = False


            #Pins för Ping)) för avståndsmätning
            self.pinBabord = 13 #Board-numrering
            self.pinStyrbord = 11
            #pins för höjdmätning med SR-04
            self.pinTrig = 16
            self.pinEcho = 18
            self.avg = 0
            
            self.invalidDistance = 600
            self.intervall = 60
            self.startDistance = 300
            self.calibratedTime = getTimeMillis()
            self.speedOfSound = 34300
            self.tot = 0
            self.count = 0
            self.old = self.startDistance

            # Disable any warning message such as GPIO pins in use
            GPIO.setwarnings(False)

            # use the values of the GPIO pins, and not the actual pin number
            # so if you connect to GPIO 25 which is on pin number 22, the 
            # reference in this code is 25, which is the number of the GPIO 
            # port and not the number of the physical pin
            GPIO.setmode(GPIO.BOARD)


            GPIO.setup(self.pinBabord,GPIO.OUT)
            GPIO.setup(self.pinStyrbord,GPIO.OUT)

            GPIO.output(self.pinStyrbord, GPIO.LOW)
            GPIO.output(self.pinBabord, GPIO.LOW)
            
            
            GPIO.setup(self.pinTrig,GPIO.OUT)
            GPIO.setup(self.pinEcho,GPIO.IN)

            GPIO.output(self.pinTrig, GPIO.LOW)


            time.sleep(1)

            self.StyrFlag = 2
            self.BaboFlag = 2

            self.BaboStartTime = 0.0
            self.StyrStartTime = 0.0
            self.StyrEndTime = 0.0
            self.BaboEndTime = 0.0
            self.firstTime = 0.0


        def getRemotePing(self):

            GPIO.output(self.pinBabord, True)
            GPIO.output(self.pinStyrbord, True)

            time.sleep(0.000004) # Wait 4 microseconds

            GPIO.output(self.pinBabord, False)
            GPIO.output(self.pinStyrbord, False)

            GPIO.setup(self.pinBabord,GPIO.IN)
            GPIO.setup(self.pinStyrbord,GPIO.IN)

            while self.BaboFlag != 0 or self.StyrFlag != 0:
                    
                if self.BaboFlag==2 and GPIO.input(self.pinBabord) == 1:
                    self.BaboStartTime = time.time() 
                    self.BaboFlag -= 1

                if self.BaboFlag == 1 and GPIO.input(self.pinBabord) == 0:
                    self.BaboEndTime = time.time()
                    self.BaboFlag -= 1


                if self.StyrFlag==2 and GPIO.input(self.pinStyrbord) == 1:
                    self.StyrStartTime = time.time() 
                    self.StyrFlag -= 1


                if self.StyrFlag == 1 and GPIO.input(self.pinStyrbord) == 0:
                    self.StyrEndTime = time.time()
                    self.StyrFlag -= 1

            self.StyrFlag = 2
            self.BaboFlag = 2

            GPIO.setup(self.pinBabord,GPIO.OUT)
            GPIO.setup(self.pinStyrbord,GPIO.OUT)

            GPIO.output(self.pinStyrbord, GPIO.LOW)
            GPIO.output(self.pinBabord, GPIO.LOW)
            
            distanceStyr = (self.StyrEndTime - self.StyrStartTime) * 34300
            distanceBabo = (self.BaboEndTime - self.BaboStartTime) * 34300
            distanceDiff = distanceStyr - distanceBabo
            if distanceDiff > 20:
                distanceDiff = 0
            elif distanceDiff <-20:
                distanceDiff = 0
            return {'dist':distanceStyr,'trackDiff': distanceDiff}
        
        def getFixedRemote(self):

            #print("ping")
            temptime = getTimeMillis()
            target_time = temptime + self.intervall -  (((temptime - self.calibratedTime) *1000)% (self.intervall*1000))/1000
            
            while target_time-getTimeMillis() > 5:
                time.sleep((target_time-getTimeMillis()-4)/1000)
            while target_time > getTimeMillis():
                pass

            
            
            #diff = 0.05 - (((time.perf_counter()-self.firstTime)*100) % (0.05*100))/100
        
            #if (diff > 0.015):
            #    time.sleep(diff-0.015)
                
            #diff = 0.05 - (((time.perf_counter()-self.firstTime)*100) % (0.05*100))/100

            ctime = getTimeMillis();
            #print("Difference "+str(ctime-target_time)+"oftarget: "+str((ctime-self.calibratedTime)% self.intervall))
            unfixed=self.getRemotePing()
        
            #return {'diff':unfixed['diff']-diff/34300,'dist':unfixed['dist']-diff/34300}
            #print(unfixed)
            '''new = unfixed['dist']
            if self.calibStartup:
                if self.count == 0 :
                    self.old = new
                    self.tot = 0
                    self.count = 1
                else :
                    diff = new-self.old
                    self.tot += diff
                    self.count += 1
                    self.avg = self.tot/(self.count-1)
                    self.avgInTime = diff/self.speedOfSound
                    print("old/new: "+str(self.old)+" / "+str(new)+ " diff T: "+str(diff/self.speedOfSound))
                    #print("Diff: "+str(diff) + "diff in time: "+str(diff/self.speedOfSound)+" Average: " + str(self.avgInTime))#+"  time per loop: "+str(getTimeMillis()-self.lastTime))
                    self.old = new
                    self.calibratedTime -= 0.0003 #diff/self.speedOfSound
                    #if self.count == 2:
                    #    self.calibratedTime += diff / self.speedOfSound
                    if self.calibStartupStartTime - getTimeMillis() < -1000 :
                        self.calibStartup = False
                        #self.calibratedTime += self.avg * self.count
                        print ("calib done")
            else :
                '''

            if self.calibStartup and self.calibStartupStartTime - getTimeMillis() < -1000 :
                print ("calib done")
                self.calibStartup = False
                        
            self.calibratedTime +=  0.0312 #self.tot/self.count #0.03 #0.02685-
            #self.lastTime = getTimeMillis()

            
            if unfixed['dist'] > 620 :
                unfixed['dist'] = self.startDistance

            return {'distError':unfixed['dist'] - self.startDistance,'trackDiff':unfixed['trackDiff']}
        
        
        def setup(self):
            print("Calibrating sensors")
            while True :
                starttime = getTimeMillis()
                distance = self.getRemotePing()['dist']
                if distance >= self.invalidDistance :
                    print("invalid distance match: "+ str(distance))
                    continue
                print("valid match: "+str(distance))
                break
            print ("calib time: " + str(self.calibratedTime))
            self.calibratedTime = starttime + (distance - self.startDistance)/(self.speedOfSound/1000)
            self.calibStartup = True
            self.calibStartupStartTime = getTimeMillis()
            
            # vårt mål är att starta mätning vid starttime+n*0.05
        
        def getHeight(self):

            # sensor manual says a pulse ength of 10Us will trigger the 
            # sensor to transmit 8 cycles of ultrasonic burst at 40kHz and 
            # wait for the reflected ultrasonic burst to be received
            
            # to get a pulse length of 10Us we need to start the pulse, then
            # wait for 10 microseconds, then stop the pulse. This will 
            # result in the pulse length being 10Us.
            
            # start the pulse on the GPIO pin 
            # change this value to the pin you are using
            # GPIO output = the pin that's connected to "Trig" on the sensor
            GPIO.output(self.pinTrig, True)
            
            # wait 10 micro seconds (this is 0.00001 seconds) so the pulse
            # length is 10Us as the sensor expects
            time.sleep(0.00001)
            
            # stop the pulse after the time above has passed
            # change this value to the pin you are using
            # GPIO output = the pin that's connected to "Trig" on the sensor
            GPIO.output(self.pinTrig, False)

            # listen to the input pin. 0 means nothing is happening. Once a
            # signal is received the value will be 1 so the while loop
            # stops and has the last recorded time the signal was 0
            # change this value to the pin you are using
            # GPIO input = the pin that's connected to "Echo" on the sensor

            sendtTime = time.time()
            signaloff=0
            while (GPIO.input(self.pinEcho) == 0) and (time.time()<sendtTime+6):
                signaloff = time.time()

            # listen to the inputand in python ifea pin. Once a signal is received, record the
            # time the signal came through
            # change this value to the pin you are using
            # GPIO input = the pin that's connected to "Echo" on the sensor
            signalon = 0
            while GPIO.input(self.pinEcho) == 1:
                signalon = time.time()
            # work out the difference in the two recorded times above to 
            # calculate the distance of an object in front of the sensor
            timepassed = signalon - signaloff
            
            # we now have our distance but it's not in a useful unit of
            # measurement. So now we convert this distance into centimetres
            distance = timepassed * 17000
            
            # return the distance of an object in front of the sensor in cm
            
            GPIO.output(self.pinTrig, GPIO.LOW)
            
#            if (self.thing % 5) == 0:
#                print(int(distance))
#            self.thing += 1
            
            return str(int(min(500,max(0,distance))))
            
            # we're no longer using the GPIO, so tell software we're done
            # GPIO.cleanup()
                
