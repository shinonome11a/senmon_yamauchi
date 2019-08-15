#include <stdio.h>
#include <time.h>
int main(void){
   time_t timer;
   struct tm *t_st;
   char s[128];
   char p[3] = "hog";
   /* 現在時刻の取得 */
   time(&timer);

   /* 現在時刻を文字列に変換して表示 */
   printf("現在時刻: %s\n", ctime(&timer));

   /* 現在時刻を構造体に変換 */
   t_st = localtime(&timer);
   printf("月: %d\n",t_st->tm_mon+1);   /* 月は＋１ */
   printf("日: %d\n",t_st->tm_mday);
   printf("時: %d\n",t_st->tm_hour);
   printf("分: %d\n",t_st->tm_min);
   printf("秒: %d\n",t_st->tm_sec);

   strftime(s, 128, "%d/%b/%Y:%T %z", t_st);

   printf("%s\n", s);

}
