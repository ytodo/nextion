#include "main.h"


int main(int argc, char *argv[])
{

    fd1 = open_nextion_serial_port("/dev/ttyAMA0", 9600);

    int t, ok, wait;

    display_addr = 0;
    check = 1000;
    gelezen = 0;
    screenLayout = 2;

    become_daemon = TRUE;
    ok = 0;
    verbose = 0;


    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // terminate on these signals
    signal(SIGINT,  terminate);            // 'Ctrl-C'
    signal(SIGQUIT, terminate);            // 'Ctrl-\'
    signal(SIGHUP,  terminate);
    signal(SIGTERM, terminate);
    signal(SIGUSR1, terminate);
    signal(SIGUSR2, terminate);

    strcpy(nextionDriverLink, NEXTIONDRIVERLINK);
    strcpy(nextionPort, NEXTIONPORT);
    strcpy(configFile,  CONFIGFILE);
    strcpy(groupsFile,  GROUPSFILE);
    strcpy(usersFile,   USERSFILE);


    datafiledir[0] = 0;


    t = 0; wait = 0; int r = 0;
    #define     SERBUFSIZE 1024
    char buffer[SERBUFSIZE * 2];
    char* s;
    int   start = 0;

    sendCommand("page 0");
//    sendCommand("cls 0");
    sendCommand("dim=50");
    sendCommand("t0.txt=\"Nextionhelper\"");


    return EXIT_SUCCESS;
}

