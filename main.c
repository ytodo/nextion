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
 *　コマンドコントロールには安田ＯＭ（7m3tjz）の公開WEBコントロールのアルゴリズムを
 *  そのまま、使用しています。
 */


#include "dmonitor.h"

int main(int argc, char *argv[])
{
	FILE	*fp;
	int	num;                // 返り値のi を受ける（件数）
	int	arraycount;
	int	fd;
	int	i;
	int	flag;
	int	bufcnt;
	char	command[32]	= {'\0'};
	char	statpre[32]	= {'\0'};
	char	concall[8]	= {'\0'};
	char	concallpre[8]	= {'\0'};
	char	SERIALPORT[16]	= {'\0'};


	/* 環境設定ファイルの読み取り */
	getconfig();
	getipaddr();

	/* 関連するサービスのコントロール */
	system("sudo systemctl restart auto_repmon");
	system("sudo systemctl restart rpt_conn");
//	system("sudo systemctl restart auto_repmon_light");

	/* 現在利用可能なリピータリストの取得*/
	num = getlinkdata();

	/* シリアルポートのオープン nextion.iniより */
	if ((nextion_port != NULL) && (strlen(nextion_port) != 0))
	{
		/* nextion.iniにポート指定が有る場合 */
		sprintf(SERIALPORT, "/dev/%s", nextion_port);
	}
	else
	{	/* ポート指定が無い場合 */
		strcpy(SERIALPORT, "/dev/ttyAMA0");

	}
	fd = openport(SERIALPORT, BAUDRATE);

	/* メインスクリーンの初期設定 */
	sendcmd("dim=dims");
	sendcmd("page MAIN");

	sprintf(command, "MAIN.version.txt=\"Ver.%d.%d.%d\"", VERSION, VERSUB, RELEASE);
	sendcmd(command);
	sendcmd("MAIN.t6.txt=version.txt");
	sprintf(command, "MAIN.station.txt=\"STATION : %s\"", station);
	sendcmd(command);
	sprintf(command, "t0.txt=\"STATION : %s\"", station);
	sendcmd(command);
	sendcmd("t1.txt=\"LINK TO : NONE\"");
	sendcmd("link.txt=\"LINK TO : NONE\"");

	/* Nextionの初期化と読み込んだリピータ総数の表示 */
	sendcmd("MAIN.t2.txt=\"\"");
	sendcmd("MAIN.t3.txt=\"\"");
	sendcmd("MAIN.stat1.txt=\"\"");
	sendcmd("MAIN.stat2.txt=\"\"");
	sprintf(command, "MAIN.stat1.txt=\"Reading %d Repeaters\"", num);
	sendcmd(command);
	sendcmd("t2.txt=MAIN.stat1.txt");

	/* 全リストを空にした後リピータ数分の文字配列にコールサインを格納 */
	for (i = 0; i < 227; i++)
	{
		sprintf(command, "VALUE.va%d.txt=\"\"", i);
		sendcmd(command);
		usleep(microsec);
	}
	usleep(microsec * 300);
	if (num > 227) num = 227;
	for (i = 0; i < num; i++)
	{
		sprintf(command, "VALUE.va%d.txt=\"%s\"", i, linkdata[i].call);
		sendcmd(command);
		usleep(microsec);
	}

	/* チェックしたIPアドレスをSYSTEM pageに表示 */
	sprintf(command, "SYSTEM.va0.txt=\"%s\"", ipaddress);
	sendcmd(command);


	/* 送・受信ループ */
	while (1)
	{
		/*
		 * 受信処理
		 */

		/* タッチパネルのデータを読み込む */
		recvdata(concall);

		/* もしタッチデータが選択されていない場合、初回のみデフォルトリピータをセットする */
		if ((strlen(concall) == 0) && (strlen(default_rpt) != 0))
		{
			strcpy(concall, default_rpt);
		}

		/* タッチデータが選択されている場合、前回と同じかチェック（同じならパス） */
		if ((strlen(concall) > 1) && (strncmp(concall, concallpre, 8) != 0))
		{
			/* 現在の返り値を保存 */
			strncpy(concallpre, concall, 8);

			/* コマンドをスイッチに振り分ける */
			if (strncmp(concall, "Restart", 7) == 0) flag = 1;
			if (strncmp(concall, "Reboot",  6) == 0) flag = 2;
			if (strncmp(concall, "Shutdown",8) == 0) flag = 3;
			if (strncmp(concall, "Update",  6) == 0) flag = 4;
			if (strncmp(concall, "UP",      2) == 0) flag = 5;
			if (strncmp(concall, "DWN",     3) == 0) flag = 6;
			if (strncmp(concall, "USERS",   5) == 0) flag = 7;
			if (strncmp(concall, "ACTIVE",  6) == 0) flag = 8;

			switch (flag) {
			case 1:						// nextionドライバのリスタート
				sendcmd("dim=10");
				system("sudo killall -q -2 dmonitor");
				system("sudo rm -f /var/run/dmonitor.pid");
				system("sudo killall -q -9 sleep");
				system("sudo killall -q -9 repeater_scan");
				system("sudo /var/www/cgi-bin/repUpd");
				system("sudo systemctl restart nextion");
				break;

			case 2:						// 再起動
				sendcmd("dim=10");
				system("sudo killall -q -2 dmonitor");
				system("sudo rm -f /var/run/dmonitor.pid");
				system("sudo killall -q -9 sleep");
				system("sudo killall -q -9 repeater_scan");
				system("sudo shutdown -r now");
				break;

			case 3:						// シャットダウン
				sendcmd("dim=10");
				system("sudo killall -q -2 dmonitor");
				system("sudo rm /var/run/dmonitor.pid");
				system("sudo killall -q -9 sleep");
				system("sudo killall -q -9 repeater_scan");
				system("sudo shutdown -h now");
				break;

			case 4:						// dmonitorのアップデート
				/* [Update]ボタンの表示を変える */
				sendcmd("SYSTEM.b4.bco=63488");
				sendcmd("SYSTEM.b4.pco=65535");
				sendcmd("SYSTEM.b4.txt=\"Checking Update\"");

				/* システムコマンドの実行 */
				system("sudo killall -q -2 dmonitor");
				system("sudo rm /var/run/dmonitor.pid");
				system("sudo killall -q -9 repeater_scan");
				system("sudo apt clean && sudo apt update && sudo apt install dmonitor -y");

				/* [REBOOT]の表示及びrebootコマンド発行 */
				sendcmd("SYSTEM.b4.bco=64512");
				sendcmd("SYSTEM.b4.txt=\"Restarting...\"");
				sendcmd("dim=10");
				system("sudo systemctl restart nextion");
				break;

			case 5:						// バッファの増加
				if (strncmp(concall, "up", 2) == 0) break;
				strcpy(concall, "up");
				system("sudo killall -q -s SIGUSR1 dmonitor");
				break;

			case 6:						// バッファの減少
				if (strncmp(concall, "dwn", 3) == 0) break;
				strcpy(concall, "dwn");
				system("sudo killall -q -s SIGUSR2 dmonitor");
				break;

			case 7:						// Remote Usersパネルへ接続ユーザ表示
				getusers(concall);
				sendcmd("page MAIN");
				break;

			case 8:						// ACTIVE REPEATERパネルへの表示と移動
				getactive(concall);
				sendcmd("page MAIN");
				if ((strncmp(concall, "J", 1) != 0) && (strncmp(concall, "Retrun", 6) != 0))
				{
					strncpy(concall, "Return", 6);
				}
				break;

			default:

				/* 指定リピータに接続する */
				i = 0;
				num = getlinkdata();
				for (i = 0; i < num; i++)
				{
					if (strncmp(linkdata[i].call, concall, 8) == 0)
					{
						/* 接続コマンド実行前処理 */
						system("sudo systemctl stop rpt_conn");
						system("sudo killall -q -s 9 repeater_scan");
						system("sudo killall -q -s 9 dmonitor");
						system("sudo rm -f /var/run/dmonitor.pid");
						system("sudo killall -q -s 9 rpt_conn");
						system("sudo rm -f /var/run/rpt_conn.pid");
						system("sudo rig_port_check");
						system("sudo cp /dev/null /var/tmp/update.log");
						system("sudo cp /var/www/html/error_msg.html.save /var/tmp/error_msg.html");
						system("sudo touch /var/tmp/error_msg.html");
						system("sudo cp /var/www/html/short_msg.html.save /var/tmp/short_msg.html");
						system("sudo touch /var/tmp/short_msg.html");
						system ("ulimit -c unlimited");

						/* 接続コマンドの実行 */
						sprintf(command, "sudo /usr/bin/dmonitor '%s' %s %s '%s' '%s'", station, linkdata[i].addr, linkdata[i].port, linkdata[i].call, linkdata[i].zone);
						system(command);
						sendcmd("page MAIN");
						break;
					}
				}
			}
			flag = 0;
		}


		/*
		 * 送信処理
		 */

		/* ステータス・ラストハードの読み取りと表示 */
		getstatus();

		/* 無線機からのコマンドを接続解除の間受け取る準備 */
		if (strcmp(status, "UNLINK FROM RIG") == 0)
		{
                        system("sudo killall -q -2 dmonitor");
                        system("sudo rm -f /var/nun/dmonitor.pid");
			usleep(microsec * 10);
			system("sudo systemctl restart rpt_conn");
			status[0] = '\0';
		}

		/* CPUの速さによるループ調整（nextion.ini:SLEEPTIME）*/
	        usleep(microsec * 5);

	}	// Loop

	/* GPIO シリアルポートのクローズ*/
	close(fd);

	return (EXIT_SUCCESS);
}
