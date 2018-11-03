#include "NextionDriver.h"
#include "main.h"
const char ENDMRKR[3]="\xFF\xFF\xFF";

void sendCommand(char *cmd)
{
    if (strlen(cmd)>0) {
        write(fd1, cmd ,strlen(cmd));
        write(fd1, ENDMRKR, 3);
        printf("\n");
        usleep(5);
    }
    if (!become_daemon) fflush(NULL);
}

int open_nextion_serial_port(char* devicename, long BAUD)
{
    int         fd;
    struct      termios newtio;


    fd = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (fd < 0) {
        perror(devicename);
        exit(EXIT_FAILURE);
    }

    tcgetattr(fd, &newtio);

    cfsetispeed(&newtio, BAUD);
    cfsetospeed(&newtio, BAUD);

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
    if((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
        exit(EXIT_FAILURE);
    }

//    printf("port %s (fd %i)\n", devicename, fd);

    return fd;
}



void terminate(int sig)
{
    char *signame[]={"INVALID", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGPOLL", "SIGPWR", "SIGSYS", NULL};

    close(fd1);
    close(fd2);
    unlink(nextionDriverLink);
    exit(0);
}

