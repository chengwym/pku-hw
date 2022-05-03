#include <iostream>
using namespace std;
int getCoins(int pay,int days){
    if(pay>=days) return pay*days;
    return pay*pay+getCoins(pay+1,days-pay);
}
int main(){

    freopen("1.in","r",stdin);
    freopen("1.out","w",stdout);

    int Y;
    while(cin>>Y&&Y){
        cout<<Y<<" "<<getCoins(1,Y)<<endl;
    }


    return 0;
}
