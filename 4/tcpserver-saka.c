#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<unistd.h>

int main(int argc,char** argv){
	int sockfd,len,s,n,i;
	struct sockaddr_in sa,ca;
	char buf[512];
	char *c = buf;
	
	if (argc !=2){
		fprintf(stderr, "invalid argument\n");
		return EXIT_FAILURE;
	}
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(atoi(argv[1]));
	
	if (bind(sockfd, (struct sockaddr *) &sa, sizeof(sa)) < 0) { 
		perror("bind"); 
		return EXIT_FAILURE;
	}
	
	listen(sockfd, 5);
	
   for(;;){
		len = sizeof(ca);
		s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
		
		
		int pid = fork();
		if (pid == 0) {  
			printf("Connected\n");
			for(;;) {                               
            n = recv(s, buf, sizeof(buf), 0);   
				
				if(n<=0){
					
					close(s);
					
					return EXIT_SUCCESS;
				}
				buf[n] = '\0';
            fputs(buf, stdout);                 
            fflush(stdout);
            c = buf;
            while (*c) {                        
					*c = (char)tolower((int)*c);
					c++;
            }   
            send(s, buf, n, 0);                
			}
			
		} else {          
			
			close(s);
			continue;
		}
		
		sleep(1);
	}
	return EXIT_SUCCESS;
}
