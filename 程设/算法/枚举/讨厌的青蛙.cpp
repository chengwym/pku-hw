#include <iostream>
#include <algorithm>
using namespace std;
int r,c,N;
struct Point{
    int x,y;
}p[5001];
bool operator<(const Point & a,const Point & b){
    return a.x==b.x&&a.y<b.y||a.x<b.x;
}
int searchStep(Point secpoint,int dX,int dY){
    Point point;
    point.x=secpoint.x+dX;
    point.y=secpoint.y+dY;
    int step=2;
    while(point.x>=1&&point.x<=r&&point.y>=1&&point.y<=c){
        if(!binary_search(p,p+N,point)){
            step=0;
            break;
        }
        point.x+=dX;
        point.y+=dY;
        step++;
    }
    return step;
}
int main(){

    // freopen("1.in","r",stdin);
    // freopen("1.out","w",stdout);

    scanf("%d%d%d",&r,&c,&N);
    for (int i=0;i<N;i++){
        scanf("%d%d",&p[i].x,&p[i].y);
    }

    sort(p,p+N);

    int max=2,step=0;
    for (int i=0;i<N-2;i++){
        for (int j=i+1;j<N-1;j++){
            int dX=p[j].x-p[i].x;
            int dY=p[j].y-p[i].y;
            int pX=p[i].x-dX;
            int pY=p[i].y-dY;
            if(pX>=1&&pX<=r&&pY>=1&&pY<=c) continue;
            if(p[i].x+(max-1)*dX>r) break;
            pY=p[i].y+(max-1)*dY;
            if(pY>c||pY<1) continue;
            step=searchStep(p[j],dX,dY);
            if(step>max) max=step;
        }
    }
    if(max==2) max=0;
    printf("%d",max);
    



    return 0;
}
