class Observable (object):
    observers = None
    def __init__(self):
        super(Observable, self).__init__()
        self.observers = []
    def subscribe(self, observer):
        #wtf init does not run :D
        if self.observers == None :
            self.observers = []
        self.observers.append(observer)
        print ("Subscribed")
    def notify(self,message,data):
        #wtf init does not run :D
        if self.observers == None :
            self.observers = []
        for observer in self.observers:
            observer.notify(message, data)

class Observer():
    def notify(self,message,data):
        print ("notified in observer class... why? Override?")


    
