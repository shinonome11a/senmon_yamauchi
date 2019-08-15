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

#define DEFAULT_SERV_TCP_PORT 8080

int main(int argc, char *argv[]){
   int sockfd, len, s, n, port;
   struct sockaddr_in ca;
   char buf[1024];
   char scode[] = "HTTP/1.0 200 OK\n";
   char eoh[] = "\n";
   FILE *fp;

   switch(argc){
      case 1:
         port = DEFAULT_SERV_TCP_PORT;
         break;
      case 2:
         port = atoi(argv[1]);
         break;
      default:
         fprintf(stderr, "invalid arguments.\n");
         return EXIT_FAILURE;
   }

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   ca.sin_family = AF_INET;
   ca.sin_addr.s_addr = htonl(INADDR_ANY);
   ca.sin_port = htons(port);

   if(bind(sockfd, (struct sockaddr *)&ca, sizeof(ca)) == -1){
      perror("bind");
      return EXIT_FAILURE;
   }

   listen(sockfd, 5);

   len = sizeof(ca);
   s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
   //ここに穴埋め。接続元IPとポート番号を画面に出力
   printf("connected from %s:%d\n",inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));

   n = recv(s, buf, sizeof(buf), 0);
   buf[n] = 0;
   //ここに穴埋め。文字列buf[]の長さと内容を画面に出力
   printf("buf.len:%ld\n%s\n",sizeof(buf), buf);

   send(s, scode, strlen(scode), 0);
   send(s, eoh, strlen(eoh), 0);

   fp = fopen("htdocs/index.html", "r");
   while((n = fread(buf, 1, 1024, fp)) > 0){
      send(s, buf, n, 0);
   }
   fclose(fp);
   close(s);
   return EXIT_SUCCESS;
}
