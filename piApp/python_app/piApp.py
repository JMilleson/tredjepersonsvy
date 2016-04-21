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
        self.calibrated = False
        self.client = None
        self.pinger = Pingers()
        
        portnr = 1339
        if len(sys.argv) > 1:
            portnr = int(sys.argv[1]) #Now you can have port as command line argument :)

        self.newSettings = False
        self.server = ServerManager(portnr)
        self.server.subscribeToServer(self)
        self.server.start()
        self.aurdoComm = ArdupilotComm()
        self.lock = False
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

        while(self.lock):
            pass
        self.lock = True
        self.aurdoComm.sendTrackData(
            
            self.sensordata['yaw'], 
            self.sensordata['uavangle'], 
            self.sensordata['height'], 
            self.sensordata['distance'] )
        self.lock = False
        
    def refreshUltraSensors(self):

        print("start get sensor....")
        while(self.lock):
            time.sleep(0.0000001)
        print("out of lock1")
        self.lock = True
        self.sensordata['height'] = self.pinger.getHeight()
        self.lock = False
        time.sleep(0.00025)
        
        #print(self.sensordata['height'])
        remoteping = self.pinger.getFixedRemote()

        
        while(self.lock):
            time.sleep(0.0000001)
        print("out of lock2")
        self.lock = True
        self.sensordata['distance'] = str(int(remoteping['distError']))
        self.sensordata['uavangle'] = str(int(remoteping['trackDiff']))
        #print("trackingdata: " + str(remoteping))
        self.lock = False

        print("stop sensor...")
        
        
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
                if "requestSensorData" in datadict:
                    #dictdata = {"sensorData", ""}
                    #dictdata["sensorData"].update(self.sensordata)
                    #print("hanindg you stuff")
                    while(self.lock):
                        time.sleep(0.0000001)
                    self.lock = True
                    dictdata = {"sendordata" : self.sensordata}
                    self.lock = False
                    jsondata = json.dumps(dictdata)
                    #print("HEJ I SEND U YO: "+jsondata)
                    self.client.request.sendto(jsondata.encode('utf-8'), self.client.client_address)
                    #print("Sended data " + jsondata)
                if "settings" in datadict :
                    print ("received settings")
                    self.newSettings = True
                    self.settings = datadict
                if "sensordataoculus" in datadict :
                    while(self.lock):
                        time.sleep(0.0000001)
                    self.lock = True
                    self.sensordata["roll"]=str(datadict["sensordataoculus"]["roll"])
                    self.sensordata["pitch"]=str(datadict["sensordataoculus"]["pitch"])
                    self.sensordata["yaw"]=str(datadict["sensordataoculus"]["yaw"])
                    self.lock = False
                if "requestVideo" in datadict :
                    videosettings = datadict["requestVideo"]
                    videostring = "raspivid " + \
                                  "-t " + str(videosettings["timeout"]) + \
                                  " -w " +str( videosettings["width"]) + \
                                  " -h " + str(videosettings["height"]) + \
                                  " -fps 42 -ih"+ \
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
                #print(traceback.format_exc())
                #or
                #print(sys.exc_info()[0])
                #self.aurdoComm.Sending=False
                #self.client.close = True
                #print(data)
                print("json error")
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
        if app.newSettings :
            for key in app.settings :    
                if key == "pitch" or key == "roll" or key == "trottle" or key == "yaw":
                    app.aurdoComm.sendPidSettings( {'trottle':'2','roll':'3','yaw':'4','pitch':'5'}[key],
                    app.settings["settings"][key]["p"],
                    app.settings["settings"][key]["i"],
                    app.settings["settings"][key]["d"])
                elif key == "target":
                    app.aurdoComm.sendTargets(
                    app.settings["settings"]["target"]["targetHeight"],
                    app.settings["settings"]["target"]["targetDistance"])
            app.newSettings = False
            print("new settings sent to aurdoino")
            print(app.settings)
        app.refreshUltraSensors()
        app.sendSensorData()
