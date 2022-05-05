#include <iostream>
#include <vector>
using namespace std;
const int N=10005;
int dp[N];
int a[205];
int dpn[N];
int main(){
    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    int n,x;
    cin>>n>>x;
    for (int i=0;i<n;i++){
        cin>>a[i];
    }
    dp[0]=1;
    dpn[0]=1;
    for (int i=0;i<n;i++){
        for (int j=x;j>=a[i];j--){
            dp[j]=dp[j]+dp[j-a[i]];
        }
    }
    vector<int>ans;
    for(int i=0;i<n;i++){
        for (int j=0;j<=x;j++){
            if(j>=a[i]) dpn[j]=dp[j]-dpn[j-a[i]];
            else dpn[j]=dp[j];
        }
        if(dpn[x]==0) ans.push_back(a[i]);
    }
    if(ans.size()==0) cout<<0;
    else {
        cout<<ans.size()<<endl;
        for (int i=0;i<ans.size();i++){
            cout<<ans[i]<<" ";
        }
    }
    return 0;
}
