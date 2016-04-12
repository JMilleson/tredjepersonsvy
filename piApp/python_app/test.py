from Server import ServerManager
from observer import *



class piApp(Observer):
    def __init__ (self):
        self.server = ServerManager()
        self.server.subscribeToServer(self)
        self.server.start()
        
    def notify(self,message,data):
        if message == "ReceivedTCP" :
            print ("Received: "+data)
        

if __name__ == "__main__":

    #Start App
    app = piApp()
