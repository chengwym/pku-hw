#include <iostream>
#include <cstring>
using namespace std;
#define conflict(a,b) a&b
const int MAXN=105;
const int MAXM=15;
int n,m;
int hill[MAXN];
int dp[MAXN][70][70];
int state[70];
char g[MAXN][MAXM];
int number[MAXN];
int main(){

    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    memset(hill,0,sizeof(hill));
    memset(dp,0,sizeof(dp));
    memset(state,0,sizeof(state));
    memset(number,0,sizeof(number));

    cin>>n>>m;
    for(int i=0;i<n;i++){
        cin>>g[i];
        for (int j=0;j<m;j++){
            if(g[i][j]=='H') hill[i]+=(1<<j);
        }
    }

    int dex=0;
    for (int i=0;i<(1<<m);i++){
        if(conflict(i,i<<1)||conflict(i,i<<2)) continue;
        int temp=i;
        while(temp){
            number[dex]+=(temp&1);
            temp=temp>>1;
        }
        state[dex++]=i;
    }

    for (int i=0;i<dex;i++){
        if(conflict(hill[0],state[i])) continue;
        dp[0][i][0]=number[i];
    }

    for (int i=0;i<dex;i++){
        if(conflict(hill[1],state[i])) continue;
        for (int j=0;j<dex;j++){
            if(conflict(state[i],state[j])) continue;
            if(conflict(state[j],hill[0])) continue;
            dp[1][i][j]=max(dp[0][j][0]+number[i],dp[1][i][j]);
        }
    }

    for (int r=2;r<n;r++){
        for (int i=0;i<dex;i++){
            if(conflict(hill[r],state[i])) continue;
            for (int j=0;j<dex;j++){
                if(conflict(state[i],state[j])) continue;
                if(conflict(hill[r-1],state[j])) continue;
                for (int k=0;k<dex;k++){
                    if(conflict(hill[r-2],state[k])||conflict(state[k],state[j])||conflict(state[k],state[i])) continue;
                    dp[r][i][j]=max(dp[r-1][j][k]+number[i],dp[r][i][j]);
                }
            }
        }
    }

    int ans=0;
    for (int i=0;i<dex;i++){
        for (int j=0;j<dex;j++){
            ans=max(ans,dp[n-1][i][j]);
        }
    }
    
    printf("%d",ans);

    return 0;
}
