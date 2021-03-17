#include "dmonitor.h"

/*
 * dmonitor.h での宣言変数を初期化する
 */
repeater_t	linkdata[LISTSIZE]	= {'\0'};
char		station[CHARSIZE]	= {'\0'};
char		ipaddress[CHARSIZE]	= {'\0'};
char		default_rpt[CHARSIZE]	= {'\0'};
char		nextion_port[CHARSIZE]	= {'\0'};
char		rigtype[8]		= {'\0'};
char		chkpktnum[8]		= {'\0'};
char		status[CHARSIZE]	= {'\0'};
char		statpre1[CHARSIZE]	= {'\0'};
char		statpre2[CHARSIZE]	= {'\0'};
char		rptcall[CHARSIZE]	= {'\0'};
char		rptcallpre1[CHARSIZE]	= {'\0'};
char		rptcallpre2[CHARSIZE]	= {'\0'};
char		statpre3[CHARSIZE]	= {'\0'};
char		line[128]		= {'\0'};
char		linepre[128]		= {'\0'};
char		linepre2[128]		= {'\0'};
char		linepre3[128]		= {'\0'};
char		dmon_pid[6]		= {'\0'};
char		dmon_pid_cur[6]		= {'\0'};
unsigned int	microsec		= 0;
unsigned int	debug			= 0;
unsigned int	stat			= 0;
unsigned int	linecounter		= 0;
