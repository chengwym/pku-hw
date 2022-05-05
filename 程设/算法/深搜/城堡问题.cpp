#include <iostream>
using namespace std;
int n,m;
const int MAXM=55;
const int MAXN=55;
int score[MAXN][MAXM];
int state[MAXN][MAXM];
int direction[4][2]={{0,-1},{-1,0},{0,1},{1,0}};
int depth;
void dfs(int x,int y){
    for (int i=0;i<4;i++){
        int newx=x+direction[i][0];
        int newy=y+direction[i][1];
        if(newx<0||newx>=n||newy<0||newy>=m||(score[x][y]&(1<<i))||state[newx][newy]==1) continue;
        depth++;
        state[newx][newy]=1;
        dfs(newx,newy);
    }
}
int main(){
    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;i++){
        for (int j=0;j<m;j++){
            scanf("%d",&score[i][j]);
        }
    }
    int dex=0,ans=0;
    for (int i=0;i<n;i++){
        for (int j=0;j<m;j++){
            if(state[i][j]==0){
                depth=0;
                dfs(i,j);
                ans=max(depth,ans);
                dex++;
            }
        }
    }
    cout<<dex<<endl<<ans;
    return 0;
}
