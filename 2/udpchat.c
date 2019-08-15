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
	struct sockaddr_in sa, ca;
	char sendline[BUFSIZE], recvline[BUFSIZE + 1];

	//if(argc != 3){
	//	fprintf(stderr, "invalid parameters\n");
	//	return -1;
	//}

	bzero( (char *)&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("255.255.255.255");
	sa.sin_port = htons(50003);
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes)); // 追加

	bzero( (char *)&ca, sizeof(ca));
	ca.sin_family = AF_INET;
	ca.sin_addr.s_addr = htonl(INADDR_ANY);
	ca.sin_port = htons(50003);

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
