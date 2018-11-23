#include "dmonitor.h"


int	fd;
int	i = 0;
int	len = 0;
char	buf[32] = {'\0'};

/*********************************************
 * シリアルポートのオープン
 *********************************************/
int openport(char *devicename, long baudrate)
{
	struct	termios newtio;

	fd = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);

	// ポートが開けない時戻る
	if (fd < 0) {
		printf("Port Open Error\n");
		return(EXIT_FAILURE);
	}

	tcgetattr(fd, &newtio);

/* test 1 */
cfsetispeed(&newtio,baudrate);
cfsetospeed(&newtio,baudrate);

newtio.c_cflag &= ~PARENB;   // geen pariteit
newtio.c_cflag &= ~CSTOPB;   // 1 stopbit
newtio.c_cflag &= ~CSIZE;    // wis databitsize
newtio.c_cflag |=  CS8;      // 8 databits

newtio.c_cflag &= ~CRTSCTS;  // geen HW flow Control
newtio.c_cflag |= CREAD | CLOCAL;

newtio.c_iflag = 0;
newtio.c_oflag = 0;
newtio.c_lflag = 0;


tcflush(fd, TCIOFLUSH);

/* test1 end */

/* test2 
	newtio.c_cflag += CREAD; 		// 受信有効
	newtio.c_cflag += CLOCAL; 		// ローカルライン（モデム制御なし）
	newtio.c_cflag += CS8; 			// データビット  : 8bit
	newtio.c_cflag += 0; 			// ストップビット: 1bit
	newtio.c_cflag += 0; 			// パリティ      : None

	cfsetispeed(&newtio, baudrate);
	cfsetospeed(&newtio, baudrate);
	cfmakeraw(&newtio); 			// RAWモード

	tcflush(fd, TCIOFLUSH);
 test 2 end */

	if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
		exit(EXIT_FAILURE);
	}

// test1
//	ioctl(fd, TCSETS, &newtio); 		//ポートの設定を有効にする

	return (fd);
}



/*********************************************
 * コマンドの送信
 *********************************************/
void sendcmd(char *cmd)
{
	/* コマンドの終了を示す文字列 */
	char	endofcmd[3] = {0xff, 0xff, 0xff};

	/* コマンド文字列と終了文字列を書き込む */
	if (strlen(cmd) > 0) {
	        write(fd, cmd ,strlen(cmd));
        	write(fd, endofcmd, 3);
	}
}


/*********************************************
 * コマンドの受信
 *********************************************/
void recvdata(char *rptcon)
{
	/* GPIO RxD のASCIIデータを長さ分読み込む */
	len = read(fd, buf, sizeof(buf));
	if (0 < len) {
		for (i = 0; i < len; i++) {
			sprintf(&rptcon[i], "%c", buf[i]);
		}
		rptcon[i] = '\0';
	}
}


