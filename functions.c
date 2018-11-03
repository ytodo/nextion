#include "main.h"

/*********************************************
 * シリアルポートのオープン
 *********************************************/
int openport(char *devicename, long baudrate)
{
	int	fd;
	struct	termios newtio;

	fd = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);

	// Port Open Error -> Exit
	if (fd < 0) {
	        perror(devicename);
		exit(EXIT_FAILURE);
	}

	tcgetattr(fd, &newtio);

	cfsetispeed(&newtio, baudrate);
	cfsetospeed(&newtio, baudrate);

	newtio.c_cflag &= ~PARENB;          // geen pariteit
	newtio.c_cflag &= ~CSTOPB;          // 1 stopbit
	newtio.c_cflag &= ~CSIZE;           // wis databitsize
	newtio.c_cflag |=  CS8;             // 8 databits
	newtio.c_cflag &= ~CRTSCTS;         // geen HW flow Control
	newtio.c_cflag |=  CREAD | CLOCAL;

	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;

	tcflush(fd, TCIOFLUSH);
	if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
		exit(EXIT_FAILURE);
	}

	return (fd);
}


/*********************************************
 * コマンドの送信
 *********************************************/
void sendcmd(char *cmd)
{
	int	fd;
	char	endofcmd[3] = {0xff, 0xff, 0xff};

	if (strlen(cmd) > 0) {
	        write(fd, cmd ,strlen(cmd));
        	write(fd, endofcmd, 3);
	}
}
