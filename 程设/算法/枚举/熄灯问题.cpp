#include <iostream>
#include <cstring>
using namespace std;
int press[6][8];
int puzzle[6][8];
bool guess(){
    for (int i=2;i<=5;i++){
        for (int j=1;j<=6;j++){
            press[i][j]=(press[i-1][j]+puzzle[i-1][j]+press[i-1][j-1]+press[i-1][j+1]+press[i-2][j])%2;
        }
    }
    for (int i=1;i<=6;i++){
        if(puzzle[5][i]!=(press[5][i]+press[5][i-1]+press[5][i+1]+press[4][i])%2) return false;
    }
    return true;
}
void enumerate(){
    for(int i=1;i<=6;i++){
        press[1][i]=0;
    }
    int c;
    while(guess()==false){
        press[1][1]++;
        c=1;
        while(press[1][c]>1){
            press[1][c]=0;
            c++;
            press[1][c]++;
        }
    }
}
int main(){

    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);


    memset(press,0,sizeof(press));
    memset(puzzle,0,sizeof(puzzle));
    for (int i=1;i<=5;i++){
        for (int j=1;j<=6;j++){
            cin>>puzzle[i][j];
        }
    }
    enumerate();
    for (int i=1;i<=5;i++){
        for (int j=1;j<=6;j++){
            cout<<press[i][j]<<" ";
        }
        cout<<endl;
    }


    return 0;
}
