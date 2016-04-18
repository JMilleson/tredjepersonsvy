from Server import ServerManager
from observer import *
from ArdupilotComm import ArdupilotComm
import subprocess
import os
from Pingers import *
#from datetime import *
import json
import sys
import traceback
import threading


class piApp(Observer):
    def __init__ (self):
        self.client = None
        self.pinger = Pingers()
        self.server = ServerManager(1338)
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
    def currenttime(self):
        #time = datetime.datetime.strptime(timestamp.split(".")[0], "%Y-%m-#dT%H:%M:%S")
        #return time + datetime.timedelta(0,float("." + timestamp[:-1].split(".")[0]))
        return time.perf_counter();  
    def sendSensorData(self):
        #print("sending tracking data: ")
        #print(self.sensordata)
        self.aurdoComm.sendTrackData(
            self.sensordata['yaw'], 
            self.sensordata['uavangle'], 
            self.sensordata['height'], 
            self.sensordata['distance'] )
        
    def refreshUltraSensors(self):
        self.sensordata['height'] = self.pinger.getHeight()
        
        if self.client != None :
            #print("we want to pingme")
            self.requestPingTimestamp = self.currenttime()
            self.client.send(b'pingme')
            remoteping = self.pinger.getRemotePing()
            self.sensordata['distance'] = remoteping['dist']
            self.sensordata['uavangle'] = remoteping['diff']
            #print("trackingdata: " + str(remoteping))
        
        
    def notify(self,message,data,sender):                  
        if message == "ReceivedTCP" :
            try:
                s = data.decode(encoding='UTF-8')
                #print ("Received: "+ s)
                datadict = json.loads(s)
                #print (datadict)
                #if "sensorConfirmation" in datadict:
                #    pass
                #    print(self.currenttime() - self.requestPingTimestamp)
                if "settings" in datadict :
                    print ("received settings")
                    for key in datadict["settings"] :
                        #print("keytest:"+key)
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
                    self.thread = GStreamerThread(videostring)
                    self.thread.start()
                if "abortVideo" in datadict:
                    videostring = "ps -ef | grep raspivid | grep -v grep | awk '{print $2}' | xargs kill -9"
                    print (videostring);
                    os.system(videostring)
                    
            except Exception  as err:
                print(traceback.format_exc())
                #or
                print(sys.exc_info()[0])
        elif message == "ClientConnected" :
            print("basestation stored")
            self.client = data

class GStreamerThread (threading.Thread):
    def __init__(self, cmd):
        super(GStreamerThread, self).__init__()
        self.cmd = cmd
    def run(self):
        os.system(self.cmd)
                
if __name__ == "__main__":

    #Start App
    app = piApp()
    while True:
        app.refreshUltraSensors()
        time.sleep(1)
