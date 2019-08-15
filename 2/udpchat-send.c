#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 送受信するメッセージのバッファサイズ
#define BUFSIZE 512

int main(int argc, char* argv[]){
   int sockfd, n, yes = 1;
   // 送信に関する情報を格納する構造体
   struct sockaddr_in sa;
   char sendline[BUFSIZE];

   // 送信用の情報を構造体saに格納
   bzero( (char *)&sa, sizeof(sa));
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = inet_addr("255.255.255.255");
   // 宛先ポート番号を50003に設定
   sa.sin_port = htons(50003);
   
   // ソケットを取得
   sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   // ブロードキャストを有効化
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));

   while(1){
      // 標準入力より送信する文字列を取得して送信
      fgets(sendline, BUFSIZE, stdin);
      n = strlen(sendline);
      sendto(sockfd, sendline, n, 0, (struct sockaddr *)&sa, sizeof(sa));
   }
   return 0;
}

