from Server import ServerManager
from observer import *
from ArdupilotComm import ArdupilotComm
import json
import sys
import traceback
import os


class piApp(Observer):
    def __init__ (self):
        self.server = ServerManager(1339)
        self.server.subscribeToServer(self)
        self.server.start()
        self.aurdoComm = ArdupilotComm()
        self.sensordata = {
            'height': "0",
            'distance': "0",
            'uavangle': "0",
            'roll': "0",
            'pitch': "0",
            'yaw': "0"};
    def sendSensorData(self):
        #print("sending tracking data: ")
        #print(self.sensordata)
        self.aurdoComm.sendTrackData(
            self.sensordata['yaw'], 
            self.sensordata['uavangle'], 
            self.sensordata['height'], 
            self.sensordata['distance'] )
        
    def notify(self,message,data,sender):                  
        if message == "ReceivedTCP" :
            try:
                s = data.decode(encoding='UTF-8')
                print ("Received: "+ s)
                datadict = json.loads(s)
                print (datadict)
                if "settings" in datadict :
                    print ("received settings")
                    for key in datadict["settings"] :
                        print("keytest:"+key)
                        if key == "pitch" or key == "roll" or key == "trottle" or key == "yaw":
                            #print ("sending "+key+" values")          
                            self.aurdoComm.sendPidSettings( {'trottle':'2','roll':'3','yaw':'4','pitch':'5'}[key],
                                datadict["settings"][key]["p"],
                                datadict["settings"][key]["i"],
                                datadict["settings"][key]["d"])
                        elif key == "target":
                            self.aurdoComm.sendTargets(
                                datadict["settings"]["target"]["targetHeight"],
                                datadict["settings"]["target"]["targetDistance"])
                if "sensordataoculus" in datadict :
                    self.sensordata["roll"]=str(datadict["sensordataoculus"]["roll"])
                    self.sensordata["pitch"]=str(datadict["sensordataoculus"]["pitch"])
                    self.sensordata["yaw"]=str(datadict["sensordataoculus"]["yaw"])
                    self.sendSensorData()
                if "requestVideo" in datadict :
                    videosettings = datadict["requestVideo"]
                    videostring = "raspivid " + \
                                  "-t " + str(videosettings["timeout"]) + \
                                  " -w " +str( videosettings["width"]) + \
                                  " -h " + str(videosettings["height"]) + \
                                  " -fps 42" \
                                  " -b " + str(videosettings["bitrate"]) + \
                                  " -n -qp " + str(videosettings["QP"]) + \
                                  " -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay" +\
                                  " config-interval=1 pt=96" + \
                                  " ! udpsink host="+sender+\
                                  " port=" + str(videosettings["port"])
                    print (videostring)
                    os.system(videostring);
                    
            except Exception  as err:
                print(traceback.format_exc())
                #or
                print(sys.exc_info()[0])
                
if __name__ == "__main__":

    #Start App
    app = piApp()
    input("press any key toexit")