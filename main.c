#include "dmonitor.h"

char    *SERIALPORT     = "/dev/ttyAMA0";

int main(int argc, char *argv[])
{
	int 	num;				// 返り値のi を受ける（件数）
	int	fd;
	int	i = 0;
	char	command[32] = {'\0'};

	/* 現在利用可能なリピータリストの取得*/
	num = getlinkdata();

	/* GPIO シリアルポートのオープン*/
	fd = openport(SERIALPORT, BAUDRATE);

	/* メインスクリーンの初期設定 */
	sendcmd("dim=100");
	sendcmd("page MAIN");
	sendcmd("t0.xcen=\"Left\"");
	sendcmd("t0.txt=\"LINK TO :         \"");
	sprintf(command, "t2.txt=\"Read %d RPTs\"", num);
	sendcmd(command);

	/* 1ページ分21ヶ所分を表示 */
	for (i = 0; i < num; i++) {
		sprintf(command, "MAIN.va%d.txt=\"%s\"", i, linkdata[i].call);
		sendcmd(command);
	}

	/* GPIO シリアルポートのクローズ*/
	close(fd);
	return (EXIT_SUCCESS);
}

