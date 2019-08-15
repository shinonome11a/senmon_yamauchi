#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>     /* ネットワークプログラミングには，これらのヘッダファイルが必要 */
#include <arpa/inet.h>      /* これらのファイルは /usr/include ディレクトリの下にある */

#define BUFSIZE 512         /* 送受信するメッセージのバッファサイズ */

/**
NAME
        udpclient -- simple udp client

SYNOPSIS
        udpclient address port

DESCRIPTION
    address:UDPサーバのIPアドレス
    port:   UDPサーバのポート番号
*/
int main(int argc, char** argv) {
    int sockfd, n;
    struct sockaddr_in  sa, ca;                         /* saは送信用，caは受信用の構造体 */
    char sendline[BUFSIZE], recvline[BUFSIZE + 1];      /* 送信用，受信用の文字列格納配列 */

    if (argc != 3) {
        fprintf(stderr, "invalid parameters\n");
        return -1;
    }

    /* ■　通信相手先の送信用情報を構造体saに格納する */
    bzero((char *)&sa, sizeof(sa));             /* 構造体saの中をゼロで埋める */
    sa.sin_family = AF_INET;                    /* TCPやUDPを使うという指定 */
    sa.sin_addr.s_addr = inet_addr(argv[1]);    /* 相手IPアドレス */
    sa.sin_port = htons(atoi(argv[2]));         /* 相手ポート番号 */

    /* ■　ソケットを１つ確保する */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);    /* UDP通信用のソケット獲得 */

    /* ■　受信用の情報を構造体caに格納する */
    bzero( (char *) &ca, sizeof(ca) );
    ca.sin_family = AF_INET;
    ca.sin_addr.s_addr  = htonl(INADDR_ANY);
    /* 自分自身のどのＩＰアドレスにパケットが到着しても自分宛として受取る */
    ca.sin_port = htons(0); 
    /* 自分側のポート番号はＯＳによって適当に付与してもらう，ここをゼロ以外の値に設定すると，自分側のポート番号を陽に指定できる */

    /* ■　受信用のポートをソケットにバインドする，これにより当該ポートでの受信が開始される */
    bind(sockfd, (struct sockaddr *)&ca, sizeof(ca));

    /* ■　キーボードから読み込んだ文字列を送信する，また受信したデータを画面に表示する */
    for(;;) {                                                               /* 無限ループ */
        fgets(sendline, BUFSIZE, stdin);                                    /* キーボードから1行読込み */
        n = strlen(sendline);                                               /* 文字列の長さを変数nに代入 */
        sendto(sockfd, sendline, n, 0, (struct sockaddr *)&sa, sizeof(sa)); /* ソケットsockfdに配列sendlineの内容を書き出す */
        n = recvfrom(                                                       /* ソケットからデータを読出す，最大512文字 */
            sockfd,
            recvline,
            BUFSIZE, 0,
            (struct sockaddr *)NULL,  (socklen_t *)NULL
        );
        recvline[n] = '\0';                                                 /* 受信データには文字列の終端記号が含まれていないので書き足す */
        fputs(recvline, stdout);                                            /* 受信文字列を画面に出力 */
        fflush(stdout);                                                     /* 画面出力はある程度溜まらないと描画しないので，強制的に書き出す */
    }

    return 0;
}
