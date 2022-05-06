#include <iostream>
using namespace std;
#define INTMAX 1<<30
const int N=17;
const int MAX=1<<16;
int map[N][N];  
int dp[MAX][N];
int main(){
    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    int n;
    cin>>n;
    for (int i=1;i<=n;i++){
        for (int j=1;j<=n;j++){
            cin>>map[i][j];
        }
    }
    int num=1<<n;
    for (int s=1;s<num;s++){
        for (int i=1;i<=n;i++){
            dp[s][i]=INTMAX;
        }
    }
    dp[1][1]=0;
    for (int s=1;s<num;s++){
        for (int i=2,i_=2;i<=n;i++,i_<<=1){
            if(s&i_){
                for (int j=1,j_=1;j<n;j++,j_<<=1){
                    if(s&j_){
                        dp[s][i]=min(dp[s][i],dp[s-i_][j]+map[j][i]);
                    }
                }
            }
        }
    }
    cout<<dp[num-1][n];

    return 0;
}
