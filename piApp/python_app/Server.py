import threading
import socketserver
from observer import *

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer, Observable):
    def __init(self):
        super(ThreadedTCPServer, self).__init__()

class ThreadedTCPHandler(socketserver.BaseRequestHandler):
    """
    The request handler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """
    def send(self, data):
        #print("attempt to send ping...")
        self.request.sendto(data, self.client_address)

    def handle(self):
        self.close = False

        #self.data = self.request.recv(1024).strip()
        print("connected to basestation")
        self.server.notify("ClientConnected",self,self.client_address[0]);
        
        while not self.close:
            self.data = self.request.recv(1024).strip()
            cur_thread = threading.current_thread();
            #print ("data received from {} .".format(self.client_address[0]))
            self.server.notify("ReceivedTCP",self.data,self.client_address[0])
            #self.request.sendto(b'Thanks!', self.client_address)
        
    

class ServerManager ():
    def __init__(self,port=1338, host="192.168.42.1"):
        self.serverInfo = {"host":host,
                      "port":port}
        self.server = ThreadedTCPServer((self.serverInfo["host"], self.serverInfo["port"]), ThreadedTCPHandler)
    def start(self):
        server_thread = threading.Thread(target=self.server.serve_forever)
        server_thread.daemon = True;
        server_thread.start()
        print ("Server loop running")
    def subscribeToServer (self, observer):
        print("subscibing in server manager")
        self.server.subscribe(observer)
