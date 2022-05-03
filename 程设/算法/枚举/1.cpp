#include <iostream>
#include <cstring>
using namespace std;
#define inf_max 0x7fffffff
int board[17][17];
int press[17][17];
int n;
int guess(){
    int step=0;
    for (int i=1;i<=n;i++){
        if(press[1][i]==1) step++;
    }
    for (int i=2;i<=n;i++){
        for (int j=1;j<=n;j++){
            press[i][j]=(board[i-1][j]+press[i-1][j]+press[i-1][j-1]+press[i-1][j+1]+press[i-2][j])%2;
            if(press[i][j]==1) step++;
        }
    }
    for (int i=1;i<=n;i++){
        if(board[n][i]!=(press[n][i]+press[n-1][i]+press[n][i-1]+press[n][i+1])%2) return inf_max;
    }
    return step;
}
void enumerate(){
    int step=inf_max;
    int c;
    int s=(1<<n);
    while(s--){
        step=min(guess(),step);
        c=1;
        press[1][c]++;
        while(press[1][c]>1){
            press[1][c]=0;
            c++;
            press[1][c]++;
        }
    }
    if(step==inf_max) cout<<"inf";
    else cout<<step;
}
int main(){

    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);

    memset(press,0,sizeof(press));
    memset(board,0,sizeof(board));

    char c;
    cin>>n; 
    for (int i=1;i<=n;i++){
        for (int j=1;j<=n;j++){
           cin>>c;
           if(c=='w') board[i][j]=1;
           else board[i][j]=0;
        }
    }

    enumerate();

    return 0;
}