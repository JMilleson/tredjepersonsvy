/*
    C socket server example
*/
 
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#include <errno.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
 
// i2c address
#define ADDRESS 0x04

#define STX '\002'
#define ETX '\003'
 
// The I2C bus: This is for V2 pi's. For V1 Model B you need i2c-0
static const char *devName = "/dev/i2c-1";    
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    printf("I2C: Connecting\n");
    int file;

    if ((file = open(devName, O_RDWR)) < 0) {
        fprintf(stderr, "I2C: Failed to access %d\n", devName);
        exit(1);
    }

    printf("I2C: acquiring buss to 0x%x\n", ADDRESS);

    if (ioctl(file, I2C_SLAVE, ADDRESS) < 0) {
        fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", ADDRESS);
        exit(1);
    }

    int16_t compassValue, targetHeight;
    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        printf("message: %s\n", client_message);        
        compassValue = client_message[1] - '0';
        compassValue *= 100;
        compassValue += (client_message[2] - '0') * 10;
        compassValue += client_message[3] - '0';
        compassValue -= 200;
        targetHeight = client_message[4] - '0';
        targetHeight *= 1000;
        targetHeight = client_message[5] - '0';
        targetHeight *= 100;
        targetHeight += (client_message[6] - '0') * 10;
        targetHeight += client_message[7] - '0';
        printf("compass: %d\n", compassValue);
        printf("target height: %d\n", targetHeight);

        write(file, client_message, 10);
        
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
}