#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

#define	SERIALPORT	"/dev/ttyAMA0"
#define	BAUDRATE	9600

int openport(char *devicename, long baudrate);
void sendcmd(char *cmd);

