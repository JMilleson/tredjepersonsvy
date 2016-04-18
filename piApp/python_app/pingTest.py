from Pingers import *
x = Pingers()
while True: 
    x.getRemotePing()
    x.getHeight()
    time.sleep(0.04)
