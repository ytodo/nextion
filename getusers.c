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
    int     j           = 0;

    /* [Return]ボタンが押されるまで繰り返す */
    while (1) {

        /* [Return]の検出 */
        recvdata(ret);
        if ((strncmp(ret, "Return", 6)) == 0) break;

        /* コマンドの標準出力オープン */
        if ((fp = fopen(users_tbl, "r")) == NULL) {
            printf("File open error!\n");
            return (EXIT_FAILURE);
        }

        /* ファイルを一行ずつ読み最終行まで繰り返す  */
        i = 0;
        while ((fgets(line, sizeof(line), fp)) != NULL) {

            /* <td> を見つけ内容を取得し、リストに表示 */
            if ((tmpptr = strstr(line, "<td>")) != NULL) {
                strncpy(tmpstr, tmpptr + 4, 8);
                sprintf(command, "USERS.t%d.txt=\"%s\"", i, tmpstr);
                sendcmd(command);
                i++;
                if (i  >= 14) break;
            }
        }

        /* ファイルクローズ */
        fclose(fp);

        /* リスト14件に満たない場合、残りをクリアする */
        if (i < 14 ) {
            for (j = i; j < 14; j++) {
                sprintf(command, "USERS.t%d.txt=\"\"", j);
                sendcmd(command);
            }
        }

        /* 3秒に一回リフレッシュする */
        sleep(3);
    }

    return(EXIT_SUCCESS);
}

