/***************************************************************
	基本的なファンクション・コマンドのツールボックス
		openport
		sendcmd
		recvdata
		getconfig
 ***************************************************************/
#include "dmonitor.h"

int	fd;
int	i = 0;
int	j = 0;
int	len = 0;
char	buf[32] = {'\0'};

/*********************************************
	シリアルポートのオープン
	This is from ON7LDS's NextionDriver
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

	cfsetispeed(&newtio,baudrate);
	cfsetospeed(&newtio,baudrate);

	newtio.c_cflag &= ~PARENB;
	newtio.c_cflag &= ~CSTOPB;          // ストップビット   : 1bit
	newtio.c_cflag &= ~CSIZE;           // データビットサイズ
	newtio.c_cflag |=  CS8;             // データビット     : 8bits

	newtio.c_cflag &= ~CRTSCTS;
	newtio.c_cflag |= CREAD | CLOCAL;   // 受信有効｜ローカルライン（モデム制御無し）

	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;

	tcflush(fd, TCIOFLUSH);

	if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
	{
		exit(EXIT_FAILURE);
	}

	ioctl(fd, TCSETS, &newtio); 		//ポートの設定を有効にする
	return (fd);
}


/*********************************************
	コマンドの送信
 *********************************************/
void sendcmd(char *cmd)
{
	/* コマンドの終了を示す文字列 */
	char	endofcmd[3] = {0xff, 0xff, 0xff};

	/* コマンド文字列と終了文字列を書き込む */
	if (strlen(cmd) > 0)
	{
		write(fd, cmd, strlen(cmd));
		write(fd, endofcmd, 3);
	}
	return;
}


/*********************************************
	コマンドの受信
 *********************************************/
void recvdata(char *rptcon)
{
	/* GPIO RxD のASCIIデータを長さ分読み込む */
	len = read(fd, buf, sizeof(buf));
	if (0 < len)
	{
		for (i = 0; i < len; i++)
		{
			if (buf[i] >= 30 && buf[i] <= 122)
			{
	 			sprintf(&rptcon[i], "%c", buf[i]);
				j++;
			}
		}
		rptcon[i] = '\0';
	}
}


/*********************************************
	環境設定ファイルnextion.ini の内容を読む
 *********************************************/
int	getconfig(void)
{
	char	*INIFILE = "/etc/nextion.ini";
	char	*RIGFILE = "/var/www/dmonitor.conf";
	char	line[64] = {'\0'};
	int	i 	 = 0;
	FILE	*fp;
	char	*ret;

	/* nextion.iniファイルをオープンする */
	if ((fp = fopen(INIFILE, "r")) == NULL)
	{
		printf("nextion.ini file open error!\n");
		return (EXIT_FAILURE);
	}
	/* テーブルを読み込み変数に格納する */
	while ((fgets(line, sizeof(line), fp)) != NULL)
	{
		if ((ret = strstr(line, "STATION"))     != NULL) sscanf(line, "STATION=%[^\n]",     station);
		if ((ret = strstr(line, "DEFAULT_RPT")) != NULL) sscanf(line, "DEFAULT_RPT=%[^\n]", default_rpt);
		if ((ret = strstr(line, "PORT"))	!= NULL) sscanf(line, "PORT=%[^\n]",	    nextion_port);
		if ((ret = strstr(line, "SLEEPTIME"))   != NULL) sscanf(line, "SLEEPTIME=%d",       &microsec);
		if ((ret = strstr(line, "DEBUG"))       != NULL) sscanf(line, "DEBUG=%d",           &debug);
	}
	fclose(fp);


	/* rig.typeファイルをオープンする */
	if ((fp = fopen(RIGFILE, "r")) == NULL)
	{
		printf("dmonitor.conf file open error!\n");
	}
	/* 内容を読込み変数に代入する */
	while ((fgets(line, sizeof(line), fp)) != NULL)
	{
		sscanf(line, "%[^\n]", rigtype);
		if ((strncmp(rigtype, "ICOM",   4) == 0) ||
		    (strncmp(rigtype, "DVAP",   4) == 0) ||
		    (strncmp(rigtype, "DVMEGA", 6) == 0) ||
		    (strncmp(rigtype, "NODE",   4) == 0)) break;
	}
	fclose(fp);

	return (0);
}
