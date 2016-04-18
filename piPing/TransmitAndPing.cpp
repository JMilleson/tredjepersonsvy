#include <cstring>      // Needed for memset
#include <sys/types.h>
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions

#include <list>
#include <iostream>
#include<fcntl.h> // Make socket non-blocking

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

char* ipString = "192.168.42.1";
char* port = "7730";


const int sigPin = 17;
void setup() {
  wiringPiSetupGpio();
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, LOW);
}

using namespace std;
int main(int argc, char **argv)
{

    int status;
    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
    struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
    memset(&host_info, 0, sizeof host_info);

    host_info.ai_family = AF_UNSPEC;   //Any ip version
    host_info.ai_socktype = SOCK_STREAM; //TCP
    
    // Now fill up the linked list of host_info structs with google's address information.
    status = getaddrinfo(ipString,port, &host_info, &host_info_list);
    
        // getaddrinfo returns 0 on succes, or some other value when an error occured.
    // (translated into human readable text by the gai_gai_strerror function).
    if (status != 0) return false;
    
    
        int socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                      host_info_list->ai_protocol);
    if (socketfd == -1)  return false;;
    
    
    
        status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) return false;

    
        freeaddrinfo(host_info_list);

    
    cout << "connected!\n";
    int tmp;
    setup();
    while(true) {
    cin >> tmp;
        cout << "Tries to send\n";      
        send(socketfd,"Cool ",1,0);
        cout << "Sent\n";
        pinMode(sigPin, OUTPUT);
        digitalWrite(sigPin, HIGH);
        delayMicroseconds(5);
        digitalWrite(sigPin, LOW);
    }


}

  
