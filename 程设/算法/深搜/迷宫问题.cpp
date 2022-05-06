#include <iostream>
#include <vector>
using namespace std;
int maze[5][5];
int state[5][5];
int direction[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
int minstep=0x7fffffff;
vector<pair<int,int>>ans;
void dfs(int x,int y,vector<pair<int,int> >temp,int step){
    if(x==4&&y==4&&minstep>step){
        ans=temp;
        minstep=step;
        return;
    }
    for (int i=0;i<4;i++){
        int newx=x+direction[i][0];
        int newy=y+direction[i][1];
        if(newy<0||newx<0||newx>=5||newy>=5||state[newx][newy]==1||maze[newx][newy]==1) continue;
        state[newx][newy]=1;
        temp.push_back(pair<int,int>(newx,newy));
        dfs(newx,newy,temp,step+1);
        temp.pop_back();
        state[newx][newy]=0;
    }
}
int main(){
    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);
    for (int i=0;i<5;i++){
        for (int j=0;j<5;j++){
            cin>>maze[i][j];
        }
    }
    vector<pair<int,int>>temp;
    dfs(0,0,temp,0);
    cout<<"(0,0)"<<endl;
    for (int i=0;i<ans.size();i++){
        if(i!=ans.size()-1){
            cout<<"("<<ans[i].first<<", "<<ans[i].second<<")"<<endl;
        }
        else {
            cout<<"("<<ans[i].first<<", "<<ans[i].second<<")"
        }
    }


    return 0;
}
