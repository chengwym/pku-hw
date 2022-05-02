#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int M,N,R,K,T;
int time_;
int HeroHp[5]={};
int HeroForce[5]={};
int gameWin=0;

string setTime(int t_){
    string tempTime="000";
    for (int i=0;i<3;i++){
        tempTime[2-i]=t_%10+'0';
        t_/=10;
    }
    return tempTime;
}

class Hero;

class Weapon{
public:
    int id;
    int force;
    string name;
    
    Weapon(){};

    Weapon(int id_,string name_);

    static Weapon* addWeapon(int num,Hero* hero);

    virtual void nt(){};

};

class sword:public Weapon{
public:
    sword();

    sword(int ownerForce);

    void nt(){};

};

class bomb:public Weapon{
public:
    
    bomb();

    void nt(){};

};

class arrow:public Weapon{
public:
    
    int used;
    arrow();

    void nt(){};

};

class HeadQuarter;

class Hero{
public:
    
    int hp;
    int id;
    int force;
    bool survive;
    Weapon* weapon[3];
    int distance;
    string name;
    HeadQuarter * camp;
    string campName;
    int numberInCamp;
    
    Hero(){};

    Hero(string name_,int distance_,int id_,int force_,HeadQuarter* camp_,string campName_,int numberInCamp_,int hp_);

    virtual void nt(){};

    static Hero* addHero(int num,HeadQuarter * camp);
};

class dragon:public Hero{
public:
    
    double morale;

    dragon(HeadQuarter * camp_);

    void nt(){};

};

class ninja:public Hero{
public:
    
    ninja(HeadQuarter * camp_);


    void nt(){};

};

class iceman:public Hero{
public:
    
    int stepSize;
    
    iceman(HeadQuarter * camp_);

    void nt(){};
    
};  

class lion:public Hero{
public:
    
    int loyalty;
    
    lion(HeadQuarter * camp_);
    
    void decreaseLoyalty(){
        loyalty-=K;
    }
    
    void swapHp(Hero * enemy,int temp){
        enemy->hp+=temp;
    }

    void nt(){};
    
};

class wolf:public Hero{
public:

    wolf(HeadQuarter * camp_);

    void fightWin(Hero * enemy){
        for (int i=0;i<3;i++){
            if(enemy->weapon[i]!=NULL&&weapon[i]==NULL){
                weapon[i]=enemy->weapon[i];
                enemy->weapon[i]=NULL;
            }
        }
    }

    void nt(){};
    
};

class FightArea;

class HeadQuarter{
public:
    
    int id;//red 0 blue1
    int seq[5];
    int enemyNumber;
    int nowseq;
    int armyNumber;
    int lifeElement;
    string campName;
    
    HeadQuarter(int id_,int seq_[5],string campName_,int lifeElement_);

    void born();

};

class FightArea{
public:
    bool ifYell;
    int id;
    int flag;//flag = 1 red , flag = -1 blue
    int raiseFlag;//1 red -1 blue
    int element;
    Hero* heroInHere[2];
    Hero* heroComeHere[2];
    int fightresult;
    int fightNowResult;
    FightArea(int id_);
    void rsize();
};
FightArea* fightArea[22]={new FightArea(0),new FightArea(1),new FightArea(2),new FightArea(3),new FightArea(4),
                        new FightArea(5),new FightArea(6),new FightArea(7),new FightArea(8),new FightArea(9),new FightArea(10),new FightArea(11),new FightArea(12),new FightArea(13),new FightArea(14),
                        new FightArea(15),new FightArea(16),new FightArea(17),new FightArea(18),new FightArea(19),new FightArea(20),new FightArea(21)};

Weapon::Weapon(int id_,string name_):id(id_),name(name_){ }

Weapon* Weapon::addWeapon(int num,Hero* hero){
    if(num==0){
        if(hero->force*2/10==0){
            return NULL;
        }
        return new sword(hero->force);
    }else if(num==1){
        return new bomb();
    }else{
        return new arrow();
    }
    return new Weapon();
}

sword::sword():Weapon(0,"sword"){}

sword::sword(int ownerForce):Weapon(0,"sword"){
    force=ownerForce*2/10;
}

bomb::bomb():Weapon(1,"bomb"){
    force=0;
}

arrow::arrow():Weapon(2,"arrow"){
    used=3;
    force=R;
}

Hero::Hero(string name_,int distance_,int id_,int force_,HeadQuarter* camp_,string campName_,int numberInCamp_,int hp_){
    for (int i=0;i<3;i++){
        weapon[i]=NULL;
    }
    name=name_;
    distance=distance_;
    id=id_;
    force=force_;
    survive=true;
    camp=camp_;
    campName=campName_;
    numberInCamp=numberInCamp_;
    hp=hp_;
}

Hero* Hero::addHero(int num,HeadQuarter * camp){
    if(num==0){
        camp->lifeElement-=HeroHp[0];
        return new dragon(camp);
    }else if(num==1){
        camp->lifeElement-=HeroHp[1];
        return new ninja(camp); 
    }else if(num==2){
        camp->lifeElement-=HeroHp[2];
        return new iceman(camp);
    }else if(num==3){
        camp->lifeElement-=HeroHp[3];
        return new lion(camp);
    }else if(num==4){
        camp->lifeElement-=HeroHp[4];
        return new wolf(camp);
    }
    return new Hero();
}

