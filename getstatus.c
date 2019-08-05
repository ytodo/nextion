/********************************************************
 * dmonitor のログファイルよりラストハード及び
 * 状況を取得し変数status に入れる
 ********************************************************/

#include "dmonitor.h"


int getstatus()
{
	FILE	*fp;
	char	line[128] 	= {'\0'};
	char	*getstatus 	= "tail -n3 /var/log/dmonitor.log";
	char	*tmpstr;

	/* コマンドの標準出力オープン */
	if ((fp = popen(getstatus, "r")) == NULL) {
		printf("File open error!\n");
		return (EXIT_FAILURE);
	}

/* sample
Nov 14 05:28:08 ham22 dmonitor[965]: dmonitor start V00.11 (Compiled Oct 22 2018 23:00:00)
Nov 14 05:28:08 ham22 dmonitor[965]: RIG(ID-xxPlus)  open
Nov 14 05:28:08 ham22 dmonitor[965]: Connected to JP3YIP A (128.53.142.124:51000)
Nov 14 05:29:50 ham22 dmonitor[988]: dmonitor start V00.11 (Compiled Oct 22 2018 23:00:00)
Nov 14 05:29:50 ham22 dmonitor[988]: RIG(ID-xxPlus)  open
Nov 14 05:29:50 ham22 dmonitor[988]: Connected to JP3YIP A (128.53.127.230:51000)
Nov 14 05:30:23 ham22 dmonitor[988]: JE3HCZ A from GW
Nov 14 05:30:29 ham22 dmonitor[988]: JE3HCZ A from GW
*/

	/* 過去のデータをクリアする  */
	memset(&status[0], '\0', sizeof(status));
	memset(&rptcall[0],'\0', sizeof(rptcall));

	/* 標準出力を配列に取得 */
	while ((fgets(line, sizeof(line), fp)) != NULL) {


		if ((tmpstr = strstr(line, "Connected")) != NULL) {
			strncpy(rptcall, tmpstr + 13, 8);
		}
		if ((tmpstr = strstr(line, "dmonitor start")) != NULL) {
			strncpy(status, tmpstr, 21);
		}
        if ((tmpstr = strstr(line, "New FiFo buffer")) != NULL) {
            strcpy(status, tmpstr + 9);
            status[strlen(status) - 1] = '\0';
        }
	}

	/* status に関する文字列があったら */
	if ((tmpstr = strstr(line, "from")) != NULL) {
    	memset(&status[0], '\0', sizeof(status));
		strncpy(status, line, 16);
		strncat(status, tmpstr - 9, 8);
	}
	if ((tmpstr = strstr(line, "rig not")) != NULL) {
		strcpy(status, "Check rig not connected.");
	}


	pclose(fp);

	return (EXIT_SUCCESS);
}





