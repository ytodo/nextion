#include "dmonitor.h"


int main(int argc, char *argv[])
{
	int 	num;				// 返り値のi を受ける（件数）
	int	arraycount;
	int	fd;
	int	i;
	int	flag;
	char	command[32] 	= {'\0'};
	char	statpre[32] 	= {'\0'};
	char	rptcallpre[32] 	= {'\0'};
	char    *SERIALPORT 	= "/dev/ttyAMA0";
	char	concall[8]	= {'\0'};
	char	concallpre[8]	= {'\0'};


	/* 現在利用可能なリピータリストの取得*/
	num = getlinkdata();

	/* GPIO シリアルポートのオープン*/
	fd = openport(SERIALPORT, BAUDRATE);

	/* メインスクリーンの初期設定 */
	sendcmd("dim=50");
	sendcmd("page MAIN");
	sendcmd("t1.txt=\"LINK TO : NONE\"");
	sendcmd("link.txt=\"LINK TO : NONE\"");

	/* 読み込んだリピータの総数を表示 */
	sprintf(command, "MAIN.stat1.txt=\"Read %d Repeaters\"", num);
	sendcmd(command);
	sendcmd("t2.txt=MAIN.stat1.txt");

	/* リピータ数分の文字配列にコールサインを格納 */
	for (i = 0; i < num; i++) {
		sprintf(command, "VALUE.va%d.txt=\"%s\"", i, linkdata[i].call);
		sendcmd(command);
	}

	/* 送・受信ループ */
	while (1) {

		/*
		 * 受信処理
		 */

		/* タッチパネルのデータを読み込む */
		recvdata(concall);
		if ((strlen(concall) > 3) && (strncmp(concall, concallpre, 8) != 0))  {

			/* 現在の返り値を保存 */
			strncpy(concallpre, concall, 8);

			/* コマンドをスイッチに振り分ける */
			if (strncmp(concall, "Restart", 7) == 0) flag = 1;
			if (strncmp(concall, "Reboot",  6) == 0) flag = 2;
			if (strncmp(concall, "Shutdown",8) == 0) flag = 3;
			switch (flag) {
			case 1:
				sprintf(command, "systemctl restart nextion.service");
				sendcmd("page MAIN");
				system(command);
				break;
			case 2:
				sprintf(command, "reboot");
				sendcmd("page MAIN");
				system(command);
				break;
			case 3:
				sprintf(command, "shutdown -h now");
				sendcmd("page MAIN");
				system(command);
				break;
			default:
				/* 指定リピータに接続する */
				i = 0;
				num = getlinkdata();
				for (i = 0; i < num; i++) {
					if (strncmp(linkdata[i].call, concall, 8) == 0) {
						/* 現在稼働中のdmonitor を止める */
						system("killall -q -s 9 dmonitor");

						/* 接続コマンドの実行 */
						sprintf(command, "dmonitor %s %s '%s'", linkdata[i].addr, linkdata[i].port, linkdata[i].call);
						sendcmd("page MAIN");
						system(command);
						break;
					}
				}
			}
		}

		/*
		 * 送信処理
		 */

		/* ステータス・ラストハードの読み取り */
		getstatus();

		/* 接続先の表示*/
		if ((strncmp(rptcall, "", 1) != 0) && (strncmp(rptcall, rptcallpre, 8) !=0)) {
			strcpy(rptcallpre, rptcall);
			sprintf(command, "t1.txt=\"LINK TO : %s\"", rptcall);
			sendcmd(command);
			sprintf(command, "MAIN.link.txt=\"LINK TO : %s\"", rptcall);
			sendcmd(command);
		}

		/* ステータス・ラストハードの表示 */
		if ((strncmp(status, "", 1) != 0) && (strncmp(status, statpre, 24) != 0)) {
			strcpy(statpre, status);
			sendcmd("dim=50");
			sendcmd("stat2.txt=MAIN.stat1.txt");
			sprintf(command, "MAIN.stat1.txt=\"%s\"", status);
			sendcmd(command);
			sendcmd("t2.txt=MAIN.stat1.txt");
			sendcmd("t3.txt=MAIN.stat2.txt");
		}
	}

	/* GPIO シリアルポートのクローズ*/
	close(fd);
	return (EXIT_SUCCESS);
}

