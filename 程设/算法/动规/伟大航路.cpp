#define INF 1<<30
#define MAXN 16
#include <iostream>
#include <cstring>
using namespace std;
int map[MAXN][MAXN];
int n;
int mindist=INF;
unsigned int dp[14][1<<14];
inline bool vis(int city,int state){
    if(state&(1<<city-1)){
        return true;
    }
    return false;
}
int dfs(int dist,int num,int crt,int state){
    if(dist>=mindist){
        return 0;
    }
    if(num==n-1){
        dist+=map[crt][n-1];
        if(dist<mindist){
            mindist=dist;
        }
        return 0;
    }
    for (int i=1;i<n-1;i++){
        if(vis(i,state)==0){
            int newstate=state|(1<<(i-1));
            if(crt==0){
                dp[i][newstate]=map[crt][i];
                dfs(dist+map[crt][i],num+1,i,newstate);
            }else if(dp[i][newstate]>dp[crt][state]+map[crt][i]){
                dp[i][newstate]=dp[crt][state]+map[crt][i];
                dfs(dist+map[crt][i],num+1,i,newstate);
            }
        }
    }
}
int main(){
    freopen("1.in","r",stdin);
    freopen("1.out","w",stdout);
    scanf("%d",&n);
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            scanf("%d",&map[i][j]);
        }
    }
    memset(dp,-1,sizeof(dp));
    dfs(0,1,0,0);
    printf("%d\n",mindist);

    return 0;
}
