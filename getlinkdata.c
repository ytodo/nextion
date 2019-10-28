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
    char        *strzone;
	char		*RPTLISTTBL = "/tmp/repeaters.tbl";
	char		line[256] = {'\0'};
	int		    i = 0;
	FILE		*fp;

	/* d-star.info からリピータリストを取得してテーブルを作成する */
	char *getrptlist = "cd /tmp && wget -q http://hole-punch.d-star.info:30011 && mv index.html repeaters.tbl";
	system(getrptlist);

	/* テーブルをオープンする */
	if ((fp = fopen(RPTLISTTBL, "r")) == NULL) {
		printf("File open error!\n");

		return (i);
	}

/* File sample
{"callsign":"JL3ZBS A","ip_address":"222.159.237.250","port":51000,"status":"off", "area":"3",  |
            "ur_call":"        ","my_call":"        ","rpt1_call":"        ","rpt2_call":"        ","zr_call":"JL3ZBS G"},
*/

	/* テーブルを読み込み構造体に格納する */
	while ((fgets(line, sizeof(line), fp)) != NULL) {

		/* Find out Callsing, IP Address and Port number */
		if ((strcall = strstr(line, "callsign")) != NULL ) {
			straddr  = strstr(line, "ip_address");
			strport  = strstr(line, "port");
            strzone  = strstr(line, "zr_call");

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

			/* ポート番号の保存 */
			strncpy(linkdata[i].port, strport + 6, 5);
			linkdata[i].port[5] = '\0';

            /* ZR_CALLの取得 */
            strncpy(linkdata[i].zone, strzone + 10, 8);
            linkdata[i].zone[8] = '\0';

			i++;
		}
	}

	fclose(fp);

	/* Test */
//	int j = 0;
//	for (j = 0; j < i; j++) {
//		printf("%2d %s %s %s\n", j + 1, linkdata[j].call, linkdata[j].addr, linkdata[j].port, linkdata[j].zone);
//	}

    /* 軒数を返す */
	return (i);
}
