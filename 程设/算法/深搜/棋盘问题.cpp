#include <cstring>
#include <iostream>
using namespace std;
int n,k,ans;
char board[8][8];
bool stateR[8],stateC[8];
void dfs(int cur,int number){
    if(cur==n*n) {
        if(number==0){
            ans++;
        }
        return;
    }
    int x=cur/n;
    int y=cur%n;
    if(board[x][y]=='#'&&stateR[x]==0&&stateC[y]==0){
        stateR[x]=1;
        stateC[y]=1;
        dfs(cur+1,number-1);
        stateR[x]=0;
        stateC[y]=0;
        dfs(cur+1,number);
    }else{
        dfs(cur+1,number);
    }
}
int main(){
    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    while(cin>>n>>k&&n!=-1&&k!=-1){
        memset(board,0,sizeof(board));
        memset(stateR,0,sizeof(stateR));
        memset(stateC,0,sizeof(stateC));
        ans=0;
        for (int i=0;i<n;i++){
            for (int j=0;j<n;j++){
                cin>>board[i][j];
            }
        }
        dfs(0,k);
        cout<<ans<<endl;
    }
    return 0;
}