dragon::dragon(HeadQuarter * camp_):Hero("dragon",0,0,HeroForce[0],camp_,camp_->campName,camp_->armyNumber,HeroHp[0]){
    weapon[numberInCamp%3]=Weapon::addWeapon(numberInCamp%3,this);
    morale=double(camp_->lifeElement)/(HeroHp[0]);
}

ninja::ninja(HeadQuarter * camp_):Hero("ninja",0,1,HeroForce[1],camp_,camp_->campName,camp_->armyNumber,HeroHp[1]){
    weapon[numberInCamp%3]=Weapon::addWeapon(numberInCamp%3,this);
    weapon[(numberInCamp+1)%3]=Weapon::addWeapon((numberInCamp+1)%3,this);
}

iceman::iceman(HeadQuarter * camp_):Hero("iceman",0,2,HeroForce[2],camp_,camp_->campName,camp_->armyNumber,HeroHp[2]){
    weapon[numberInCamp%3]=Weapon::addWeapon(numberInCamp%3,this);
    stepSize=0;
}

lion::lion(HeadQuarter * camp_):Hero("lion",0,3,HeroForce[3],camp_,camp_->campName,camp_->armyNumber,HeroHp[3]){
    loyalty=camp->lifeElement;
}

wolf::wolf(HeadQuarter * camp_):Hero("wolf",0,4,HeroForce[4],camp_,camp_->campName,camp_->armyNumber,HeroHp[4]){}

FightArea::FightArea(int id_){
    id=id_;
    heroInHere[0]=heroInHere[1]=NULL;
    heroComeHere[0]=heroComeHere[1]=NULL;
    fightresult=0;//red 1 blue -1
    raiseFlag=0;
    element=0;
    flag=0;
    fightNowResult=0;
    ifYell=false;
}

void FightArea::rsize(){
    heroInHere[0]=heroInHere[1]=NULL;
    heroComeHere[0]=heroComeHere[1]=NULL;
    fightresult=0;//red 1 blue -1
    raiseFlag=0;
    fightNowResult=0;
    element=0;
    flag=0;
    ifYell=false;
}

HeadQuarter::HeadQuarter(int id_,int seq_[5],string campName_,int lifeElement_){
    id=id_;
    for (int i=0;i<5;i++){
        seq[i]=seq_[i];
    }
    enemyNumber=0;
    nowseq=0;
    armyNumber=0;
    campName=campName_;
    lifeElement=lifeElement_;
}

void HeadQuarter::born(){
    if(lifeElement>=HeroHp[seq[nowseq]]){
        armyNumber++;
        Hero* it=Hero::addHero(seq[nowseq],this);
        nowseq++;
        nowseq%=5;
        cout<<setTime(time_)<<":00 "<<campName<<" "<<it->name<<" "<<armyNumber<<" born"<<endl;
        if(it->id==0){
            dragon* itDragon=dynamic_cast<dragon*>(it);
            cout<<"Its morale is ";
            printf("%.2f\n",itDragon->morale);
        }else if(it->id==3){
            lion* itLion=dynamic_cast<lion*>(it);
            cout<<"Its loyalty is "<<itLion->loyalty<<endl;
        }
        if(id==0){
            fightArea[0]->heroInHere[0]=it;
        }else if(id==1){
            fightArea[N+1]->heroInHere[1]=it;
        }
    }
}

void lionScape(){
    for (int i=0;i<=N+1;i++){
        if(i!=N+1){
            if(fightArea[i]->heroInHere[0]!=NULL&&fightArea[i]->heroInHere[0]->id==3){
                lion* it=dynamic_cast<lion*>(fightArea[i]->heroInHere[0]);
                if(it->loyalty<=0){
                    cout << setTime(time_) <<":05 "<<it->campName<<" "<<it->name<<" "<<it->numberInCamp<<" ran away"<<endl;
                    fightArea[i]->heroInHere[0]=NULL;
                }
            }
        }
        if(i!=0){
            if(fightArea[i]->heroInHere[1]!=NULL&&fightArea[i]->heroInHere[1]->id==3){
                lion* it=dynamic_cast<lion*>(fightArea[i]->heroInHere[1]);
                if(it->loyalty<=0){
                    cout << setTime(time_) <<":05 "<<it->campName<<" "<<it->name<<" "<<it->numberInCamp<<" ran away"<<endl;
                    fightArea[i]->heroInHere[1]=NULL;
                }
            }
        }
    }
}

