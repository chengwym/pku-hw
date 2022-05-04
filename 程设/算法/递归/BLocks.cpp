#include <iostream>
#include <cstring>
using namespace std;
int color[210];
int len[210];
int Click[210][210][210];

int click_box(int i,int j,int ex_len){
    if(Click[i][j][ex_len]!=-1) return Click[i][j][ex_len];
    if(i==j) {
        Click[i][j][ex_len]=(ex_len+len[i])*(ex_len+len[i]);
        return Click[i][j][ex_len];
    }
    int ans=0;
    for (int k=i;k<j-1;k++){
        if(color[k]==color[j]){
            if(Click[i][k][ex_len+len[j]]==-1) Click[i][k][ex_len+len[j]]=click_box(i,k,ex_len+len[j]);
            if(Click[k+1][j-1][0]==-1) Click[k+1][j-1][0]=click_box(k+1,j-1,0);
            ans=max(ans,Click[i][k][ex_len+len[j]]+Click[k+1][j-1][0]);
        }
    }
    if(Click[i][j-1][0]==-1)  Click[i][j-1][0]=click_box(i,j-1,0);
    ans=max(ans,Click[i][j-1][0]+(len[j]+ex_len)*(len[j]+ex_len));
    Click[i][j][ex_len]=ans;
    return ans;
}

int main(){

    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);

    int t;
    cin>>t;
    for (int j=1;j<=t;j++){
        memset(color,0,sizeof(color));
        memset(len,0,sizeof(len));
        memset(Click,-1,sizeof(Click));
        int N;
        cin>>N;
        int n=1;
        for (int i=0;i<N;i++){
            if(i==0){
                cin>>color[1];
                len[1]=1;
            }else{
                int temp;
                cin>>temp;
                if(temp==color[n]){
                    len[n]++;
                }else{
                    n++;
                    color[n]=temp;
                    len[n]=1;
                }
            }
        }
        cout<<"Case "<<j<<": "click_box(1,n,0)<<endl;
    }



    return 0;
}
