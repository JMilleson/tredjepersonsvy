#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

#define STX '\002'
#define ETX '\003'
#define divider '|'

int main(int argc , char *argv[])
{

	//-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
	int uart0_filestream = -1;
	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
	
	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B57600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);


	long ints[5];
	float doubles[12];
	ints[0] = 111;
	ints[1] = -222;
	ints[2] = 333;
	ints[3] = 444;
	ints[4] = 555;
	ints[5] = 666;
	doubles[0] = 0.1;
	doubles[1] = 0.1;
	doubles[2] = 0.1;
	doubles[3] = 0.01;
	doubles[4] = 0.01;
	doubles[5] = 0.01;
	doubles[6] = 0.2;
	doubles[7] = 0.2;
	doubles[8] = 0.2;
	doubles[9] = 0.001;
	doubles[10] = 0.001;
	doubles[11] = 0.001;
	unsigned char data[256];
	data[0] = STX;
	long count = 1;
	int i = 0;
	for (i = 0; i < 5; i++)
	{
		char str[8];
		sprintf(str,"%d", ints[i]);
		int a = 0;
		for (; a < sizeof(str) / sizeof(str[0]); a++)
		{
			if (str[a] == '\0') {
				break;
			}
			data[count++] = str[a];
		}
		data[count++] = divider;
	}
	for (i = 0; i < 12; i++)
	{
		char str[12];
		sprintf(str,"%f", doubles[i]);
		int a = 0;
		for (; a < sizeof(str) / sizeof(str[0]); a++)
		{
			if (str[a] == '\0') {
				break;
			}
			data[count++] = str[a];
		}
		data[count++] = divider;
	}
	data[count++] = ETX;
	data[count++] = '\0';

	while(1){
		if (uart0_filestream != -1){
			printf("data: %s\n", data);
			int written = write(uart0_filestream, data, count);		//Filestream, bytes to write, number of bytes to write
			printf("written: %d\n", written);
			if (written < 0)
			{
				printf("UART TX error\n");
			}
		}
		sleep(1);
	}

}