void moveOneStep(){
    for (int i=0;i<=N+1;i++){
        if(i!=N+1){
            if(fightArea[i]->heroInHere[0]!=NULL){
                fightArea[i+1]->heroComeHere[0]=fightArea[i]->heroInHere[0];
                fightArea[i]->heroInHere[0]=NULL;
                if((fightArea[i+1]->heroComeHere[0]->id)==2){
                    iceman* itIceman=dynamic_cast<iceman*>(fightArea[i+1]->heroComeHere[0]);
                    if(itIceman->stepSize==1){
                        itIceman->force+=20;
                        itIceman->hp=max(1,itIceman->hp-9);
                        itIceman->stepSize=0;
                    }else{
                        itIceman->stepSize++;
                    }
                }
            }
        }
        if(i!=0){
            if(fightArea[i]->heroInHere[1]!=NULL){
                fightArea[i-1]->heroComeHere[1]=fightArea[i]->heroInHere[1];
                fightArea[i]->heroInHere[1]=NULL;
                if(fightArea[i-1]->heroComeHere[1]->id==2){
                    iceman* itIceman=dynamic_cast<iceman*>(fightArea[i-1]->heroComeHere[1]);
                    if(itIceman->stepSize==1){
                        itIceman->force+=20;
                        itIceman->hp=max(1,itIceman->hp-9);
                        itIceman->stepSize=0;
                    }else{
                        itIceman->stepSize++;
                    }
                }
            }
        }
    }
}

void PrintMoveOneStep(){
    if(fightArea[0]->heroComeHere[1]!=NULL){
        auto it=fightArea[0]->heroComeHere[1];
        cout<<setTime(time_)<<":10 blue "<<it->name<<" "<<it->numberInCamp<<" reached red headquarter with "<<it->hp<<" elements and force "<<it->force<<endl;
        if(fightArea[0]->heroInHere[1]==NULL){
            fightArea[0]->heroInHere[1]=fightArea[0]->heroComeHere[1];
            fightArea[0]->heroComeHere[1]=NULL;
        }else if(fightArea[0]->heroInHere[1]!=NULL){
            gameWin=-1;
            cout<<setTime(time_)<<":10 red headquarter was taken"<<endl;
        }
    }
    for (int i=1;i<=N;i++){
        if(fightArea[i]->heroComeHere[0]!=NULL){
            auto it=fightArea[i]->heroComeHere[0];
            cout<<setTime(time_)<<":10 red "<<it->name<<" "<<it->numberInCamp<<" marched to city "<<i<<" with "<<it->hp<<" elements and force "<<it->force<<endl;
            fightArea[i]->heroInHere[0]=fightArea[i]->heroComeHere[0];
            fightArea[i]->heroComeHere[0]=NULL;
        }
        if(fightArea[i]->heroComeHere[1]!=NULL){
            auto it=fightArea[i]->heroComeHere[1];
            cout<<setTime(time_)<<":10 blue "<<it->name<<" "<<it->numberInCamp<<" marched to city "<<i<<" with "<<it->hp<<" elements and force "<<it->force<<endl;
            fightArea[i]->heroInHere[1]=fightArea[i]->heroComeHere[1];
            fightArea[i]->heroComeHere[1]=NULL;
        }
    }
    if(fightArea[N+1]->heroComeHere[0]!=NULL){
        auto it=fightArea[N+1]->heroComeHere[0];
        cout<<setTime(time_)<<":10 red "<<it->name<<" "<<it->numberInCamp<<" reached blue headquarter with "<<it->hp<<" elements and force "<<it->force<<endl;
        if(fightArea[N+1]->heroInHere[0]==NULL){
            fightArea[N+1]->heroInHere[0]=fightArea[N+1]->heroComeHere[0];
            fightArea[N+1]->heroComeHere[0]=NULL;
        }else if(fightArea[N+1]->heroInHere[0]!=NULL){
            gameWin=1;
            cout<<setTime(time_)<<":10 blue headquarter was taken"<<endl;
        }
    }
}

void produceElement(){
    for (int i=1;i<=N;i++){
        fightArea[i]->element+=10;
        if(fightArea[i]->heroInHere[0]!=NULL&&fightArea[i]->heroInHere[1]==NULL){
            (fightArea[i]->heroInHere[0]->camp->lifeElement)+=(fightArea[i]->element);
            cout<<setTime(time_)<<":30 red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" earned "<<fightArea[i]->element<<" elements for his headquarter"<<endl;
            fightArea[i]->element=0;
        }
        if(fightArea[i]->heroInHere[1]!=NULL&&fightArea[i]->heroInHere[0]==NULL){
            (fightArea[i]->heroInHere[1]->camp->lifeElement)+=(fightArea[i]->element);
            cout<<setTime(time_)<<":30 blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" earned "<<fightArea[i]->element<<" elements for his headquarter"<<endl;
            fightArea[i]->element=0;
        }
    }
}

