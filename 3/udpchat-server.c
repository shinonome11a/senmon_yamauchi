#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFSIZE 512


struct addrlist{
   int time;
   struct sockaddr_in ap;
   struct addrlist *next;
};


struct addrstart{
   struct addrlist *next;
};
struct addrstart start;


void showAddrList(){
   struct addrlist *now = (struct addrlist *)&start;
   if(now->next != NULL){
      now = now->next;
      printf("Clients List\n");
   }else{
      printf("No Clients\n");
      return;
   }
   while (now != NULL){
      printf("  %s:%d %d\n", inet_ntoa(now->ap.sin_addr),ntohs(now->ap.sin_port),now->time);
      now = now->next;
   }
}

struct addrlist *makeCell(struct sockaddr_in pa){
   struct addrlist *new = malloc(sizeof(struct addrlist));
   if(new != NULL){
      new->time = time(NULL);
      new->ap = pa;
      printf("make: %s:%d %d\n", inet_ntoa(new->ap.sin_addr),ntohs(new->ap.sin_port), new->time);
      new->next = NULL;
   }
   return new;
}

void deleteCell(int n){
   struct addrlist *now = (struct addrlist *)&start;
   for(int i = 1; i < n; i++){
      now = now->next;
   }
   struct addrlist *tmp = now->next;;
   now->next = now->next->next;
   printf("delete: %s:%d %d\n", inet_ntoa(tmp->ap.sin_addr),ntohs(tmp->ap.sin_port), tmp->time);
   free(tmp);
}


int main(int argc, char *argv[]){
   int sockfd, n;
   struct sockaddr_in ca, sa;
   char sendline[BUFSIZE], recvline[BUFSIZE + 1];
   int casize = sizeof(ca);
   struct addrlist *now;
   start.next = NULL;

   sockfd = socket(AF_INET, SOCK_DGRAM, 0);

   bzero( (char *)&ca, sizeof(ca));
   ca.sin_family = AF_INET;
   ca.sin_addr.s_addr = htonl(INADDR_ANY);
   ca.sin_port = htons(59001);

   printf("Welcome to UDP CHAT SERVER ver1.0 server port is 59001\n");

   bind(sockfd, (struct sockaddr *)&ca, sizeof(ca));
   while(1){
      n = recvfrom(sockfd, recvline, BUFSIZE, 0, (struct sockaddr *)&ca, (socklen_t *)&casize);
      recvline[n] = '\0';
      printf("%s:%d %ld --> ", inet_ntoa(ca.sin_addr),ntohs(ca.sin_port), time(NULL));
      fputs(recvline, stdout);
      fflush(stdout);
      now = (struct addrlist *)&start;
      int flag = 0;
      while(now->next != NULL){
         if(now->next->ap.sin_addr.s_addr == ca.sin_addr.s_addr){
            now->next->time = time(NULL);
            flag = 1;
            break;
         }
         now = now->next;
      }
      if(flag == 0){
         now->next = makeCell(ca);
         showAddrList();
      }

      bzero( (char *)&sa, sizeof(sa));
      sa.sin_family = AF_INET;

      snprintf(sendline,BUFSIZE,"%s%s%s",inet_ntoa(ca.sin_addr)," --> ",recvline);
      memset(recvline, '\0',BUFSIZE + 1);
      n = strlen(sendline);
      now = (struct addrlist *)&start;
      int j = 1;
      while(now->next != NULL){
         if(time(NULL) - now->next->time <= 30){
            sa.sin_addr.s_addr = now->next->ap.sin_addr.s_addr;
            sa.sin_port = htons(59001);
            sendto(sockfd, sendline, n, 0, (struct sockaddr *)&sa, sizeof(sa));
            now = now->next;
         }else{
            deleteCell(j);
            showAddrList();
         }
         j++;
      }
   }
   return 0;
}
