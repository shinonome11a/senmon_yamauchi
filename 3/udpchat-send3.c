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
   int sockfd, n, yes = 1;
   struct sockaddr_in sa;
   char sendline[BUFSIZE] = "hoge";

   if(argc != 3){
      fprintf(stderr, "invalid parameters\n");
      return -1;
   }

   bzero( (char *)&sa, sizeof(sa));
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = inet_addr(argv[1]);
   sa.sin_port = htons(atoi(argv[2]));

   sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));
	
	int i = 0;
   while(1){
		printf("%d\r", i++);
      //sendline = "fuga";
      n = strlen(sendline);
      sendto(sockfd, sendline, n, 0, (struct sockaddr *)&sa, sizeof(sa));
   }
   return 0;
}

