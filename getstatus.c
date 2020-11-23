#include "dmonitor.h"

/*********************************************************************
 dmonitor のログファイルよりラストハード及び状況を取得し変数status に入れる
 *********************************************************************/
int getstatus()
{
	FILE	*fp;
	char	*tmpptr;
	char	*getstatus	= "tail -n3 /var/log/dmonitor.log";
	char	line[128]	= {'\0'};
	char	tmpstr[32]	= {'\0'};
	char	command[64]	= {'\0'};
	char	mycall[8]	= {'\0'};
	char	mycallpre[8]	= {'\0'};
	char	stat[5]		= {'\0'};

	/* コマンドの標準出力オープン */
	if ((fp = popen(getstatus, "r")) == NULL)
	{
		printf("File open error!\n");
		return (EXIT_FAILURE);
	}

	/* ファイル内容を配列に取得 */
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		/* 過去のデータをクリアする  */
		memset(status, '\0', sizeof(status));
		memset(tmpstr, '\0', sizeof(tmpstr));

		/* <1>どこに接続したかを取得 */
		if ((tmpptr = strstr(line, "Connected")) != NULL)
		{
			rptcall[0] = '\0';
			strncpy(rptcall, tmpptr + 13, 8);
			disp_rpt();
		}

		/* <2>dmonitorの開始とバージョンを取得 */
		if ((tmpptr = strstr(line, "dmonitor start")) != NULL)
		{
			strncpy(status, tmpptr, 21);
			disp_stat();
		}

		/* <3>dmonitorへの信号がZRからかGW側からかを判断して status 代入の準備のみする */
		if ((tmpptr = strstr(line, "from ZR")) != NULL || (tmpptr = strstr(line, "from GW")) != NULL)
		{
			/* MyCallsignの取得 */
			memset(mycall, '\0', sizeof(mycall));
			strncpy(mycall, tmpptr - 9, 8);         // My Callsign

			/* MyCallsignが単なるループではない場合 */
			if (strncmp(mycall, mycallpre, 8) != 0)
			{
				strncpy(status, line, 12);             // 日付時分
				strcat(status, " ");
				strncat(status, mycall, 8);            // コールサイン
				strncat(status, tmpptr + 4, 3);        // ZR/GW
			}
			disp_stat();
		}

		/* <4>無線機から送信したときのログを出力 */
		if ((tmpptr = strstr(line, "from Rig"   )) != NULL ||
		    (tmpptr = strstr(line, "from DVAP"  )) != NULL ||
		    (tmpptr = strstr(line, "from DVMEGA")) != NULL)
		{
			if (strncmp("Rig",  tmpptr + 5, 3) == 0)
			{
				if (strncmp(rigtype, "ICOM", 4) == 0)
			 	{
					strncpy(tmpstr, " TM", 3);
				} else {
					strncpy(tmpstr, " RF", 3);
				}
			} else {
				strncpy(tmpstr, " RF", 3);
			}
			strncpy(status, line, 12);              // 日付時分
			strcat(status, " ");
			strncat(status, tmpptr - 9, 8);         // コールサイン
			strcat(status, tmpstr);                 // Terminal-AP Mode/DVAP/DVMEGA/Node
			disp_stat();

		}

		/* <5>無線機の接続状況 */
		if ((debug == 1) && (((tmpptr = strstr(line, "Frequency Set")) != NULL)
				 || (strstr(line, "RIG(ID-xxPlus) init") != NULL)))
		{
			/* RFを使う場合には周波数表示で初期化表示とする */
			if (strstr(line, "DVAP"))   strcpy(status, "DVAP FREQ. ");
			if (strstr(line, "DVMEGA")) strcpy(status, "DVMEGA FREQ. ");

			/* ケーブル接続の場合はポートの初期化を表示する */
			if (strcmp(rigtype, "ICOM") == 0)
			{
				strcpy(status, "RIG initializing is done.");
			} else {
				strncat(status, tmpptr + 14, 3);
				strcat(status, ".");
				strncat(status, tmpptr + 17, 3);
				strcat(status, " MHz");
			}
			disp_stat();
		}

		/* <6>Last packet wrong ステータスの場合、文字を黄色に */
		if ((debug == 1) && (strstr(line, "Last packet wrong") != NULL))
		{
			strcpy(status, "Last packet is wrong...");
			disp_stat();
		}

		/* <7>バッファの拡張のサイズを取得 */
		if ((tmpptr = strstr(line, "New FiFo buffer")) != NULL)
		{
			strcpy(status, tmpptr + 9);
			status[strlen(status) - 1] = '\0';
			disp_stat();
		}

		/* <8>UNLINKコマンドの処理 */
		if ((strstr(line, "my2:UNLK") != NULL) || (strstr(line, "UNLINK   from Rig") != NULL))
		{
			strcpy(status, "UNLINK FROM RIG");
			disp_stat();
		}

		/* <9>接続解除を取得 */
		if (strstr(line, "dmonitor end") != NULL)
		{
			rptcall[0] = '\0';
			strcpy(rptcall, "NONE");
			disp_rpt();
			strcpy(status, "Disconnected");
			disp_stat();
		}

		/* <10>ドロップパケット比の表示 */
		if ((debug == 1) && ((tmpptr = strstr(line, "drop packet")) != NULL))
		{
			strcpy(status, "Drop PKT ");
			strcat(status, tmpptr + 17);
			status[strlen(status) - 1] = '\0';
			disp_stat();
		}

		/* <11>SCAN中の接続リピータを表示 */
		if ((tmpptr = strstr(line, "ZR area")) != NULL)
		{
			strncpy(rptcall, tmpptr + 8, 8);
			disp_rpt();
		}
	}

	/* 標準出力クローズ */
	pclose(fp);
	return (EXIT_SUCCESS);
}


/*********************************************************************
 変数rptcall の内容をNextionに表示する関数
 *********************************************************************/
int disp_rpt()
{
	char command[64] = {'\0'};

	/* rptcallが設定されており且つ内容が前回と異なる場合表示する */
	if (strcmp(rptcall, rptcallpre1) != 0)
	{
		strcpy(rptcallpre1, rptcall);

		sprintf(command, "MAIN.t1.txt=\"LINK TO : %s\"", rptcall);
		sendcmd(command);
		sprintf(command, "MAIN.link.txt=\"LINK TO : %s\"", rptcall);
		sendcmd(command);
	}
	return(0);
}


/*********************************************************************
 変数status の内容をNextionに表示する関数
 *********************************************************************/
int disp_stat()
{
	char command[64] = {'\0'};

	if ((strcmp(status, statpre1) != 0) && (strcmp(status, statpre2) != 0) && (strcmp(status, statpre3) != 0))
	{
		strcpy(statpre3, statpre2);
		strcpy(statpre2, statpre1);
		strcpy(statpre1, status);

printf("%s\n", status);

		/* STATUS1 => STATUS2 */
		sendcmd("MAIN.stat2.txt=MAIN.stat1.txt");

		/* 取得ステイタス=> STATUS1 */
		sprintf(command, "MAIN.stat1.txt=\"%s\"", status);
		sendcmd(command);

		sendcmd("MAIN.t2.txt=MAIN.stat1.txt");
		sendcmd("MAIN.t3.txt=MAIN.stat2.txt");
		sendcmd("USERS.t8.txt=DMON.stat1.txt");
		sendcmd("USERS.t9.txt=DMON.stat2.txt");
	}
	return(0);
}
