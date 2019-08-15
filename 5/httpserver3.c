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
   char scode[1024]; // "HTTP/1.0 200 OK\n"が格納される
   char filepass[1024];
   char filename[1024];
   char request[128];
   char eoh[] = "\n"; // HTTPヘッダの終わり
   FILE *fp; // HTTPで返すファイルのポインタ
   

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
   
   // ソケットを確保
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   // 構造体caを用意
   ca.sin_family = AF_INET;
   ca.sin_addr.s_addr = htonl(INADDR_ANY);
   ca.sin_port = htons(port);
   
   //bindし、戻り値が負の場合bindエラーを返して終了
   if(bind(sockfd, (struct sockaddr *)&ca, sizeof(ca)) == -1){
      perror("bind");
      return EXIT_FAILURE;
   }
   
   // listen()でクライアントからの通信を待ち受ける
   listen(sockfd, 5);
   
   while(1){
      len = sizeof(ca);
      
      // accept()で接続を受け入れ、接続されるとaccept()を抜ける
      s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
      
      // forkしてプロセスIDを変数pidに格納
      int pid = fork();
      if(pid == 0){
         printf("connected from %s:%d\n",inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
         
         // ブラウザから送られてきたデータを受け取る。
         n = recv(s, buf, sizeof(buf), 0);
         buf[n] = 0;
         
         printf("buf.len:%ld\n%s\n",sizeof(buf), buf);
         
         // 送られてきたデータをメソッドとファイルパスに分離する
         sscanf(buf,"%s %s",request,filepass);
         // ファイルパスが"/"もしくは何もなかったっ場合はindex.htmlを返す。
         if(strcmp(filepass,"/") == 0 || strcmp(filepass,"") == 0){
            strcpy(filepass,"/index.html");
         }

         // ドキュメントルートを指定してファイルパスに付け加える
         sprintf(filename,"htdocs%s", filepass);
         printf("request: %s %s\n", request, filename);
         
         // 指定されたファイルを読み取り専用で開く
         fp = fopen(filename, "r");
         
         if(fp == NULL){
            // ファイルのオープンに失敗した時はステータスコード404とそれ用のファイルを返す
            strcpy(scode, "HTTP/1.0 404 NOT FOUND\n");
            fp = fopen("htdocs/404.html", "r");
         }else{
            // ファイルのオープンに成功した時はステータスコード200を返す。
            strcpy(scode, "HTTP/1.0 200 OK\n");
         }
         
         // ステータスコードと空行をあらかじめ送信
         send(s, scode, strlen(scode), 0);
         send(s, eoh, strlen(eoh), 0);

         // 1バイトずつ読み込み、読めなくなるまで読めたバイト数だけ送信
         while((n = fread(buf, 1, 1024, fp)) > 0){
            send(s, buf, n, 0);
         }

         fclose(fp);
         close(s);
         
      }else{
         continue;
      }
   }
   return EXIT_SUCCESS;
}
