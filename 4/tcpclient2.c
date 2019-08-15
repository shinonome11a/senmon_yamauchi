#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]){
   int sockfd, n;
   struct sockaddr_in sa;
   char sendline[512], buf[512];

   if(argc != 3){
      fprintf(stderr, "invalid arguments.\n");
      return EXIT_FAILURE;
   }

   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   bzero((char *)&sa, sizeof(sa));
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = inet_addr(argv[1]);
   sa.sin_port = htons(atoi(argv[2]));

   connect(sockfd,(struct sockaddr *)&sa, sizeof(sa));
   printf("Connected\n");

   while(1){
      fgets(sendline, 512, stdin);
      n = strlen(sendline);
      //printf("sendline: %d\n",n);
      if(n == 1){
         close(sockfd); // 追加箇所
         return EXIT_SUCCESS;
      }
      send(sockfd, sendline, n, 0);
      n = recv(sockfd, buf, sizeof(buf), 0);
      //printf("recv: %d\n", n);
      buf[n] = '\0';
      fputs(buf,stdout);
      fflush(stdout);
   }

   return EXIT_SUCCESS;
}
