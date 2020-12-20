#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
	//ファイル構造体へのポインタを宣言
	FILE *fp;
	int element = 100; //配列の要素数
	
	//時間保存用変数
	double dayTime; //桁数が多いのでdouble型
	double year[element];
	double month[element];
	double day[element];
	double hour[element];
	double minute[element];
	double seconds[element];
	
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
	double ping[element];//応答結果を数値で保存する
	
	int timeOutCount;//タイムアウトした回数を記録
	int firstErrorFlag = 0;//最初のエラーのフラグ
	int flag = 0;//フラグ処理用変数
	int N; //許容できるタイムアウトの回数
	int m; //t直近m回
	int t; //平均応答時間の閾値
	double averageResponseTime = 0;//平均応答時間
	int elementNum = 0;//配列要素番号
	int i;
	
	printf("許容できるタイムアウトの回数Nを入力してください\n");
	scanf("%d\n",&N);
	//エラー処理
	if(N < 0){
	    printf("エラー、もう一度許容できるタイムアウトの回数Nを入力してください\n");
	    scanf("%d\n",&N);
	}
	
	printf("mを入力してください\n");
	scanf("%d\n",&m);
	//エラー処理
	if(m < 0){
	    printf("エラー、もう一度mを入力してください\n");
	    scanf("%d\n",&m);
	}
	
	printf("平均応答時間tを入力してください\n");
	scanf("%d\n",&t);
	if(t < 0){
	    printf("エラー、もう一度平均応答時間tを入力してください\n");
	    scanf("%d\n",&t);
	}
	
	printf("N:%d m:%d t:%d\n",N,m,t);

	//rog.txtをオープン
	fp = fopen("rog.txt","r");

	//ファイルオープンに失敗したとき
	if(fp==NULL){
		//失敗を表示し終了
		printf("ファイルオープン失敗\n");
		return -1;
	}

	//カンマ区切りごとに読み込み
	while(fscanf(fp, "%lf,%[^,],%s", &dayTime, adress, p) != EOF) {
	    year[elementNum] = dayTime / 10000000000;
	    month[elementNum] = fmod((dayTime / 100000000) ,100);
	    day[elementNum] = fmod((dayTime / 1000000) ,100);
	    hour[elementNum] = fmod((dayTime / 10000) ,100);
	    minute[elementNum] = fmod((dayTime / 100) ,100);
	    seconds[elementNum] = fmod(dayTime ,100);
	    
	    //フラグが１の場合
	    if(flag == 1){
	        ping[elementNum] = atof(p);//文字列を数値に変換
	        
	        //printf("%d\n",ping[elementNum]);
	        //printf("%.0f年%.0f月%.0f日%.0f時%.0f分%.0f秒\n",year,month,day,hour,minute,seconds);//数値確認用
	        
	        //現在のデータの時間からエラーが起きた時間をマイナスし、故障期間を求める
	        printf("故障期間：%.0f年%.0f月%.0f日%.0f時%.0f分%.0f秒\n"
	        ,year[elementNum] - errorTime_year
	        ,month[elementNum] - errorTime_month
	        ,day[elementNum] - errorTime_day
	        ,hour[elementNum] - errorTime_hour
	        ,minute[elementNum] - errorTime_minute
	        ,seconds[elementNum] - errorTime_seconds);
	        
	        //フラグのリセット
	        timeOutCount = 0;
	        flag = 0;
	        firstErrorFlag = 0;
	    }
	    
	    //エラーが出た場合
	    if(p[0] == '-'){
	        //printf("%.0f,%s,%s\n", dayTime, adress, p); //数値確認用
	        ping[elementNum] = 0;
	        //printf("%.0f年%.0f月%.0f日%.0f時%.0f分%.0f秒\n",year,month,day,hour,minute,seconds);//数値確認用
	        
	        //最初のエラーが起きた時間を記録
	        if(firstErrorFlag == 0){
	            errorTime = dayTime;
	            errorTime_year = year[elementNum];
	            errorTime_month = month[elementNum];
	            errorTime_day = day[elementNum];
	            errorTime_hour = hour[elementNum];
	            errorTime_minute = minute[elementNum];
	            errorTime_seconds = seconds[elementNum];
	            /*printf("故障前期間：%.0f年%.0f月%.0f日%.0f時%.0f分%.0f秒\n"
	            ,errorTime_year
	            ,errorTime_month
	            ,errorTime_day
	            ,errorTime_hour
	            ,errorTime_minute
	            ,errorTime_seconds);
	            */
	            firstErrorFlag = 1;
	        }
	        
	        elementNum ++;
	        timeOutCount++;
	        if(timeOutCount == N){
	            printf("故障状態のサーバーアドレス:%s\n", adress);
	            flag = 1;
	        }
	    } else { 
	        timeOutCount = 0;//エラーが起きなかった場合タイムアウトのカウントを0に
	        ping[elementNum] = atof(p);
	        
	        if(elementNum - m >= 0){
	            for(i = elementNum - m; i < elementNum;i++){
	                averageResponseTime += ping[i];
	            }
	            if(averageResponseTime >= t){
	                printf("平均応答時間:%.0f\n",averageResponseTime);
	                printf("負荷状態となっている期間：%.0f年%.0f月%.0f日%.0f時%.0f分%.0f秒\n"
	                ,year[elementNum - 1] - year[elementNum - m]
	                ,month[elementNum - 1] - month[elementNum - m]
	                ,day[elementNum - 1] - day[elementNum - m]
	                ,hour[elementNum - 1] - hour[elementNum - m]
	                ,minute[elementNum - 1] - minute[elementNum - m]
	                ,seconds[elementNum - 1] - seconds[elementNum - m]);
	                averageResponseTime = 0;
	            } else {
	                //printf("平均応答時間:%.0f\n",averageResponseTime);//数値確認用
	                averageResponseTime = 0;
	            }
	        } 
	        elementNum ++;
	        
	    /*//数値確認用
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