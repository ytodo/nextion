/********************************************************
 *                                                      *
 *  サーバよりmulti_forward のインストールされた        *
 *  ゲートウェイリストをダウンロードして配列を作成する。*
 *                                                      *
 ********************************************************/

#include "dmonitor.h"


int getlinkdata()
{
	char		*strcall;
	char		*straddr;
	char		*strport;
	char		*RPTLISTTBL = "/tmp/repeaters.tbl";
	char		line[128] = {'\0'};
	int		    i = 0;
	FILE		*fp;

	/* d-star.info からリピータリストを取得してテーブルを作成する */
	char *getrptlist = "cd /tmp && wget -q http://hole-punch.d-star.info:30011 && mv index.html repeaters.tbl";
	system(getrptlist);

	/* テーブルをオープンする */
	if ((fp = fopen(RPTLISTTBL, "r")) == NULL) {
		printf("File open error!\n");

		return (EXIT_FAILURE);
	}

/* File sample
{"Connected Table":[
{"callsign":"JK1ZRW  ","ip_address":"183.177.205.139","port":51000},
{"callsign":"JL3ZBS  ","ip_address":"111.64.23.93","port":51000},
{"callsign":"JP0YDP  ","ip_address":"219.117.234.39","port":1024},
{"callsign":"JP1YDS  ","ip_address":"210.239.249.164","port":51000},
{"callsign":"JP1YEM  ","ip_address":"119.173.54.159","port":51000},
*/

	/* テーブルを読み込み構造体に格納する */
	while ((fgets(line, sizeof(line), fp)) != NULL) {

		/* Find out Callsing, IP Address and Port number */
		if ((strcall = strstr(line, "callsign")) != NULL ) {
			 straddr = strstr(line, "ip_address");
			 strport = strstr(line, "port");

			/* コールサインの保存 */
			strncpy(linkdata[i].call, strcall + 11, 8);

			/* 第八番目もじ（拡張子）が空白だったら「Ａ」とする */
			if (strncmp(" ", &linkdata[i].call[7], 1) == 0) {
				strncpy(&linkdata[i].call[7], "A", 1);
			}
			linkdata[i].call[8] = '\0';

			/* ＩＰアドレスの保存 */
			strncpy(linkdata[i].addr, straddr + 13, strlen(straddr) - strlen(strport) - 16);
			linkdata[i].addr[strlen(straddr) - strlen(strport) - 16] = '\0';

			/* ポート番号の保存*/
			strncpy(linkdata[i].port, strport + 6, 5);
			linkdata[i].port[5] = '\0';

			i++;
		}
	}

	fclose(fp);

	/* Test */
//	int j = 0;
//	for (j = 0; j < i; j++) {
//		printf("%2d %s %s %s\n", j + 1, linkdata[j].call, linkdata[j].addr, linkdata[j].port);
//	}

    /* 軒数を返す */
	return (i);
}
