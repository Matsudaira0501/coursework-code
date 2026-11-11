#include <stdio.h>
#include <math.h>

/*1.3 考察
前半のcos(x)のラグランジュ補完では12点では大体の補完しかできていなかったが、
120点にすると綺麗に補完することができていたように感じられる。
しかし、f(x)の後半では1,-1と極端に変化した際に補完したグラフがf(x)から大きくはずれてしまった。
また、どの補完においても区分的にラグランジュ補完をしたときにできるつなぎ目の部分で、
関数が滑らかにつながっていないといった問題も見受けられた。
これらの問題を解決するためには、スプライン補完を用いる必要があると考えられる。
*/

double l_j (double x, int j, int N);
double l_k (double x, int j, int N, double x_k[], double y_k[], int t);

int N = 4;

//サンプリング点の設定
//1~4の4点を選択した。
double  x_j[4] = {3.08, 3.36, 3.64, 3.92};

double  y_j[4] = {2.25, 1.91, 1.46, 0.94};

//補間計算
int main(void){

    int i, j, k;

    //1000回のループでyに様々なxを代入する。
    for(i=0; i<1000; i++ ){
        double x = 3.0 + (1 / 1000.00) * (double)i;
        double y = 0.0;
        //3次ラグランジュ補完
        for(j=0; j<N; j++) 
        {
            y = y + y_j[j] * l_j(x, j, N);  
        }
        //yが1に近いときのx, yの値をそれぞれ表示する。
        if(0.999 < y && y < 1.001){
                printf("x = %f\n", x);
                printf("y = %f\n", y);
                }
    }
//表示された値からyの値が1に最も近いものを探し、
//y=1となるようなxを有効数字3桁にする。
printf("以上より、1.1の答えは x = 3.89\n");  

    double  x_k[120] = {};

    double  y_k[120] = {};

    //12分割したときのx, yの値をリストに代入。
    //12点を抽出する際にまず0~8πの区間を12分割し、それぞれの区間での中央値を抽出した点とした。120点の時も同様。
    for(k=0; k<12; k++){
        double x1 = (((M_PI * 2) * (double)(k+1) - M_PI) / 3);
        x_k[k] = x1;
        double y1 = 0.0;
        
        if((0 <= x1) && (x1 < 4 * M_PI)){
            double y1 = cos(x1);
            y_k[k] = y1;
        }
        else if((4 * M_PI <= x1 && x1< 5 * M_PI) || (6 * M_PI <= x1 && x1 < 7 * M_PI)){
            double y1 = 1;
            y_k[k] = y1;
        }
        else {
            double y1 = -1;
            y_k[k] = y1;
        }
    }

//各グラフについて1200点をプロットし、csvファイルで出力しExcelでグラフにした。
//以下のコードからわかるように
//kaiseki4-1.csvが12点抽出時の3次ラグランジュ補完
//kaiseki4-2.csvが12点抽出時の5次ラグランジュ補完
//kaiseki4-3.csvが120点抽出時の3次ラグランジュ補完
//kaiseki4-4.csvが120点抽出時の5次ラグランジュ補完
//kaiseki4-5.csvが元の関数 である。

FILE *fp;
//12点3次のラグランジュ補完
fp = fopen("kaiseki4-1.csv", "w");
    if(fp == NULL){
        printf("file not open");
        return -1;
    }

    for(i=0; i<1200; i++){
        double x =  (4 * M_PI / 1200.00) + (8 * M_PI / 1200.00) * (double)i;
        double y = 0.0;
        
        if(i<400){
        for(j=0; j<N; j++)
        {
            y = y + y_k[j] * l_k(x, j, N, x_k, y_k, 0);
        }
        fprintf(fp,"%f,%f\n",x,y);
        }
        
        else if(400 <= i && i < 800){
        for(j=4; j<N+4; j++)
        {
            y = y + y_k[j] * l_k(x, j, N, x_k, y_k, 4);
        }
        fprintf(fp,"%f,%f\n",x,y);
        }

        else{
        for(j=8; j<N+8; j++)
        {
            y = y + y_k[j] * l_k(x, j, N, x_k, y_k, 8);
        }
        fprintf(fp,"%f,%f\n",x,y);    
        }
    }    

fclose(fp);

N = 6;

FILE *fp1;
//12点5次のラグランジュ補完
fp1 = fopen("kaiseki4-2.csv", "w");
    if(fp1 == NULL){
        printf("file not open");
        return -1;
    }

    for(i=0; i<1200; i++){
        double x =  (4 * M_PI / 1200.00) + (8 * M_PI / 1200.00) * (double)i;
        double y = 0.0;

        if(i<600){
        for(j=0; j<N; j++)
        {
            y = y + y_k[j] * l_k(x, j, N, x_k, y_k, 0);
        }
        fprintf(fp1,"%f,%f\n",x,y);
        }
        
        else{
        for(j=6; j<N+6; j++)
        {
            y = y + y_k[j] * l_k(x, j, N, x_k, y_k, 6);
        }
        fprintf(fp1,"%f,%f\n",x,y);
        }
    
    }   
fclose(fp1);

//120点を抽出しリストに代入。
for(k=0; k<120; k++){
        double x1 = (((M_PI * 2) * (double)(k+1) - M_PI) / 30.00);
        x_k[k] = x1;
        double y1 = 0.0;
        
        if(0 <= x1 && x1 < 4 * M_PI){
            double y1 = cos(x1);
            y_k[k] = y1;
        }
        else if((4 * M_PI <= x1 && x1< 5 * M_PI)||(6 * M_PI <= x1 && x1 < 7 * M_PI)){
            double y1 = 1;
            y_k[k] = y1;
        }
        else{
            double y1 = -1;
            y_k[k] = y1;
        }
    }

N = 4;

FILE *fp2;
//120点3次のラグランジュ補完
fp2 = fopen("kaiseki4-3.csv", "w");
    if(fp2 == NULL){
        printf("file not open");
        return -1;
    }

    int t = 0;
    for(i=0; i<1200; i++){
        double x = (((M_PI * 2) * (double)(i+1) - M_PI) / 300.00);
        double y = 0.0;
            
        //x0~x39まではx_k[0~3],y_k[0~3]から補完された関数yに代入、x40~x79までは...というようなループ
        for(j=t; j<N+t; j++)
        {
            y = y + y_k[j] * l_k(x, j, N, x_k, y_k, t);
        }
        if(i % 40 == 39){t += 4;}
        
        fprintf(fp2,"%f,%f\n",x,y);
    }   

fclose(fp2);

N = 6;

FILE *fp3;
//120点5次のラグランジュ補完
fp3 = fopen("kaiseki4-4.csv", "w");
    if(fp3 == NULL){
        printf("file not open");
        return -1;
    }

    t = 0;
    for(i=0; i<1200; i++){
        double x = (((M_PI * 2) * (double)(i+1) - M_PI) / 300.00);
        double y = 0.0;
            
        //x0~x59まではx_k[0~5],y_k[0~5]から補完された関数yに代入、x60~x119までは...というようなループ
        for(j=t; j<N+t; j++)
        {
            y = y + y_k[j] * l_k(x, j, N, x_k, y_k, t);
        }
        if(i % 60 == 59){t += 6;}
        
        fprintf(fp3,"%f,%f\n",x,y);
    }   

fclose(fp3);


FILE *fpa;
//元の関数
fpa = fopen("kaiseki4-5.csv", "w");
    if(fpa == NULL){
        printf("file not open");
        return -1;
    }

for(k=0; k<1200; k++){
        double x1 = (((M_PI * 2) * (double)(k+1) - M_PI) / 300.00);
        double y1 = 0.0;

        if(0 <= x1 && x1 < 4 * M_PI){
            double y1 = cos(x1);
            fprintf(fpa,"%f,%f\n", x1, y1);
        }
        else if((4 * M_PI <= x1 && x1< 5 * M_PI)||(6 * M_PI <= x1 && x1 < 7 * M_PI)){
            double y1 = 1;
            fprintf(fpa,"%f,%f\n", x1, y1);
        }
        else{
            double y1 = -1;
            fprintf(fpa,"%f,%f\n", x1, y1);
        }
        
    }

fclose(fpa);

return 0;
}

double l_j (double x, int j, int N)
{
int i;
double x_u, x_l;
x_u = 1.0;
x_l = 1.0;
    for (i=0; i<N; i++){
        if (j != i) {
            x_u = x_u * ( x - x_j[i] );
            x_l = x_l * ( x_j[j] - x_j[i] );
        }
    }
return ( x_u / x_l );
}

double l_k (double x, int j, int N, double x_k[], double y_k[], int t)
{
int i;
double x_u, x_l;
x_u = 1.0;
x_l = 1.0;
    for (i=0; i<N; i++){
        if (j != i+t){
            x_u = x_u * ( x - x_k[i+t] );
            x_l = x_l * ( x_k[j] - x_k[i+t] );
        }
    }
return ( x_u / x_l );
}