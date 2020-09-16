/************************************************************ dmonitor のログファイルよりラストハード及び 状況を取得し変数status に入れる
 ************************************************************/

#include "dmonitor.h"

int getstatus()
{
	FILE	*fp;
	char	*tmpptr;
	char	*getstatus	= "tail -n3 /var/log/dmonitor.log";
	char	tmpstr[32]	= {'\0'};
	char	line[128]	= {'\0'};
	char	command[32]	= {'\0'};
	char	mycall[8]	= {'\0'};
	char	mycallpre[8]	= {'\0'};
	char	tmpstat[32]	= {'\0'};

	/* コマンドの標準出力オープン */
	if ((fp = popen(getstatus, "r")) == NULL)
	{
		printf("File open error!\n");
		return (EXIT_FAILURE);
	}

	/* 標準出力を配列に取得 */
	while ((fgets(line, sizeof(line), fp)) != NULL)
	{
		/* 過去のデータをクリアする  */
		memset(&status[0], '\0', sizeof(status));
		memset(&tmpstr[0], '\0', sizeof(tmpstr));

		/* dmonitorへの信号がZRからかGW側からかを判断して status 代入の準備のみする */
		if ((tmpptr = strstr(line, "from ZR")) != NULL || (tmpptr = strstr(line, "from GW")) != NULL)
		{
			memset(&tmpstat[0], '\0', sizeof(tmpstat));

			/* MyCallsignの取得 */
			memset(&mycall[0], '\0', sizeof(mycall));
			strncpy(mycall, tmpptr - 9, 8);		// My Callsign

			/* MyCallsignが単なるループではない場合 */
			if (strncmp(mycall, mycallpre, 8) != 0)
			{
				strncpy(tmpstat, line, 12);      	// 日付時分
				strcat(tmpstat, " ");
				strncat(tmpstat, mycall, 8); 		// コールサイン
				strncat(tmpstat, tmpptr + 4, 3);	// ZR/GW
			}
		}

		/* rpt2, rpt1, ur, my の行が見つかったら,コールサインを照合して前段のtmpstatをstatusに代入 */
		if ((tmpptr = strstr(line, "my:")) != NULL)
		{
			if (strncmp(mycall, tmpptr + 3, 8) == 0) strcpy(status, tmpstat);
			stat = 0;
		}

		/* 無線機から送信したときのログを出力 */
		if ((tmpptr = strstr(line, "from Rig")) != NULL || (tmpptr = strstr(line, "from DVAP")) != NULL)
		{
			if (strncmp("Rig",  tmpptr + 5, 3) == 0) strncpy(tmpstr, " TM", 3);
			if (strncmp("DVAP", tmpptr + 5, 4) == 0) strncpy(tmpstr, " DV", 3);

			strncpy(status, line, 12);		// 日付時分
			strcat(status, " ");
			strncat(status, tmpptr - 9, 8);		// コールサイン
			strcat(status, tmpstr);			// Terminal-AP Mode/DVAP Mode
			stat = 0;
		}

		/* dmonitorの開始とバージョンを取得 */
		if ((tmpptr = strstr(line, "dmonitor start")) != NULL)
		{
			strncpy(status, tmpptr, 21);
			break;
		}

		/* どこに接続したかを取得 */
		if ((tmpptr = strstr(line, "Connected")) != NULL)
		{
			strncpy(rptcall, tmpptr + 13, 8);
		}

		/* Last packet wrong ステータスの場合、文字を黄色に */
		if ((stat == 1) && (debug == 1) && (strstr(line, "Last packet wrong") != NULL))
		{
			strcpy(status, "Last packet is wrong...");
		}

		/* バッファの拡張のサイズを取得 */
		if ((tmpptr = strstr(line, "New FiFo buffer")) != NULL)
		{
			strcpy(status, tmpptr + 9);
			status[strlen(status) - 1] = '\0';
		}

		/* 無線機の接続状況 */
		if ((debug == 1) && (strstr(line, "init/re-init") != NULL))
		{
			strcpy(status, "RIG initializing is done.");
		}

		/* DVAP使用時の周波数 */
		if ((debug == 1) && ((tmpptr = strstr(line, "Frequency Set")) != NULL))
		{
			strcpy(status, "DVAP FREQ. ");
			strncat(status, tmpptr + 14, 3);
			strcat(status, ".");
			strncat(status, tmpptr + 17, 3);
			strcat(status, " MHz");
			break;
		}

		/* UNLINKコマンドの処理 */
		if (strstr(line, "my2:UNLK") != NULL)
		{
			strcpy(status, "UNLINK FROM RIG");
		}

		/* 接続解除を取得 */
		if (strstr(line, "dmonitor end") != NULL)
		{
			strcpy(status, "Disconnected");
			strcpy(rptcall, "NONE");
		}

		/* ドロップパケット比の表示 */
		if ((debug == 1) && ((tmpptr = strstr(line, "drop packet")) != NULL))
		{
			strcpy(status, "Drop PKT ");
			strcat(status, tmpptr + 17);
			status[strlen(status) - 1] = '\0';
			stat = 1;
		}


	}
	pclose(fp);

	/* 接続先の表示*/
	if (((strncmp(rptcall, "J", 1) == 0) || (strncmp(rptcall, "NONE", 4) == 0)) && (strncmp(rptcall, rptcallpre, 8) != 0))
	{
		strncpy(rptcallpre, rptcall, 8);
		sprintf(command, "MAIN.t1.txt=\"LINK TO : %s\"", rptcall);
		sendcmd(command);
		sprintf(command, "MAIN.link.txt=\"LINK TO : %s\"", rptcall);
		sendcmd(command);
	}

	/* ステータス・ラストハードの表示 */
	if ((strlen(status) != 0) && (strncmp(status, statpre, 24) != 0))
	{
		strcpy(statpre, status);

		/* STATUS1 => STATUS2 */
		sendcmd("MAIN.stat2.txt=MAIN.stat1.txt");

		/* 取得ステイタス=> STATUS1 */
		sprintf(command, "MAIN.stat1.txt=\"%s\"", status);
		sendcmd(command);
		sendcmd("MAIN.t2.txt=MAIN.stat1.txt");
		sendcmd("MAIN.t3.txt=MAIN.stat2.txt");

		/* statusをクリアする */
		status[0] = '\0';
	}

	return (EXIT_SUCCESS);
}
