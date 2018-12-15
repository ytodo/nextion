/************************************************
 * D-STAR dmonitor Get Access Status Program	*
 *      getaccess version 00.01                	*
 *      2018.11.28 -                        	*
 *                                          	*
 *  Repeater(multi_forward)からのストリームか 	*
 *  又は無線機側からのものかを判断し、いくつか	*
 *  データを取得する			 	*
 *  					    	*
 ************************************************/

/* header files */
#include "getaddcess.h"


/****************************************************************
 *                                                              *
 *  Get Access main routine                                     *
 *                                                              *
 ****************************************************************/
void getaccess(char *addr, char *port)
{
	/* Terminal command execute */
	sprintf(str, "tcpdump -A -vvv port %s -c1", port);
	fp = popen(str, "r");


/*
 * Sample
 *

### multi_forward 側エリアCQ データ ####################################
11:10:31.212214 IP (tos 0x0, ttl 52, id 44604, offset 0, flags [DF], proto UDP (17), lengt           h 86)
    pl9818.ag5354.nttpc.ne.jp.51000 > 10.1.0.109.57968: [udp sum ok] UDP, length 58
        0x0000:  4500 0056 ae3c 4000 3411 9c5d 8035 715a  E..V.<@.4..].5qZ
        0x0010:  0a01 006d c738 e270 0042 52d4 4453 5452  ...m.8.p.BR.DSTR
        0x0020:  6de3 7392 0030 2000 0104 c0cb 8000 0000  m.s..0..........
        0x0030:  4a50 3359 4950 2041 4a50 3359 4950 2047  JP3YIP.AJP3YIP.G
        0x0040:  4351 4351 4351 2020 4a45 3348 435a 2020  CQCQCQ..JE3HCZ..
        0x0050:  3731 3030 29da                           7100).

### dmonitor からmulti_forward への送信データ ##########################
*/

    	/* Store command return to line	*/
	while (fgets(line, sizeof(line), fp) != NULL) {

		/* If keyword "linux" existed in line, set words from "linux" to EOF to ret */
        	if ((ret = strstr(line, addr)) != NULL) {
			strcpy(rptversion[i], ret);
		}
	}

	/* Close file */
	pclose(fp);

	return (ret);
}