void shootarrows(){
    for (int i=0;i<=N+1;i++){
        if(i!=N+1&&i!=N&&fightArea[i]->heroInHere[0]!=NULL&&fightArea[i]->heroInHere[0]->weapon[2]!=NULL&&fightArea[i+1]->heroInHere[1]!=NULL){
            Hero* it=fightArea[i]->heroInHere[0];
            arrow* itsWeapon=dynamic_cast<arrow*>(it->weapon[2]);
            itsWeapon->used--;
            cout<<setTime(time_)<<":35 red "<<it->name<<" "<<it->numberInCamp<<" shot";
            Hero* enemyit=fightArea[i+1]->heroInHere[1];
            enemyit->hp-=R;
            if(enemyit->hp>0){
                cout<<endl;
            }
            if(enemyit->hp<=0){
                cout<<" and killed blue "<<fightArea[i+1]->heroInHere[1]->name<<" "<<fightArea[i+1]->heroInHere[1]->numberInCamp<<endl;
                if(fightArea[i+1]->heroInHere[0]!=NULL){
                    
                    if(fightArea[i+1]->heroInHere[0]->id==0){
                        dragon* itDragon=dynamic_cast<dragon*>(fightArea[i+1]->heroInHere[0]);
                        itDragon->morale+=0.2;
                        if((fightArea[i+1]->flag==0&&fightArea[i+1]->id%2==1||fightArea[i+1]->flag==1)&&itDragon->morale>0.8){
                            fightArea[i+1]->ifYell=true;
                        }
                    }
                    fightArea[i+1]->fightNowResult=1;
                }
            }
            if(itsWeapon->used==0){
                it->weapon[2]==NULL;
                itsWeapon==NULL;
                fightArea[i]->heroInHere[0]->weapon[2]=NULL;
            }
        }
        if(i!=0&&i!=1&&fightArea[i]->heroInHere[1]!=NULL&&fightArea[i]->heroInHere[1]->weapon[2]!=NULL&&fightArea[i-1]->heroInHere[0]!=NULL){
            Hero* it=fightArea[i]->heroInHere[1];
            arrow* itsWeapon=dynamic_cast<arrow*>(it->weapon[2]);
            itsWeapon->used--;
            cout<<setTime(time_)<<":35 blue "<<it->name<<" "<<it->numberInCamp<<" shot";
            Hero* enemyit=fightArea[i-1]->heroInHere[0];
            enemyit->hp-=R;
            if(enemyit->hp>0){
                cout<<endl;
            }
            if(enemyit->hp<=0){
                cout<<" and killed red "<<fightArea[i-1]->heroInHere[0]->name<<" "<<fightArea[i-1]->heroInHere[0]->numberInCamp<<endl;
                if(fightArea[i-1]->heroInHere[1]!=NULL){
                    
                    if(fightArea[i-1]->heroInHere[1]->id==0){
                        if(fightArea[i-1]->flag==0&&fightArea[i-1]->id%2==0||fightArea[i-1]->flag==-1){
                            fightArea[i-1]->ifYell=true;
                        }
                    }
                    fightArea[i-1]->fightNowResult=-1;
                }
            }
            if(itsWeapon->used==0){
                it->weapon[2]==NULL;
                itsWeapon==NULL;
                fightArea[i]->heroInHere[1]->weapon[2]=NULL;
            }
        }
        
    }

    for (int i=0;i<=N+1;i++){
        if(i!=N+1&&i!=N&&fightArea[i+1]->heroInHere[1]!=NULL&&fightArea[i+1]->heroInHere[1]->hp<=0){
            if(fightArea[i+1]->heroInHere[0]!=NULL&&fightArea[i+1]->heroInHere[0]->hp>0&&fightArea[i+1]->heroInHere[0]->id==4){
                wolf* itWolf=dynamic_cast<wolf*>(fightArea[i+1]->heroInHere[0]);
                itWolf->fightWin(fightArea[i+1]->heroInHere[1]);      
            }
            fightArea[i+1]->heroInHere[1]=NULL;
        }
        if(i!=0&&i!=1&&fightArea[i-1]->heroInHere[0]!=NULL&&fightArea[i-1]->heroInHere[0]->hp<=0){
            if(fightArea[i-1]->heroInHere[1]!=NULL&&fightArea[i-1]->heroInHere[1]->hp>0&&fightArea[i-1]->heroInHere[1]->id==4){
                wolf* itWolf=dynamic_cast<wolf*>(fightArea[i-1]->heroInHere[1]);
                itWolf->fightWin(fightArea[i-1]->heroInHere[0]);      
            }
            fightArea[i-1]->heroInHere[0]=NULL;
        }
    }

    for (int i=0;i<=N+1;i++){
        if(i!=N+1&&i!=N&&fightArea[i+1]->fightNowResult==1){
            if(fightArea[i+1]->heroInHere[0]!=NULL){
                if(fightArea[i+1]->fightresult==1){
                    fightArea[i+1]->raiseFlag=1;
                }else{
                    fightArea[i+1]->fightresult=1;
                }
            }else{
                fightArea[i+1]->fightNowResult=0;
            }
        }
        if(i!=0&&i!=1&&fightArea[i-1]->fightNowResult==-1){
            if(fightArea[i-1]->heroInHere[1]!=NULL){
                if(fightArea[i-1]->fightresult==-1){
                    fightArea[i-1]->raiseFlag=-1;
                }else{
                    fightArea[i-1]->fightresult=-1;
                }
            }else{
                fightArea[i-1]->fightNowResult=0;
            }
        }
    }

}

