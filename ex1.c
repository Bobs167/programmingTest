#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
	//ファイル構造体へのポインタを宣言
	FILE *fp;
	//時間保存用変数
	double dayTime; //桁数が多いのでdouble型
	double year;
	double month;
	double day;
	double hour;
	double minute;
	double seconds;
	
	//エラーが起きた時間を記録
	double errorTime;
	double errorTime_year;
	double errorTime_month;
	double errorTime_day;
	double errorTime_hour;
	double errorTime_minute;
	double errorTime_seconds;
	
	char adress[256];//サーバーアドレスを保存

	char p[10];//応答結果を一時保存
	int ping;//応答結果を数値で保存する
	
	int flag = 0;//フラグ処理用変数
	
	double failurePeriod;//故障期間

	//rog.txtをオープン
	fp = fopen("rog2.txt","r");

	//ファイルオープンに失敗したとき
	if(fp==NULL){
		//失敗を表示し終了
		printf("ファイルオープン失敗\n");
		return -1;
	}

	//カンマ区切りごとに読み込み
	while(fscanf(fp, "%lf,%[^,],%s", &dayTime, adress, p) != EOF) {
	    
	    //一つ前でエラーが出ていた場合
	    if(flag == 1){
	        year = dayTime / 10000000000;
	        //double型では演算子%が使用できないためfmod関数を使用
	        month = fmod((dayTime / 100000000) ,100);
	        day = fmod((dayTime / 1000000) ,100);
	        hour = fmod((dayTime / 10000) ,100);
	        minute = fmod((dayTime / 100) ,100);
	        seconds = fmod(dayTime ,100);
	        //printf("%.0f年%.0f月%.0f日%.0f時%.0f分%.0f秒\n",year,month,day,hour,minute,seconds);//数値確認用
	        
	        //現在のデータの時間からエラーが起きた時間をマイナスし、故障期間を求める
	        printf("故障期間：%.0f年%.0f月%.0f日%.0f時%.0f分%.0f秒\n",year - errorTime_year,month - errorTime_month,day - errorTime_day,hour - errorTime_hour,minute - errorTime_minute,seconds - errorTime_seconds);
	        flag = 0;
	    }
	    
	    //エラーが出た場合
	    if(p[0] == '-'){
	        //printf("故障状態のサーバーアドレス:%s\n", adress);
	        //printf("%.0f,%s,%s\n", dayTime, adress, p); //数値確認用
	        
	        //year = fmod(dayTime,100000000000);
	        year = dayTime / 10000000000;
	        month = fmod((dayTime / 100000000) ,100);
	        day = fmod((dayTime / 1000000) ,100);
	        hour = fmod((dayTime / 10000) ,100);
	        minute = fmod((dayTime / 100) ,100);
	        seconds = fmod(dayTime ,100);
	        printf("故障状態のサーバーアドレス:%s\n",adress);
	        //printf("%.0f年%.0f月%.0f日%.0f時%.0f分%.0f秒\n",year,month,day,hour,minute,seconds);//数値確認用
	        
	        //エラーが起きた時間を記録
	        errorTime = dayTime;
	        errorTime_year = year;
	        errorTime_month = month;
	        errorTime_day = day;
	        errorTime_hour = hour;
	        errorTime_minute = minute;
	        errorTime_seconds = seconds;
	        flag = 1;
	    } else { //数値確認用
	    /*
	        year = dayTime / 10000000000;
	        month = fmod((dayTime / 100000000) ,100);
	        day = fmod((dayTime / 1000000) ,100);
	        hour = fmod((dayTime / 10000) ,100);
	        minute = fmod((dayTime / 100) ,100);
	        seconds = fmod(dayTime ,100);
	        
	        ping = atof(p);
		    printf("%.0f,%s,%d\n", dayTime, adress, ping);
		    printf("%.0f年%.0f月%.0f日%.0f時%.0f分%.0f秒\n",year,month,day,hour,minute,seconds);
		    */
	    } 
	}

	//ファイルを閉じる
	fclose(fp);
	return 0;
}