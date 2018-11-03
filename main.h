#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include "NextionDriver.h" //Copyright (C) 2017,2018 by Lieven De Samblanx ON7LDS

void sendCommand(char *cmd);
int open_nextion_serial_port(char* devicename, long BAUD);
void terminate(int sig);

