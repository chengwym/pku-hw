#include <iostream>
#include <vector>
using namespace std;
bool statePrime[100001];
int prime[100001];
int Swap(int i){
    string temp="";
    while(i){
        temp+=i%10+'0';
        i/=10;
    }
    int ans=0;
    for (int j=0;j<temp.size();j++){
        ans*=10;
        ans+=temp[j]-'0';
    }
    return ans;
}
vector<int> f(int m,int n){
    vector<int>ans;
    for (int i=m;i<=n;i++){
        if(statePrime[i]==0&&statePrime[Swap(i)]==0){
            ans.push_back(i);
        }
    }
    return ans;
}
void PrimeNumber(){
    int cnt=0;
    for (int i=2;i<100002;i++){
        if(!statePrime[i]){
            prime[cnt++]=i;
        }
        for (int j=0;j<cnt;j++){
            if(i*prime[j]>100001) break;
            statePrime[i*prime[j]]=1;
            if(i%prime[j]==0) break;
        }
    }
}
int main(){
    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    int M,N;
    cin>>M>>N;
    PrimeNumber();
    vector<int>ans;
    ans=f(M,N);
    for (int i=0;i<ans.size();i++){
        if(i==ans.size()-1){
            cout<<ans[i];
        }else{
            cout<<ans[i]<<",";
        }
    }
    if(ans.size()==0){
        cout<<"No";
    }
    return 0;
}
