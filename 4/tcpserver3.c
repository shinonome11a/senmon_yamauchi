#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

int main (int argc, char *argv[]){
   int sockfd, len, s, n, i;
   struct sockaddr_in sa, ca;
   char buf[512];
   char *c = buf;
   

   if(argc != 2){
      fprintf(stderr, "invalid argumens\n");
      return EXIT_FAILURE;
   }
   
   // ソケットを１つ確保
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   // 構造体saを用意
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = htons(INADDR_ANY);
   sa.sin_port = htons(atoi(argv[1]));
   
   // bindし、戻り値が負の場合はbindエラーを返して終了
   if(bind(sockfd,(struct sockaddr *)&sa, sizeof(sa)) < 0){
      perror("bind");
      return EXIT_FAILURE;
   }
   
   // listen()でクライアントからの通信を待ち受ける
   listen(sockfd, 5);
   
   while(1){
      len = sizeof(ca);

      // accept()で接続を受け入れ、接続されるとaccept()を抜ける
      s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
      // sockfdはTCP接続を受け入れるソケット、sは接続されたソケット
      
      // forkしてプロセスIDを変数pidに格納
      int pid = fork();
      
      if(pid == 0){
      // 子プロセスの時以下の項目を実行

         printf("Connected %d\n",s);
         
         while(1){
            n = recv(s, buf, sizeof(buf), 0);
            if(n <= 0){
               close(s);
               return EXIT_SUCCESS;
            }
            buf[n] = '\0';
            fputs(buf,stdout);
            fflush(stdout);
            c = buf;
            while(*c){
               *c = (char)tolower((int)*c);
               c++;
            }
            send(s,buf,n,0);
            memset(buf,'\0',512);
         }
      } else {
         continue;
      }
   }
      
   return EXIT_SUCCESS;
}
