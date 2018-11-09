#include "dmonitor.h"

char    *SERIALPORT     = "/dev/ttyAMA0";

int main(int argc, char *argv[])
{
	/* 現在利用可能なリピータリストの取得*/
	int num;				// 返り値のi を受ける（件数）
	num = getlinkdata();

	/* GPIO シリアルポートのオープン*/
	int	fd;
	fd = openport(SERIALPORT, BAUDRATE);

	/* test */
	int	i = 0;
	for (i = 0; i < num; i++) {
		printf("%2d : %s %s %s\n", i + 1, linkdata[i].call, linkdata[i].addr, linkdata[i].port);
	}

	/* リピータリストの表示*/
	i 	= 0;
	int	j = 0;
	char	command[9];
	char	pagenum[9];

	/* 明るさを100%に設定 */
	sendcmd("dim=50");


	/* 3ページ分を切り替える */
	for (j = 0; j < 3; j++) {
		sprintf(pagenum, "page %d", j + 1);
		sendcmd(pagenum);

		/* 1ページ分21ヶ所分を表示 */
		for (i = 0; i < 21; i++) {
			sprintf(command, "t%d.txt=\"%s\"", i + 100, linkdata[i + (j * 21)].call);
			sendcmd(command);
		}
	}

	/* GPIO シリアルポートのクローズ*/
	close(fd);
	return (EXIT_SUCCESS);
}

