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

    def handle(self):
        close = False
        while not close:
            self.data = self.request.recv(1024).strip()
            cur_thread = threading.current_thread();
            print ("data received from {} .".format(self.client_address[0]))
            self.server.notify("ReceivedTCP",self.data)
            self.request.sendto(b'Thanks!', self.client_address)
        
    

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