int fight(Hero a,Hero b){//1 a win,0 dogfall,-1 a defeat
    if(a.weapon[0]!=NULL){ //does a have sword
        b.hp-=(a.weapon[0]->force+a.force);
    }else{
        b.hp-=(a.force);
    }
    if(b.hp<=0){ //does b die
        return 1;
    }else if(b.id!=1){ //b is survive
        if(b.weapon[0]!=NULL){
            a.hp-=(b.weapon[0]->force+b.force/2);
        }else{
            a.hp-=(b.force/2);
        }
        if(a.hp<=0){
            return -1;
        }else{
            return 0;
        }
    }
    return 0;
}

int judgeIfDie(FightArea site){
    if(site.flag==0&&site.id%2==1||site.flag==1){
        return fight(*site.heroInHere[0],*site.heroInHere[1]);
    }
    if(site.flag==0&&site.id%2==0||site.flag==-1){
        return -1*fight(*site.heroInHere[1],*site.heroInHere[0]);
    }
    return 0;//-1 red die , 1 blue die 
}

void useBOMB(){
    for (int i=0;i<=N+1;i++){
        if(fightArea[i]->heroInHere[0]!=NULL&&fightArea[i]->heroInHere[1]!=NULL&&fightArea[i]->heroInHere[0]->weapon[1]!=NULL&&i!=N+1){
            //red fight
            if(judgeIfDie(*fightArea[i])==-1){
                cout<<setTime(time_)<<":38 red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" used a bomb and killed blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<endl;
                fightArea[i]->heroInHere[0]=NULL;
                fightArea[i]->heroInHere[1]=NULL;
            }
        }
        if(fightArea[i]->heroInHere[0]!=NULL&&fightArea[i]->heroInHere[1]!=NULL&&fightArea[i]->heroInHere[1]->weapon[1]!=NULL&&i!=0){
            if(judgeIfDie(*fightArea[i])==1){
                cout<<setTime(time_)<<":38 blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" used a bomb and killed red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<endl;
                fightArea[i]->heroInHere[0]=NULL;
                fightArea[i]->heroInHere[1]=NULL;
            }
        }
    }
}

