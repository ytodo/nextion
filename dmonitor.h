/**************************************************
 * D-STAR Repeater Nextion display for dmonitor   *
 *      main.h version 00.01                      *
 *      2018.11.01 -                              *
 *                                                *
 *  Multi_Forwardが搭載されているリピータリストを *
 *  取得して「接続可能リピータ」としてdmonitor用  *
 *  ディスプレイに表示。タッチパネルから接続する　*
 *                                                *
 **************************************************/
#ifndef __DMONITOR_H__
#define __DMONITOR_H__

/* Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

//test open port
#include <sys/ioctl.h>



/* Macros */
#define	BAUDRATE	9600
#define LISTSIZE	256
#define CHARSIZE	30

/* Variables */
typedef struct {        			// 構造体の宣言
	char	name[20];
	char	call[9];
	char	addr[16];
	char 	port[6];
} repeater_t;
extern	repeater_t	linkdata[LISTSIZE];	// 構造体配列の宣言
extern	char	status[CHARSIZE];
extern	char	rptcall[CHARSIZE];

/* Functions */
int	getlinkdata();
int	getstatus();
int	openport(char *devicename, long baudrate);
void	recvdata(char *rptcon);
void	sendcmd(char *cmd);

#endif // __DMONITOR_H__

