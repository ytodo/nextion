/*
 *  Copyright (C) 2018-2020 by Yosh Todo JE3HCZ
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  本プログラムはD-STAR Network の一部の機能を実現するための物で、
 *  アマチュア無線の技術習得とその本来の用途以外では使用しないでください。
 *
 */

/***********************************************************
	D-STAR Repeater Nextion display for dmonitor
		D*MONITOR version 1.5.0
		2018.11.01 - 2020.01.24

	Multi_Forwardが搭載されているリピータリストを
	取得して「接続可能リピータ」としてdmonitor用
	ディスプレイに表示。タッチパネルから接続する
 ***********************************************************/
#ifndef __DMONITOR_H__
#define __DMONITOR_H__

/* Header Files */
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<fcntl.h>
#include	<termios.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/ioctl.h>

/* Macros */
#define	BAUDRATE	9600
#define	LISTSIZE	256
#define	CHARSIZE	32
#define	VERSION		1
#define	VERSUB		6
#define RELEASE		6
#define REPMON		"auto_repmon_light"
//#define REPMON		"auto_repmon"

/* Variables */
typedef struct {                  			// 構造体の宣言
	char	name[20];
	char	call[9];
	char	addr[16];
	char	port[6];
	char	zone[9];
}	repeater_t;
extern	repeater_t	linkdata[LISTSIZE];		// 構造体配列の宣言(リピータリスト）

extern	char		station[CHARSIZE];		// 
extern	char		ipaddress[CHARSIZE];
extern	char		default_rpt[CHARSIZE];
extern 	char		nextion_port[CHARSIZE];
extern	char		rigtype[8];
extern	char		status[CHARSIZE];
extern	char		statpre1[CHARSIZE];
extern	char		statpre2[CHARSIZE];
extern	char		statpre3[CHARSIZE];
extern	char		rptcall[CHARSIZE];
extern	char		rptcallpre1[CHARSIZE];
extern	char		rptcallpre2[CHARSIZE];
extern	char		dmon_pid[6];
extern	char		dmon_pid_cur[6];
extern	unsigned int	microsec;
extern	unsigned int	debug;
extern	unsigned int	stat;
extern	unsigned int	linecounter;

/* Functions */
int	getlinkdata();
int	getstatus();
int	getconfig();
int	getipaddr();
int	disp_stat();
int	disp_rpt();
int	getusers(char ret[16]);
int	getactive(char ret[16]);
int	openport(char *devicename, long baudrate);
void	recvdata(char *rptcon);
void	sendcmd(char *cmd);

#endif // __DMONITOR_H__
