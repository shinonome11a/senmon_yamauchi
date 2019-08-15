#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
   int sockfd, n;
   // 送信に関する情報を格納する構造体
   struct sockaddr_in sa;
   char sendline[512], buf[512];

   if(argc != 3){
      fprintf(stderr, "invalid arguments.\n");
      return EXIT_FAILURE;
   }
   
   // ソケットを１つ確保する
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   // 送信用の情報を構造体saに格納する
   bzero((char *)&sa, sizeof(sa));
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = inet_addr(argv[1]);
   sa.sin_port = htons(atoi(argv[2]));

   // connect()で目的の相手と接続
   connect(sockfd,(struct sockaddr *)&sa, sizeof(sa));
   printf("Connected\n");

   while(1){
      fgets(sendline, 512, stdin);
      n = strlen(sendline);
      send(sockfd, sendline, n, 0);
      n = recv(sockfd, buf, sizeof(buf), 0);
      buf[n] = '\0';
      fputs(buf,stdout);
      fflush(stdout);
   }

   return EXIT_SUCCESS;
}