void battle(HeadQuarter* red,HeadQuarter* blue){

    int redElement=0,blueElement=0;

    for (int i=1;i<=N;i++){
        
        if(fightArea[i]->heroInHere[0]!=NULL&&fightArea[i]->heroInHere[1]!=NULL){
            
            int redLionHp,blueLionHp;
            if(fightArea[i]->heroInHere[0]->id==3){
                redLionHp=fightArea[i]->heroInHere[0]->hp;
            }
            if(fightArea[i]->heroInHere[1]->id==3){
                blueLionHp=fightArea[i]->heroInHere[1]->hp;
            }
            
            if(fightArea[i]->flag==0&&(fightArea[i]->id)%2==1||fightArea[i]->flag==1){//red fight first
                if(fightArea[i]->heroInHere[0]->weapon[0]!=NULL){
                    fightArea[i]->heroInHere[1]->hp-=(fightArea[i]->heroInHere[0]->weapon[0]->force+fightArea[i]->heroInHere[0]->force);
                    fightArea[i]->heroInHere[0]->weapon[0]->force=fightArea[i]->heroInHere[0]->weapon[0]->force*8/10;
                    if(fightArea[i]->heroInHere[0]->weapon[0]->force==0){
                        fightArea[i]->heroInHere[0]->weapon[0]=NULL;
                    }
                }else{
                    fightArea[i]->heroInHere[1]->hp-=(fightArea[i]->heroInHere[0]->force);
                }
                cout<<setTime(time_)<<":40 red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" attacked blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" in city "<<i<<" with "<<fightArea[i]->heroInHere[0]->hp<<" elements and force "<<fightArea[i]->heroInHere[0]->force<<endl;
                if(fightArea[i]->heroInHere[1]->hp<=0){
                    cout<<setTime(time_)<<":40 blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" was killed in city "<<i<<endl;
                    if(fightArea[i]->heroInHere[0]->id==4){
                        wolf* itWolf=dynamic_cast<wolf*>(fightArea[i]->heroInHere[0]);
                        itWolf->fightWin(fightArea[i]->heroInHere[1]);
                    }
                    if(fightArea[i]->heroInHere[1]->id==3){
                        lion* itLion=dynamic_cast<lion*>(fightArea[i]->heroInHere[1]);
                        itLion->swapHp(fightArea[i]->heroInHere[0],blueLionHp);
                    }
                    fightArea[i]->heroInHere[1]=NULL;
                }else if(fightArea[i]->heroInHere[1]->id!=1){
                    if(fightArea[i]->heroInHere[1]->weapon[0]!=NULL){
                        fightArea[i]->heroInHere[0]->hp-=(fightArea[i]->heroInHere[1]->force/2+fightArea[i]->heroInHere[1]->weapon[0]->force);
                        fightArea[i]->heroInHere[1]->weapon[0]->force=fightArea[i]->heroInHere[1]->weapon[0]->force*8/10;
                        if(fightArea[i]->heroInHere[1]->weapon[0]->force==0){
                            fightArea[i]->heroInHere[1]->weapon[0]=NULL;
                        }
                    }else {
                        fightArea[i]->heroInHere[0]->hp-=(fightArea[i]->heroInHere[1]->force/2);
                    }
                    cout<<setTime(time_)<<":40 blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" fought back against red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" in city "<<i<<endl;
                    if(fightArea[i]->heroInHere[0]->hp<=0){
                        cout<<setTime(time_)<<":40 red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" was killed in city "<<i<<endl;
                        if(fightArea[i]->heroInHere[1]->id==4){
                            wolf* itWolf=dynamic_cast<wolf*>(fightArea[i]->heroInHere[1]);
                            itWolf->fightWin(fightArea[i]->heroInHere[0]);
                        }
                        if(fightArea[i]->heroInHere[0]->id==3){
                            lion* itLion=dynamic_cast<lion*>(fightArea[i]->heroInHere[0]);
                            itLion->swapHp(fightArea[i]->heroInHere[1],redLionHp);
                        }
                        fightArea[i]->heroInHere[0]=NULL;
                    }
                }

                if(fightArea[i]->heroInHere[0]!=NULL&&fightArea[i]->heroInHere[0]->id==0){
                    dragon* it=dynamic_cast<dragon*>(fightArea[i]->heroInHere[0]);
                    if(fightArea[i]->heroInHere[1]==NULL){
                        it->morale+=0.2;
                    }else if(fightArea[i]->heroInHere[1]!=NULL){
                        it->morale-=0.2;
                    }
                    if(it->morale>0.8){
                        cout<<setTime(time_)<<":40 red dragon "<<it->numberInCamp<<" yelled in city "<<i<<endl;
                    }
                }
            }
            
            
            if(fightArea[i]->flag==0&&(fightArea[i]->id)%2==0||fightArea[i]->flag==-1){//blue fight first
                if(fightArea[i]->heroInHere[1]->weapon[0]!=NULL){
                    fightArea[i]->heroInHere[0]->hp-=(fightArea[i]->heroInHere[1]->weapon[0]->force+fightArea[i]->heroInHere[1]->force);
                    fightArea[i]->heroInHere[1]->weapon[0]->force=fightArea[i]->heroInHere[1]->weapon[0]->force*8/10;
                    if(fightArea[i]->heroInHere[1]->weapon[0]->force==0){
                        fightArea[i]->heroInHere[1]->weapon[0]=NULL;
                    }
                }else{
                    fightArea[i]->heroInHere[0]->hp-=(fightArea[i]->heroInHere[1]->force);
                }
                cout<<setTime(time_)<<":40 blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" attacked red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" in city "<<i<<" with "<<fightArea[i]->heroInHere[1]->hp<<" elements and force "<<fightArea[i]->heroInHere[1]->force<<endl;
                if(fightArea[i]->heroInHere[0]->hp<=0){
                    cout<<setTime(time_)<<":40 red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" was killed in city "<<i<<endl;
                    if(fightArea[i]->heroInHere[1]->id==4){
                        wolf* itWolf=dynamic_cast<wolf*>(fightArea[i]->heroInHere[1]);
                        itWolf->fightWin(fightArea[i]->heroInHere[0]);
                    }
                    if(fightArea[i]->heroInHere[0]->id==3){
                        lion* itLion=dynamic_cast<lion*>(fightArea[i]->heroInHere[0]);
                        itLion->swapHp(fightArea[i]->heroInHere[1],redLionHp);
                    }
                    fightArea[i]->heroInHere[0]=NULL;
                }else if(fightArea[i]->heroInHere[0]->id!=1){
                    if(fightArea[i]->heroInHere[0]->weapon[0]!=NULL){
                        fightArea[i]->heroInHere[1]->hp-=(fightArea[i]->heroInHere[0]->force/2+fightArea[i]->heroInHere[0]->weapon[0]->force);
                        fightArea[i]->heroInHere[0]->weapon[0]->force=fightArea[i]->heroInHere[0]->weapon[0]->force*8/10;
                        if(fightArea[i]->heroInHere[0]->weapon[0]->force==0){
                            fightArea[i]->heroInHere[0]->weapon[0]=NULL;
                        }
                    }else {
                        fightArea[i]->heroInHere[1]->hp-=(fightArea[i]->heroInHere[0]->force/2);
                    }
                    cout<<setTime(time_)<<":40 red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" fought back against blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" in city "<<i<<endl;
                    if(fightArea[i]->heroInHere[1]->hp<=0){
                        cout<<setTime(time_)<<":40 blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" was killed in city "<<i<<endl;
                        if(fightArea[i]->heroInHere[0]->id==4){
                            wolf* itWolf=dynamic_cast<wolf*>(fightArea[i]->heroInHere[0]);
                            itWolf->fightWin(fightArea[i]->heroInHere[1]);
                        }
                        if(fightArea[i]->heroInHere[1]->id==3){
                            lion* itLion=dynamic_cast<lion*>(fightArea[i]->heroInHere[1]);
                            itLion->swapHp(fightArea[i]->heroInHere[0],blueLionHp);
                        }
                        fightArea[i]->heroInHere[1]=NULL;
                    }
                }

                if(fightArea[i]->heroInHere[1]!=NULL&&fightArea[i]->heroInHere[1]->id==0){
                    dragon* it=dynamic_cast<dragon*>(fightArea[i]->heroInHere[1]);
                    if(fightArea[i]->heroInHere[0]==NULL){
                        it->morale+=0.2;
                    }else if(fightArea[i]->heroInHere[0]!=NULL){
                        it->morale-=0.2;
                    }
                    if(it->morale>0.8){
                        cout<<setTime(time_)<<":40 blue dragon "<<it->numberInCamp<<" yelled in city "<<i<<endl;
                    }
                }
            }

            if(fightArea[i]->heroInHere[0]!=NULL&&fightArea[i]->heroInHere[1]!=NULL){
                if(fightArea[i]->heroInHere[0]->id==3){
                    lion* itLion=dynamic_cast<lion*>(fightArea[i]->heroInHere[0]);
                    itLion->decreaseLoyalty();
                }
                if(fightArea[i]->heroInHere[1]->id==3){
                    lion* itLion=dynamic_cast<lion*>(fightArea[i]->heroInHere[1]);
                    itLion->decreaseLoyalty();
                }
                fightArea[i]->fightresult=0;
            }
            else if(fightArea[i]->heroInHere[1]==NULL&&fightArea[i]->heroInHere[0]!=NULL){//red win
                if(fightArea[i]->fightresult==1){
                    fightArea[i]->raiseFlag=1;
                }else{
                    fightArea[i]->fightresult=1;
                }
                fightArea[i]->fightNowResult=1;
                redElement+=fightArea[i]->element;         
                cout<<setTime(time_)<<":40 red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" earned "<<fightArea[i]->element<<" elements for his headquarter"<<endl;
                fightArea[i]->element=0;
            }else if(fightArea[i]->heroInHere[0]==NULL&&fightArea[i]->heroInHere[1]!=NULL){//blue win
                if(fightArea[i]->fightresult==-1){
                    fightArea[i]->raiseFlag=-1;
                }else{
                    fightArea[i]->fightresult=-1;
                }
                fightArea[i]->fightNowResult=-1;
                blueElement+=fightArea[i]->element;         
                cout<<setTime(time_)<<":40 blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" earned "<<fightArea[i]->element<<" elements for his headquarter"<<endl;
                fightArea[i]->element=0;
            }

            if(fightArea[i]->raiseFlag==1&&fightArea[i]->flag!=1){
                cout<<setTime(time_)<<":40 red flag raised in city "<<i<<endl;
                fightArea[i]->raiseFlag=0;
                fightArea[i]->flag=1;
            }
            if(fightArea[i]->raiseFlag==-1&&fightArea[i]->flag!=-1){
                cout<<setTime(time_)<<":40 blue flag raised in city "<<i<<endl;
                fightArea[i]->raiseFlag=0;
                fightArea[i]->flag=-1;
            }

        }else if(fightArea[i]->heroInHere[0]!=NULL&&fightArea[i]->heroInHere[1]==NULL){
            if(fightArea[i]->ifYell==true){
                cout<<setTime(time_)<<":40 red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" yelled in city "<<i<<endl;
                fightArea[i]->ifYell=false;
            }
            if(fightArea[i]->fightNowResult==1){
                cout<<setTime(time_)<<":40 red "<<fightArea[i]->heroInHere[0]->name<<" "<<fightArea[i]->heroInHere[0]->numberInCamp<<" earned "<<fightArea[i]->element<<" elements for his headquarter"<<endl;
                redElement+=fightArea[i]->element;
                fightArea[i]->element=0;
            }
            if(fightArea[i]->raiseFlag==1&&fightArea[i]->flag!=1){
                cout<<setTime(time_)<<":40 red flag raised in city "<<i<<endl;
                fightArea[i]->raiseFlag=0;
                fightArea[i]->flag=1;
            }
        }else if(fightArea[i]->heroInHere[1]!=NULL&&fightArea[i]->heroInHere[0]==NULL){
            if(fightArea[i]->ifYell==true){
                cout<<setTime(time_)<<":40 blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" yelled in city "<<i<<endl;
                fightArea[i]->ifYell=false;
            }
            if(fightArea[i]->fightNowResult==-1){
                cout<<setTime(time_)<<":40 blue "<<fightArea[i]->heroInHere[1]->name<<" "<<fightArea[i]->heroInHere[1]->numberInCamp<<" earned "<<fightArea[i]->element<<" elements for his headquarter"<<endl;
                blueElement+=fightArea[i]->element;
                fightArea[i]->element=0;
            }
            if(fightArea[i]->raiseFlag==-1&&fightArea[i]->flag!=-1){
                cout<<setTime(time_)<<":40 blue flag raised in city "<<i<<endl;
                fightArea[i]->raiseFlag=0;
                fightArea[i]->flag=-1;
            }
        }


    }

    for (int i=N;i>=1;i--){
        if(fightArea[i]->fightNowResult==1){
            if(red->lifeElement>=8&&fightArea[i]->heroInHere[0]!=NULL){
                fightArea[i]->heroInHere[0]->hp+=8;
                red->lifeElement-=8;
            }
            fightArea[i]->fightNowResult=0;
        }
    }

    for (int i=1;i<=N;i++){
        if(fightArea[i]->fightNowResult==-1){
            if(blue->lifeElement>=8&&fightArea[i]->heroInHere[1]!=NULL){
                blue->lifeElement-=8;
                fightArea[i]->heroInHere[1]->hp+=8;
            }
            fightArea[i]->fightNowResult=0;
        }
    }

    red->lifeElement+=redElement;
    blue->lifeElement+=blueElement;

}

