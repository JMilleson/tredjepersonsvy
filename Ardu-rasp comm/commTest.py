from ArdupilotComm import ArdupilotComm

x = ArdupilotComm()

while true:

    x.sendTrackData(10,11,12,13)

    x.sendPidSettings(tr0ackDataid,1.31,9919931.6546465,5345.11)
