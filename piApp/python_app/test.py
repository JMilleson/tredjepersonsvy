from Server import ServerManager
from observer import *
import json



class piApp(Observer):
    def __init__ (self):
        self.server = ServerManager()
        self.server.subscribeToServer(self)
        self.server.start()
        
    def notify(self,message,data):                  
        if message == "ReceivedTCP" :
            try:
                s = data.decode(encoding='UTF-8')
                print ("Received: "+ s)
                datadict = json.loads(s)
                print (datadict)
            except:
                pass
                
        

if __name__ == "__main__":

    #Start App
    app = piApp()