void reportElement(HeadQuarter* red,HeadQuarter* blue){
    cout<<setTime(time_)<<":50 "<<red->lifeElement<<" elements in red headquarter"<<endl;
    cout<<setTime(time_)<<":50 "<<blue->lifeElement<<" elements in blue headquarter"<<endl;
}

void reportWeapon(HeadQuarter* red,HeadQuarter* blue){
    for(int i=0;i<=N+1;i++){
        if(fightArea[i]->heroInHere[0]!=NULL){
            auto it=fightArea[i]->heroInHere[0];
            int weaponNumber=(it->weapon[0]!=NULL)+(it->weapon[1]!=NULL)+(it->weapon[2]!=NULL);
            cout<<setTime(time_)<<":55 red "<<it->name<<" "<<it->numberInCamp<<" has ";
            if(weaponNumber==0){
                cout<<"no weapon";
            }
            int seqWeapon[3]={2,1,0};
            int nowNumber=0;
            for (int i=0;i<3;i++){
                if(it->weapon[seqWeapon[i]]!=NULL){
                    if(i==0){
                        arrow* itarrow=dynamic_cast<arrow*>(it->weapon[2]);
                        cout<<"arrow("<<itarrow->used<<")";
                    }else if(i==1){
                        cout<<"bomb";
                    }else {
                        sword* itsword=dynamic_cast<sword*>(it->weapon[0]);
                        cout<<"sword("<<itsword->force<<")";
                    }
                    if((++nowNumber)!=weaponNumber){
                        cout<<",";
                    }
                }
            }
            cout<<endl;
        }
    }
    
    for(int i=0;i<=N+1;i++){
        if(fightArea[i]->heroInHere[1]!=NULL){
            auto it=fightArea[i]->heroInHere[1];
            int weaponNumber=(it->weapon[0]!=NULL)+(it->weapon[1]!=NULL)+(it->weapon[2]!=NULL);
            cout<<setTime(time_)<<":55 blue "<<it->name<<" "<<it->numberInCamp<<" has ";
            if(weaponNumber==0){
                cout<<"no weapon";
            }
            int seqWeapon[3]={2,1,0};
            int nowNumber=0;
            for (int i=0;i<3;i++){
                if(it->weapon[seqWeapon[i]]!=NULL){
                    if(i==0){
                        arrow* itarrow=dynamic_cast<arrow*>(it->weapon[2]);
                        cout<<"arrow("<<itarrow->used<<")";
                    }else if(i==1){
                        cout<<"bomb";
                    }else {
                        sword* itsword=dynamic_cast<sword*>(it->weapon[0]);
                        cout<<"sword("<<itsword->force<<")";
                    }
                    if((++nowNumber)!=weaponNumber){
                        cout<<",";
                    }
                }
            }
            cout<<endl;
        }
    }
}

