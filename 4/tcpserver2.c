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

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   //printf("%d\n",sockfd);
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = htons(INADDR_ANY);
   sa.sin_port = htons(atoi(argv[1]));

   if(bind(sockfd,(struct sockaddr *)&sa, sizeof(sa)) < 0){
      perror("bind");
      return EXIT_FAILURE;
   }

   listen(sockfd, 5);

   len = sizeof(ca);
   s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
   printf("Connected %d\n",s);

   while(1){
      n = recv(s, buf, sizeof(buf), 0);
      //printf("recv: %d\n",n);
      if(n == 0){
         close(sockfd);
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

   return EXIT_SUCCESS;
}
