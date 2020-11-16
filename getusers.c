#include "dmonitor.h"

int getusers(char ret[16])
{
	FILE    *fp;
	char    *users_tbl  = "/var/tmp/connected_table.html";
	char    line[64]    = {'\0'};
	char    tmpstr[32]  = {'\0'};
	char    *tmpptr;
	char    command[32] = {'\0'};
	int     i           = 0;
	int     j           = 0;

	/* 画面をUSERSに切替える */
	sendcmd("page USERS");
	sprintf(command, "USERS.b0.txt=\"LINKED USERS on %s\"", rptcallpre1);
	sendcmd(command);

	/* [Return]ボタンが押されるまで繰り返す */
	while (1)
	{
		/* 既存ステイタスの表示 */
		sendcmd("t8.txt=MAIN.stat1.txt");
		sendcmd("t9.txt=MAIN.stat2.txt");

		/* [Return]の検出 */
		recvdata(ret);
		if ((strncmp(ret, "Return", 6)) == 0)
		{
			ret[6] = '\0';
			break;
		}

		/* コマンドの標準出力オープン */
		if ((fp = fopen(users_tbl, "r")) == NULL)
		{
			printf("File open error!\n");
			return (EXIT_FAILURE);
		}

		/* ファイルを一行ずつ読み最終行まで繰り返す  */
		i = 0;
		while ((fgets(line, sizeof(line), fp)) != NULL)
		{
			/* <td> を見つけ内容を取得し、リストに表示 */
			if ((tmpptr = strstr(line, "<td>")) != NULL)
			{
				strncpy(tmpstr, tmpptr + 4, 8);
				sprintf(command, "USERS.t%d.txt=\"%s\"", i, tmpstr);
				sendcmd(command);
				i++;
				if (i >= 8) break;
        		}
        	}

	        /* ファイルクローズ */
		fclose(fp);

        	/* リスト8件に満たない場合、残りをクリアする */
		if (i < 8 )
		{
			for (j = i; j < 8; j++)
			{
				sprintf(command, "USERS.t%d.txt=\" \"", j);
				sendcmd(command);
			}
		}

		/* ステータス・ラストハードの表示 */
		getstatus();

		/* 1秒に一回リフレッシュする */
		sleep(1);
	}
	sendcmd("page MAIN");

	return(EXIT_SUCCESS);
}
