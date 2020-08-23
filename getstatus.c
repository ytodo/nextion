/************************************************************
	dmonitor のログファイルよりラストハード及び
	状況を取得し変数status に入れる
 ************************************************************/

#include "dmonitor.h"

int getstatus()
{
	FILE	*fp;
	char	*getstatus	= "tail -n3 /var/log/dmonitor.log";
	char	*tmpptr;
	char	tmpstr[32]	= {'\0'};
	char	ret[2]		= {'\0'};
	char	line[128]	= {'\0'};
	char	jitter_av[8]	= {'\0'};
	char	jitter_mx[8]	= {'\0'};
	char	jitter_mi[8]	= {'\0'};
	char	zrgw[8]		= {'\0'};

	/* コマンドの標準出力オープン */
	if ((fp = popen(getstatus, "r")) == NULL)
	{
		printf("File open error!\n");
		return (EXIT_FAILURE);
	}

	/* 過去のデータをクリアする  */
	memset(&status[0], '\0', sizeof(status));
	memset(&rptcall[0],'\0', sizeof(rptcall));

	/* 標準出力を配列に取得 */
	while ((fgets(line, sizeof(line), fp)) != NULL)
	{
		/* rpt2, rpt1, ur, my の行が見つかったら */
		if ((tmpptr = strstr(line, "rpt1:")) != NULL)
		{
			memset(&zrgw[0], '\0', sizeof(tmpstr));
			if (strncmp("G", tmpptr + 12, 1) == 0) strncpy(tmpstr, " GW", 3);
			if (strncmp("A", tmpptr + 12, 1) == 0) strncpy(tmpstr, " ZR", 3);
			if (strncmp("B", tmpptr + 12, 1) == 0) strncpy(tmpstr, " ZR", 3);

			memset(&status[0], '\0', sizeof(status));
			strncpy(status, line, 12);      	// 日付時分
			strcat(status, " ");
			strncat(status, tmpptr + 29, 8); 	// コールサイン
			strcat(status, tmpstr); 		// ZR/GW
			stat = 0;
		}

		/* 無線機から送信したときのログを出力 */
		if ((tmpptr = strstr(line, "from Rig")) != NULL || (tmpptr = strstr(line, "from DVAP")) != NULL)
		{
			memset(&zrgw[0], '\0', sizeof(tmpstr));
			if (strncmp("Rig",  tmpptr + 5, 3) == 0) strncpy(tmpstr, " TM", 3);
			if (strncmp("DVAP", tmpptr + 5, 4) == 0) strncpy(tmpstr, " DV", 3);

			memset(&status[0], '\0', sizeof(status));
			strncpy(status, line, 12);		// 日付時分
			strcat(status, " ");
			strncat(status, tmpptr - 9, 8);		// コールサイン
			strcat(status, tmpstr);			// Terminal-AP Mode/DVAP Mode
			stat = 0;
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
			break;
		}

		/* dmonitorの開始とバージョンを取得 */
		if ((tmpptr = strstr(line, "dmonitor start")) != NULL)
		{
			memset(&status[0], '\0', sizeof(status));
			strncpy(status, tmpptr, 21);
		}

		/* バッファの拡張のサイズを取得 */
		if ((tmpptr = strstr(line, "New FiFo buffer")) != NULL)
		{
			memset(&status[0], '\0', sizeof(status));
			strcpy(status, tmpptr + 9);
			status[strlen(status) - 1] = '\0';
		}

		/* 接続解除を取得 */
		if (strstr(line, "dmonitor end") != NULL)
		{
			memset(&status[0], '\0', sizeof(status));
			strcpy(status, "Disconnected");
		}

		/* 無線機の接続状況 */
		if ((debug == 1) && (strstr(line, "init/re-init") != NULL))
		{
			memset(&status[0], '\0', sizeof(status));
			strcpy(status, "RIG initializing is done.");
		}

		/* ドロップパケット比の表示 */
		if ((debug == 1) && ((tmpptr = strstr(line, "drop packet")) != NULL))
		{
			memset(&status[0], '\0', sizeof(status));
			strcpy(status, "Drop PKT ");
			strcat(status, tmpptr + 17);
			status[strlen(status) - 1] = '\0';
			stat = 1;
		}
	}
	pclose(fp);
	return (EXIT_SUCCESS);
}

/*
Jul 15 04:53:38 ham12 dmonitor[10813]: dmonitor start V01.43 (Compiled Jul  3 2020 04:20:00)
Jul 15 04:53:38 ham12 dmonitor[10813]: Connected to JL3ZBS A (111.64.21.98:51000) from JE3HCZ D
Jul 15 04:53:39 ham12 dmonitor[10813]: RIG(ID-xxPlus) open
Jul 15 04:53:39 ham12 dmonitor[10813]: RIG(ID-xxPlus) init/re-init done
Jul 15 04:53:52 ham12 dmonitor[10813]: JE3HCZ A from ZR
Jul 15 04:53:52 ham12 dmonitor[10813]: drop packet rate 0.00% (0/13)
Jul 15 04:53:52 ham12 dmonitor[10813]: jitter info. ave:16mSec. max:168mSec. min:1mSec.
Jul 15 04:53:53 ham12 dmonitor[10813]: rpt2:JL3ZBS A rpt1:JL3ZBS A ur:JK1ZRW Z my:JE3HCZ A my2:ADMO
Jul 15 04:53:53 ham12 dmonitor[10813]: JK1ZRW Z from GW
Jul 15 04:53:54 ham12 dmonitor[10813]: drop packet rate 0.00% (0/13)
Jul 15 04:53:54 ham12 dmonitor[10813]: jitter info. ave:17mSec. max:21mSec. min:17mSec.
Jul 15 04:53:54 ham12 dmonitor[10813]: rpt2:JL3ZBS A rpt1:JL3ZBS G ur:JE3HCZ A my:JK1ZRW Z my2:ZECH
Jul 15 04:54:26 ham12 dmonitor[10813]: RIG(ICOM) close.
Jul 15 04:54:26 ham12 dmonitor[10813]: dmonitor end
*/

