
/*
 *  Copyright (C) 2018-2019 by Yosh Todo JE3HCZ
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  本プログラムはD-STAR Network の一部の機能を実現するための物で、
 *  アマチュア無線の技術習得とその本来の用途以外では使用しないでください。
 *
 */


#include "dmonitor.h"


int main(int argc, char *argv[])
{
	int 	num;				// 返り値のi を受ける（件数）
	int	    arraycount;
	int	    fd;
	int	    i;
	int	    flag;
	char	command[32] 	= {'\0'};
	char	statpre[32] 	= {'\0'};
	char	rptcallpre[32] 	= {'\0'};
	char    *SERIALPORT 	= "/dev/ttyAMA0";
	char	concall[8]	    = {'\0'};
	char	concallpre[8]	= {'\0'};

    /* 環境設定ファイルの読み取り */
    getconfig();

	/* 現在利用可能なリピータリストの取得*/
	num = getlinkdata();

	/* GPIO シリアルポートのオープン*/
	fd = openport(SERIALPORT, BAUDRATE);

	/* メインスクリーンの初期設定 */
	sendcmd("dim=50");
	sendcmd("page MAIN");
    sprintf(command, "MAIN.station.txt=\"STATION : %s\"", station);
    sendcmd(command);
    sprintf(command, "t0.txt=\"STATION : %s\"", station);
    sendcmd(command);
  	sendcmd("t1.txt=\"LINK TO : NONE\"");
    sendcmd("link.txt=\"LINK TO : NONE\"");

	/* 読み込んだリピータの総数を表示 */
	sprintf(command, "MAIN.stat1.txt=\"Read %d Repeaters\"", num);
	sendcmd(command);
	sendcmd("t2.txt=MAIN.stat1.txt");

	/* リピータ数分の文字配列にコールサインを格納 */
    if (num > 229) num = 229;
  	for (i = 0; i < num; i++) {
       	sprintf(command, "VALUE.va%d.txt=\"%s\"", i, linkdata[i].call);
        sendcmd(command);
        usleep(microsec);
   	}

	/* 送・受信ループ */
	while (1) {

		/*
		 * 受信処理
		 */

		/* タッチパネルのデータを読み込む */
		recvdata(concall);

        /* もしタッチデータが選択されていない場合、初回のみデフォルトリピータをセットする */
        if ((strlen(concall) == 0) && (strlen(default_rpt) != 0)) {
            strcpy(concall, default_rpt);
            default_rpt[0] = '\0';
        }

        /* タッチデータが選択されている場合、前回と同じかチェック（同じならパス） */
		if ((strlen(concall) > 3) && (strncmp(concall, concallpre, 8) != 0))  {

			/* 現在の返り値を保存 */
			strncpy(concallpre, concall, 8);

			/* コマンドをスイッチに振り分ける */
			if (strncmp(concall, "Restart", 7) == 0) flag = 1;
			if (strncmp(concall, "Reboot",  6) == 0) flag = 2;
			if (strncmp(concall, "Shutdown",8) == 0) flag = 3;
			switch (flag) {
			case 1:
                sendcmd("page MAIN");
				system("systemctl restart nextion.service");
				break;

			case 2:
				sendcmd("page MAIN");
				system("shutdown -r now");
				break;

			case 3:
				sendcmd("page MAIN");
				system("shutdown -h now");
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

            /* STATUS1 => STATUS2 */
			sendcmd("MAIN.stat2.txt=MAIN.stat1.txt");

            /* 取得ステイタス=> STATUS1 */
			sprintf(command, "MAIN.stat1.txt=\"%s\"", status);
			sendcmd(command);
			sendcmd("t2.txt=MAIN.stat1.txt");
			sendcmd("t3.txt=MAIN.stat2.txt");
		}

	sleep (0);
	}

	/* GPIO シリアルポートのクローズ*/
	close(fd);
	return (EXIT_SUCCESS);
}

