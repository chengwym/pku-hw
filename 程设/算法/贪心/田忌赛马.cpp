#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=1005;
int a[N],b[N];
bool statea[N];
int main(){
    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    int n;
    while(cin>>n&&n){
        memset(a,0,sizeof(a));
        memset(b,0,sizeof(b));
        memset(statea,0,sizeof(statea));
        for(int i=0;i<n;i++){
            cin>>a[i];
        }
        for (int i=0;i<n;i++){
            cin>>b[i];
        }
        sort(a,a+n);
        sort(b,b+n);
        int num=0,Tmin=0,Kmin=0,Tmax=n-1,Kmax=n-1;
        while(Tmin<=Tmax){
            if(a[Tmin]<b[Kmin]){
                Tmin++;
                Kmax--;
                num--;
            }else if(a[Tmin]>b[Kmin]){
                Tmin++;
                Kmin++;
                num++;
            }else{
                if(a[Tmax]>b[Kmax]){
                    Tmax--;
                    Kmax--;
                    num++;
                }else if(a[Tmax]<b[Kmax]){
                    Kmax--;
                    Tmin++;
                    num--;
                }else{
                    if(a[Tmin]==b[Kmax]){
                        break;
                    }else if(a[Tmin]<b[Kmax]){
                        Tmin++;
                        Kmax--;
                        num--;
                    }
                }
            }
        }
        cout<<num*200<<endl;
    }



    return 0;
}
