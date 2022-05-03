#include <iostream>
using namespace std;
int clocks[9];
int step[9];
void clock_ans(int i1,int i2,int i3){
    int i4 = (4 - ((i1 + i2 + clocks[0]) % 4)) % 4;//A
	int i5 = (4 - ((i1 + i2 + i3 + clocks[1]) % 4)) % 4;//B
	int i6 = (4 - ((i2 + i3 + clocks[2]) % 4)) % 4;//C
	int i7 = (4 - ((i1 + i4 + i5 + clocks[3]) % 4)) % 4;//D
	int i9 = (4 - ((i1 + i3 + i5 + i7 + clocks[4]) % 4)) % 4;//E
	int i8 = (4 - ((i6 + i9 + clocks[8]) % 4)) % 4;//I
    if((clocks[5] + i3 + i5 + i6 + i9) % 4 == 0 &&//F
		(clocks[6] + i4 + i7 + i8) % 4 == 0 &&//G
		(clocks[7] + i5 + i7 + i8 + i9) % 4 == 0//H)
    ){
        int temp[9]={i1,i2,i3,i4,i5,i6,i7,i8,i9};
        int sum=0,ans=0;
        for (int i=0;i<9;i++){
            sum+=temp[i];
            ans+=step[i];
        }
        if(sum<ans){
            memcpy(step,temp,sizeof(step));
        }
    }
}
void ans(){
    for (int i1=0;i1<4;i1++){
        for (int i2=0;i2<4;i2++){
            for (int i3=0;i3<4;i3++){
                clock_ans(i1,i2,i3);
            }
        }
    }
}
int main(){
    freopen("1.in","r",stdin);
    freopen("1.out","w",stdout);
    for (int i=0;i<9;i++){
        cin>>clocks[i];
        step[i]=4;
    }
    ans();
    for (int i=0;i<9;i++){
        if(step[i]!=0){
            for (int j=0;j<step[i];j++){
                cout<<i+1<<" ";
            }
        }
    }
    return 0;
}
