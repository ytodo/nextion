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

/* Macros */
#define	BAUDRATE	9600

/* Variables */
typedef struct {        		// 構造体の宣言
	char	name[20];
	char	call[9];
	char	addr[16];
	char 	port[6];
} repeater_t;
extern	repeater_t	linkdata[512];	// 構造体配列の宣言

/* Functions */
int	getlinkdata();
int	openport(char *devicename, long baudrate);
void	sendcmd(char *cmd);

#endif // __DMONITOR_H__

