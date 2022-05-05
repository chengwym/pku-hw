#include <iostream>
using namespace std;
const int maxr=105;
int R,C;
int dp[maxr][maxr];
int h[maxr][maxr];
int direction[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
int dfs(int x,int y){
    if(dp[x][y]!=0) return dp[x][y];
    dp[x][y]=1;
    for (int i=0;i<4;i++){
        int newx=x+direction[i][0];
        int newy=y+direction[i][1];
        if(newx<0||newx>=R||newy<0||newy>=C||h[x][y]<=h[newx][newy]) continue;
        dp[x][y]=max(dfs(newx,newy)+1,dp[x][y]);
    }
    return dp[x][y];
}
int main(){
    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    scanf("%d%d",&R,&C);
    for (int i=0;i<R;i++){
        for (int j=0;j<C;j++){
            scanf("%d",&h[i][j]);
        }
    }   
    int ans=0;
    for (int i=0;i<R;i++){
        for (int j=0;j<C;j++){
            ans=max(ans,dfs(i,j));
        }
    }
    cout<<ans;
    return 0;
}