void timeLine(HeadQuarter* &red,HeadQuarter* &blue){

    
    if(time_*60>T) return;
    red->born();
    blue->born();
    if(time_*60+5>T) return;
    lionScape();
    if(time_*60+10>T) return;
    moveOneStep();
    PrintMoveOneStep();
    if(gameWin!=0){
        return;
    }
    if(time_*60+30>T) return;
    produceElement();
    if(time_*60+35>T) return;
    shootarrows();
    if(time_*60+38>T) return;
    useBOMB();
    if(time_*60+40>T) return;
    battle(red,blue);
    if(time_*60+50>T) return;
    reportElement(red,blue);
    if(time_*60+55>T) return;
    reportWeapon(red,blue);

}

int main(){

    freopen("0.in","r",stdin);
    freopen("0.out","w",stdout);

    int t;
    cin>>t;
    for(int k=1;k<=t;k++){

        cin>>M>>N>>R>>K>>T;
        for (int i=0;i<5;i++){
            cin>>HeroHp[i];
        }
        for (int i=0;i<5;i++){
            cin>>HeroForce[i];
        }
        
        int seqRED[5]={2,3,4,1,0};
        int seqBLUE[5]={3,0,1,2,4};
        HeadQuarter* red=new HeadQuarter(0,seqRED,"red",M);
        HeadQuarter* blue=new HeadQuarter(1,seqBLUE,"blue",M);

        cout<<"Case "<<k<<":"<<endl;

        for (time_=0;time_<=T/60;time_++){
            
            timeLine(red,blue);
            if(gameWin!=0){
                break;
            }

        }

        for (int i=0;i<22;i++){
            fightArea[i]->rsize();
        }

        gameWin=0;

    }

    return 0;
}
