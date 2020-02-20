/********************************************************
	サーバよりmulti_forward のインストールされた        
	ゲートウェイリストをダウンロードして配列を作成する。
 ********************************************************/

#include "dmonitor.h"

int getlinkdata()
{
	char		*strcall;
	char		*straddr;
	char		*strport;
	char		*strzone;
	char		*RPTLISTTBL = "/var/tmp/repeater_mon.html";
	char		line[512] = {'\0'};
	int			i = 0;
	FILE		*fp;

	/* d-star.info からリピータリストを取得してテーブルを作成する */
//	char *getrptlist = "cd /tmp && wget -q http://hole-punch.d-star.info:30011 && mv index.html repeaters.tbl";
//	system(getrptlist);

	/* テーブルをオープンする */
	if ((fp = fopen(RPTLISTTBL, "r")) == NULL)
	{
		printf("File open error!\n");
		return (i);
	}

/* File sample
<a name="area3"></a>3エリア<br>
<table border="1" style="font-size:0.9em;">
<tr><td><tt><a href="/cgi-bin/monitor?ip_addr=121.94.194.18&port=51000&callsign='JL3ZBS A'&zr_call='JL3ZBS G'" target="cmd1">JL3ZBS&nbsp;A</a></tt></td><td></td>
<td><tt><a href="/cgi-bin/monitor?ip_addr=153.248.246.39&port=51000&callsign='JP3YCO A'&rep_name='姫路奥山430'&zr_call='JP3YCO G'" target="cmd1">JP3YCO&nbsp;A</a></tt></td><td>姫路奥山430</td>
*/

	/* テーブルを読み込み構造体に格納する */
	while ((fgets(line, sizeof(line), fp)) != NULL)
	{
		/* Find out Callsign, IP Address and Port number */
		if ((strcall = strstr(line, "callsign")) != NULL )
		{
			straddr  = strstr(line, "ip_addr");
			strport  = strstr(line, "port");
			strzone  = strstr(line, "zr_call");

			/* コールサインの保存 */
			strncpy(linkdata[i].call, strcall + 10, 8);

			/* 第八番目もじ（拡張子）が空白だったら「Ａ」とする */
			if (strncmp(" ", &linkdata[i].call[7], 1) == 0)
			{
				strncpy(&linkdata[i].call[7], "A", 1);
			}
			linkdata[i].call[8] = '\0';

			/* ＩＰアドレスの保存 */
			strncpy(linkdata[i].addr, straddr + 8, strlen(straddr) - strlen(strport) - 9);
			linkdata[i].addr[strlen(straddr) - strlen(strport) - 9] = '\0';

			/* ポート番号の保存 */
			strncpy(linkdata[i].port, strport + 5, 5);
			linkdata[i].port[5] = '\0';

			/* ZR_CALLの取得 */
			strncpy(linkdata[i].zone, strzone + 9, 8);
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
