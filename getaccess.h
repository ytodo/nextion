/********************************************
 * D-STAR Repeater Version Check  Program   *
 *      main.h version 00.01                *
 *      2018.01.01 -                        *
 *                                          *
 *  Repeater(Node)から送信されるバージョン  *
 *  情報を収集しリストにする                *
 *  その上で/tmp にverfile と言うファイルを *
 *  作成 chkver プログラムに提供する        *
 *                                          *
 ********************************************/
#ifndef __GETACCESS_H__
#define __GETACCESS_H__

/* header files */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "dmonitor.h"

/* macros */
#define NUM 		512
#define	WAIT_TIME	90

/* variables */
FILE    *fp;
char    *conffile 	= "/etc/ircddbgateway";
char    *verfile  	= "/dstar/verfile";
char    line[NUM]	= {'\0'};
char    str[NUM]	= {'\0'};
char    *iface;
char    *ret;
char	*ret2;
char    rptband[4][20], rptaddr[4][20], rptport[4][20];
char    rptversion[4][100];
int     i = 0;
int     j = 0;
unsigned int sleep(unsigned int seconds);

/* time */
time_t time(time_t *t_start);
time_t time(time_t *t_end);


#endif // __GETACCESS_H__

