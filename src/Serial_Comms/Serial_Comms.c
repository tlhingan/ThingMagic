#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyO4"
#define _POSIX_SOURCE 1
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;
struct termios oldtios, newtios;

void
signal_handler_IO(int status);
int wait_flag=TRUE, fd;

int
SerialSetup(){
	//struct termios oldtios, newtios;
	char buf[255];
	int c, res;
	//struct sigaction saio;
	
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(fd < 0) {
		perror(MODEMDEVICE);
		exit(-1);
	}

	/* saio.sa_handler = signal_handler_IO;
	sigemptyset(&saio.sa_mask);
	saio.sa_flags = 0;
	saio.sa_restorer = NULL;
	sigaction(SIGIO,&saio,NULL); */

	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, FASYNC);

	tcgetattr(fd, &oldtios);
	newtios.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtios.c_iflag = IGNPAR | ICRNL;
	newtios.c_oflag = 0;
	newtios.c_lflag = ICANON;
	newtios.c_cc[VMIN]=1;
	newtios.c_cc[VTIME]= 0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtios);

	/*while(STOP == FALSE){
		printf(".\n");
		usleep(100000);

		if(wait_flag==FALSE){
			res = read(fd, buf, 255);
			buf[res] = 0;
			printf(":%s:%d\n", buf, res);
			if(res==1) STOP==TRUE;
			wait_flag = TRUE;
		}
		tcsetattr(fd, TCSANOW, &oldtios);
	}*/
}

void signal_handler_IO(int status)
{
	printf("received SIGIO signal\n");
	wait_flag = FALSE;
}


int
SerialWrite(void* itemToSend){

	//char thisArray[] = {0x42,0xFF,0xFF,0xFF,0xFF,0x0D};

	return write(fd, itemToSend, sizeof(itemToSend));
}

int
ClosePort(){
	tcsetattr(fd, TCSANOW, &oldtios);
	close(fd);
}
