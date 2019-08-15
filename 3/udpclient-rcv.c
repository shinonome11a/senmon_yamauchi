#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 512

int main(int argc, char* argv[]){
   int sockfd, n;
   struct sockaddr_in ca;
   char recvline[BUFSIZE + 1];

   if(argc != 3){
      fprintf(stderr, "invalid parameters\n");
      return -1;
   }
   sockfd = socket(AF_INET, SOCK_DGRAM, 0);

   bzero( (char *)&ca, sizeof(ca));
   ca.sin_family = AF_INET;
   ca.sin_addr.s_addr = htonl(INADDR_ANY);
   ca.sin_port = htons(atoi(argv[2]));

   bind(sockfd, (struct sockaddr *)&ca, sizeof(ca));
   while(1){
      n = recvfrom(sockfd, recvline, BUFSIZE, 0, (struct sockaddr *)NULL, (socklen_t *)NULL);
      recvline[n] = '\0';
      fputs(recvline, stdout);
      fflush(stdout);
   }
   return 0;
}

