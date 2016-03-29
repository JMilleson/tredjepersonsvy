#ifndef PISERVER_H
#define PISERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>



#define PORTNR 5000

class piServer
{
public:
    piServer();
    ~piServer();
    int connfd;
    
};

#endif // PISERVER_H
