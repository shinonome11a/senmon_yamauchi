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
	struct sockaddr_in sa, ca;
	char sendline[BUFSIZE], recvline[BUFSIZE + 1];

	if(argc != 3){
		fprintf(stderr, "invalid parameters\n");
		return -1;
	}

	bzero( (char *)&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(argv[1]);
	sa.sin_port = htons(atoi(argv[2]));
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero( (char *)&ca, sizeof(ca));
	ca.sin_family = AF_INET;
	ca.sin_addr.s_addr = htonl(INADDR_ANY);
	ca.sin_port = htons(0);

	bind(sockfd, (struct sockaddr *)&ca, sizeof(ca));
	while(1){
		fgets(sendline, BUFSIZE, stdin);
		n = strlen(sendline);
		sendto(sockfd, sendline, n, 0, (struct sockaddr *)&sa, sizeof(sa));
		n = recvfrom(sockfd, recvline, BUFSIZE, 0, (struct sockaddr *)NULL, (socklen_t *)NULL);
		recvline[n] = '\0';
		fputs(recvline, stdout);
		fflush(stdout);
	}
	return 0;
}
