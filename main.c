#include "main.h"


int main(int argc, char *argv[])
{
	int	fd;

	fd = openport(SERIALPORT, BAUDRATE);

	sendcmd("page 0");
//	sendcmd("cls 0");
	sendcmd("dim=100");
	sendcmd("t0.txt=\"Nextionhelper\"");

	close(fd);
	return EXIT_SUCCESS;
}

