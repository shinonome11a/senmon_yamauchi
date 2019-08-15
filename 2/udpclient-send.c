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
   int sockfd, n;

   // 送信に関する情報を格納する構造体
   struct sockaddr_in sa;
   char sendline[BUFSIZE];

   if(argc != 3){
      fprintf(stderr, "invalid parameters\n");
      return -1;
   }

   // 送信用の情報を構造体saに格納する
   bzero( (char *)&sa, sizeof(sa));
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = inet_addr(argv[1]);
   sa.sin_port = htons(atoi(argv[2]));
   
   // ソケットを取得する
   sockfd = socket(AF_INET, SOCK_DGRAM, 0);

   while(1){
      // 標準入力より送信する文字列を取得して送信する
      fgets(sendline, BUFSIZE, stdin);
      n = strlen(sendline);
      sendto(sockfd, sendline, n, 0, (struct sockaddr *)&sa, sizeof(sa));
   }
   return 0;
}

