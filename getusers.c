#include "dmonitor.h"

int getusers()
{
    FILE    *fp;
    char    *users_tbl  = "/var/www/html/connected_table.html";
    char    line[64]    = {'\0'};
    char    tmpstr[32]  = {'\0'};
    char    *tmpptr;
    char    command[32] = {'\0'};
    char    ret[16]     = {'\0'};
    int     i           = 0;

    /* Remote Usersページへ */
    sendcmd("page USERS");

    /* [Return]ボタンが押されるまで繰り返す */
    while ((strncmp(ret, "Return", 6)) != 0) {

        /* [Return]の検出 */
        recvdata(ret);

        /* コマンドの標準出力オープン */
        if ((fp = fopen(users_tbl, "r")) == NULL) {
            printf("File open error!\n");
            return (EXIT_FAILURE);
        }

        /* 接続ユーザリストをクリア */
        for (i = 0; i < 7; i++) {
            sprintf(command, "USERS.t%d.txt=\"\"", i);
            sendcmd(command);
        }
        i = 0;

       /* ファイルを一行ずつ読み最終行まで繰り返す  */
        while ((fgets(line, sizeof(line), fp)) != NULL) {

            /* <td> を見つけ内容を取得し、リストに表示 */
            if ((tmpptr = strstr(line, "<td>")) != NULL) {
                strncpy(tmpstr, tmpptr + 4, 8);
                sprintf(command, "USERS.t%d.txt=\"%s\"", i, tmpstr);
                sendcmd(command);
                i++;
            }
        }

        /* ファイルクローズ */
        fclose(fp);

        /* 5秒に一回リフレッシュする */
        sleep(5);
    }

    return(EXIT_SUCCESS);
}

