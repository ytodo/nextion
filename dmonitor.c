#include "dmonitor.h"

/*
 * dmonitor.h での宣言変数を初期化する
 */
repeater_t	linkdata[LISTSIZE]	= {'\0'};
char		station[CHARSIZE]	= {'\0'};
char		ipaddress[CHARSIZE]	= {'\0'};
char		default_rpt[CHARSIZE]	= {'\0'};
char		status[CHARSIZE]	= {'\0'};
char		statpre[CHARSIZE]	= {'\0'};
char		rptcall[CHARSIZE]	= {'\0'};
char		rptcallpre[CHARSIZE]	= {'\0'};
unsigned int	microsec		= 0;
unsigned int	debug			= 0;
unsigned int	stat			= 0;
