#include "dmonitor.h"

repeater_t	    linkdata[LISTSIZE] 	  = {'\0'};
char		    status[CHARSIZE] 	  = {'\0'};
char		    rptcall[CHARSIZE]	  = {'\0'};
char            station[CHARSIZE]     = {'\0'};
char            default_rpt[CHARSIZE] ={'\0'};
unsigned int    microsec              = 0;
