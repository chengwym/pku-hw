#include <iostream>
#include <cstring>
using namespace std;
#define int_max 0x7fffffff
int beg[31];
int ans[31];
int press[31];
int n;
void enumerate(){
    int step=int_max;
    for (int i=0;i<2;i++){
        int temp=0;
        press[1]=i;
        if(press[1]==1) temp++;
        for (int j=2;j<=n;j++){
            press[j]=(beg[j-1]+ans[j-1]+press[j-2]+press[j-1])%2;
            if(press[j]==1) temp++;
        }
        if(ans[n]==(press[n]+press[n-1]+beg[n])%2){
            step=min(step,temp);
        }
    }
    if(step==int_max) cout<<"impossible";
    else cout<<step;
}
int main(){
    freopen("1.in","r",stdin);
    freopen("1.out","w",stdout);
    string temp,temp1;
    cin>>temp>>temp1;
    for (int i=0;i<temp.size();i++){
        beg[i+1]=temp[i]-'0';
        ans[i+1]=temp1[i]-'0';
    }
    n=temp.size();
    enumerate();
    return 0;
}
