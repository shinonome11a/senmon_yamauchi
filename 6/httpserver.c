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
#include <time.h>

#define DEFAULT_SERV_TCP_PORT 8080

// 関数原型宣言
int writeLog(char *addr, char *method, char *filename, char *http, int status);
char *logTime(void);

// アクセスログ書き出し用関数
int writeLog(char *addr, char *method, char *filename, char *http, int status){
   
   // アクセスログファイルのポインタ
   FILE *log;

   // アクセスログファイルを追記形式でオープン
   // オープンできなかった時は関数を終了
   if ((log = fopen("logs/access_log", "a+")) == NULL) {
      fprintf(stderr, "ログファイルのオープンに失敗しました.ログ出力無しで続行します.\n");
      return EXIT_FAILURE;
   }
   
   // アクセスログに必要な情報を書き込む
   fprintf(log,"%s - - [%s] \"%s %s %s\" %d -\r\n",addr, logTime(), method, filename, http, status);
   fclose(log);
   return EXIT_SUCCESS;

}

// 時刻をログ用にフォーマットを整えて返す
char *logTime(void){
   static char string[128]; // 返す文字列
   time_t timer; // UNIX時刻を格納する変数
   struct tm *timeptr; // UNIX時刻から変換する構造体
   int len = 128;

   // UNIX時刻を取得
   timer = time(NULL);

   // UNIX時刻を構造体に変換
   timeptr = localtime(&timer);

   // 時刻が取得できなかった時の処理
   if(string == NULL){
      fprintf(stderr, "時刻の取得に失敗しました．");
      return "----";
   }

   // 構造体から必要な形式の文字列に変換して変数stringに格納
   strftime(string, len, "%d/%b/%Y:%T %z",timeptr);

   return string;
}

int main(int argc, char *argv[]){
   int sockfd, len, s, n, port;
   struct sockaddr_in ca;
   char buf[1024];
   char scode[1024]; // = "HTTP/1.0 200 OK\n";
   char filepass[1024];
   char filename[1024];
   char http[128];
   char request[128];
   char eoh[] = "\n"; 
   FILE *fp;
   

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
   
   // プログラム起動時刻を表示(オプション)
   printf("%s\n", logTime());

   // ソケットを確保
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   // 構造体caを用意
   ca.sin_family = AF_INET;
   ca.sin_addr.s_addr = htonl(INADDR_ANY);
   ca.sin_port = htons(port);
   
   // bindし、戻り値が負の場合はbindエラーを返してプログラム終了
   if(bind(sockfd, (struct sockaddr *)&ca, sizeof(ca)) == -1){
      perror("bind");
      return EXIT_FAILURE;
   }
   
   // listen()でクライアントからの通信を待ち受ける
   listen(sockfd, 5);
   
   while(1){
      len = sizeof(ca);

      // accept()で接続を受け入れ、接続されるとaccesp()を抜ける
      s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
      
      // forkしてプロセスIDを変数pidに格納
      int pid = fork();
      if(pid == 0){
         printf("connected from %s:%d\n",inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
         
         // ブラウザから送られてきたデータを受け取る
         n = recv(s, buf, sizeof(buf), 0);
         buf[n] = 0;
            
         printf("buf.len:%ld\n%s\n",sizeof(buf), buf);
         
         // 送られてきたデータをメソッドとファイルパスに分離
         sscanf(buf,"%s %s %s",request,filepass,http);
         if(strcmp(filepass,"/") == 0 || strcmp(filepass,"") == 0){
            strcpy(filepass,"/index.html");
         }

         // ドキュメントルートを指定してファイルパスに付け加える
         sprintf(filename,"htdocs%s", filepass);
         printf("request: %s %s %s\n---\n", request, filename, http);
         
         // 指定されたファイルを読み取り専用で開く
         fp = fopen(filename, "r");
   
         if(fp == NULL){
            strcpy(scode, "HTTP/1.0 404 NOT FOUND\n");
            fp = fopen("htdocs/404.html", "r");

            // ログに出力
            writeLog(inet_ntoa(ca.sin_addr), request, filepass, http, 404);
         }else{
            strcpy(scode, "HTTP/1.0 200 OK\n");

            // ログを出力
            writeLog(inet_ntoa(ca.sin_addr), request, filepass, http, 200);
         }

         // ステータスコードと空行をあらかじめ送信
         send(s, scode, strlen(scode), 0);
         send(s, eoh, strlen(eoh), 0);
         
         // 1バイトずつ読み込み、読めなくなるまで読めたバイト数だけ送信
         int num = 0;
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
