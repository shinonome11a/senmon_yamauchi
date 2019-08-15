#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(void){
   
   //元データ
   int orig = 0x1A2B3C4D;
   int net, host;

   printf("元データ：\t\t\t%x\n", orig);

   //ネットワークバイトオーダーへ変換
   net = ntohl(orig);
   printf("ネットワークバイトオーダー：\t%x\n", net);

   //変換したデータを/ホストバイトオーダーへ変換
   host = htonl(net);
   printf("ホストバイトオーダー：\t\t%x\n", host);

   return 0;
}
