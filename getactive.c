#include "dmonitor.h"

int getactive()
{
	FILE    *fp;
//	char    *active_tbl  = "/var/tmp/repeater_active.html";
	char    *active_tbl  = "/var/www/html/repeater_active.html";
	char    *tmpptr;
	char    line[64]    = {'\0'};
	char    tmpstr[32]  = {'\0'};
	char    command[32] = {'\0'};
	char    statpre[32] = {'\0'};
	char    ret[16]     = {'\0'};
	int     i           = 0;
	int     j           = 0;

	/* [Return]ボタンが押されるかリピータが選択されるまで繰り返す */
	while (1)
	{
		/* [Return]又は接続リピータの検出 */
		recvdata(ret);

		if (    (strncmp(ret, "J", 1)) == 0 || (strncmp(ret, "Return", 6)) == 0   )
		{
			strcpy(concall, ret);
			break;
		}

		/* コマンドの標準出力オープン */
		if ((fp = fopen(active_tbl, "r")) == NULL)
		{
			printf("File open error!\n");
			return (EXIT_FAILURE);
		}

		/* ファイルを一行ずつ読み最終行まで繰り返す  */
		i = 0;
		while ((fgets(line, sizeof(line), fp)) != NULL)
		{
			/* callsign を見つけ内容を取得し、リストに表示 */
			if ((tmpptr = strstr(line, "callsign")) != NULL)
			{
				strncpy(tmpstr, tmpptr + 10, 8);
				sprintf(command, "ACTIVE.t%d.txt=\"%s\"", i, tmpstr);
				sendcmd(command);
				i++;
				if (i >= 18) break;
        		}
        	}

		/* ファイルクローズ */
		fclose(fp);

		 	/* リスト8件に満たない場合、残りをクリアする */
		if (i < 18 )
		{
			for (j = i; j < 18; j++)
			{
				sprintf(command, "ACTIVE.t%d.txt=\" \"", j);
				sendcmd(command);
			}
		}

		/* 2秒に一回リフレッシュする */
		sleep(2);
	}

	return(0);
}
