#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <set>
#include <random>
#define ll long long
const bool is_debug=0;
const bool is_local=1;
using namespace std;
map<string,int>mp_users;
string users_name[500];
map<string,int>mp_nodes;
string nodes_name[500];
vector<int>G[10010];
int nodes_val[500];
int dis[500][500];
int DIS;
int T,M,N;
int cnttt;
ll totUserVal;
int chudu[500];
int rudu[500];

vector<string> splitByCom(string data,char c){
    vector<string>res;
    stringstream ss(data);
    string si("");
    while(getline(ss, si, c)) {
        res.push_back(si);
    }
    return res;
}
void inputData(){
    string in_path;
    if(is_local){
        //in_path="/Users/ylf9811/Downloads/huaweicode2022/smallData/";
        in_path="/Users/ylf9811/Downloads/huaweicode2022/data/";
    }else{
        in_path="/data/";
    }
    string infile1="demand.csv";
    string infile2="site_bandwidth.csv";
    string infile3="qos.csv";
    string infile4="config.ini";
    string inname=in_path+infile1;
    string line_data;
    //=================================================================
    freopen(inname.c_str(),"r",stdin);
    cin>>line_data;
    vector<string>tableTitle=splitByCom(line_data,',');
    M=0;
    for(int i=1;i<tableTitle.size();i++){
        mp_users[tableTitle[i]]=M;
        users_name[M]=tableTitle[i];
        M++;
    }
    T=0;
    while(cin>>line_data){
        vector<string>res=splitByCom(line_data,',');
        for(int i=1;i<res.size();i++){
            G[T].push_back(stoi(res[i]));
            totUserVal+=stoi(res[i]);
        }
        T++;
    }
    fclose(stdin);
    cin.clear();
    //=============================================================
    inname=in_path+infile2;
    freopen(inname.c_str(),"r",stdin);
    cin>>line_data;
    N=0;
    while(cin>>line_data){
        vector<string>res=splitByCom(line_data,',');
        mp_nodes[res[0]]=N;
        nodes_name[N]=res[0];
        nodes_val[N]=stoi(res[1]);
        N++;
    }
    fclose(stdin);
    cin.clear();
    //=============================================================
    inname=in_path+infile3;
    freopen(inname.c_str(),"r",stdin);
    cin>>line_data;
    tableTitle=splitByCom(line_data,',');
    while(cin>>line_data){
        vector<string>res=splitByCom(line_data,',');
        for(int i=1;i<res.size();i++){
            int uid=mp_users[tableTitle[i]];
            int nid=mp_nodes[res[0]];
            dis[uid][nid]=stoi(res[i]);
        }
    }
    fclose(stdin);
    cin.clear();
    //=============================================================
    inname=in_path+infile4;
    freopen(inname.c_str(),"r",stdin);
    cin>>line_data;
    cin>>line_data;
    DIS=stoi(splitByCom(line_data,'=')[1]);
    fclose(stdin);

    cin.clear();
    if(is_debug){
        printf("DIS %d\n",DIS);
        printf("TABLE: \n");
        for(int i=0;i<N;i++)printf("%s ",nodes_name[i].c_str());
        printf("\n");
        for(int i=0;i<M;i++)printf("%s ",users_name[i].c_str());
        printf("\n");
        printf("================================\n");
        for(int i=0;i<M;i++){
            for(int j=0;j<N;j++)
                printf("%d ",dis[i][j]);
            printf("\n");
        }
        printf("================================\n");
    }
}
struct edge{
    int v,t,pre;
}e[500*500];
int num,head[500],ord[500],Tn;
vector<int>Co[500];
int myrandom (int i) { return std::rand()%i;}
void addEdge(int from,int to,int val){
    e[num].v=to;
    e[num].t=val;
    e[num].pre=head[from];
    head[from]=num++;
}
bool Bfs(){
    for(int i=0;i<Tn;i++)ord[i]=-1;
    queue<int>Q;
    Q.push(0);
    ord[0]=0;
    while(!Q.empty()){
        int now=Q.front();
        Q.pop();
        vector<int>Ei;
        for(int i=head[now];i!=-1;i=e[i].pre)
            Ei.push_back(i);
        random_shuffle ( Ei.begin(), Ei.end() );
        for(auto i:Ei){
            if(e[i].t>0&&ord[e[i].v]==-1){
                Q.push(e[i].v);
                ord[e[i].v]=ord[now]+1;
                if(e[i].v==Tn-1)return 1;
            }
        }
    }
    return 0;
}
ll Dfs(int now,ll nowflow){
    if(now==Tn-1)return nowflow;
    int totflow=0;
    for(int i=head[now];i!=-1;i=e[i].pre){
        if(ord[e[i].v]==ord[now]+1&&e[i].t>0){
            int canflow=Dfs(e[i].v,min(nowflow,1ll*e[i].t));
            e[i].t-=canflow;
            e[i^1].t+=canflow;
            totflow+=canflow;
            nowflow-=canflow;
            if(nowflow<=0)break;
        }
    }
    return totflow;
}
int nodes_tims[500];
bool bad_nodes[500];
int cntt;
const double nodesLow=0.9;
const double nodesUp=0.95;
double nodesNow=nodesLow;
const double nodesScale=nodesUp-nodesLow;
ll totUserValLei;
double ruduFac=0.5;
vector<string>Sol(vector<int>users_val){
    std::random_device rd;
    std::mt19937 rng(rd());
    ll totUserValNow=0;
    for(auto it:users_val)
        totUserValNow+=it;
    vector<string>ansPath;
    Tn=N+M+2;
    ll usersSum=0;
    for(int i=0;i<M;i++)usersSum+=users_val[i];
    ll nodesSum=0;
    for(int i=0;i<N;i++)nodesSum+=nodes_val[i];
    printf("userSum %lld; nodeSum %lld\n",usersSum,nodesSum);
    cntt++;
    //if(cntt<=T*0.75){
    //if(usersSum<=nodesSum*0.01){
    if(1){
        printf("now use method 2\n");
        vector<int>nos_val;
        for(int i=0;i<N;i++)
            nos_val.push_back(nodes_val[i]);
        vector<int>us_val;
        for(int i=0;i<M;i++)
            us_val.push_back(users_val[i]);
        int nodeALim=N-ceil(N*nodesNow);
        int Tlim=T-ceil(T*0.95);
        //nodesNow+=nodesScale*totUserValNow/totUserVal;
        nodesNow+=nodesScale/T;
        printf("nodesNow %lf\n",nodesNow);
        int useNodes=0;
        set<int>nodeSet;
        vector<int>randPs;
        vector<pair<int,int>>userTo[500];
        //if(totUserValLei<totUserVal*0.9)
        if(1)
        {
            //if(totUserValNow*1.3591*T>=totUserVal){
            for(int i=0;i<N;i++)
                if(nodes_tims[i]<Tlim)
                    nodeSet.insert(i);
            while(nodeSet.size()){
                if(useNodes>=nodeALim)break;
                int done=1;
                for(int i=0;i<M;i++)
                    if(us_val[i])done=0;
                if(done)break;
                vector<pair<double,int>>tmpNodes;
                ll totSumMax=0;
                vector<ll>totSums;
                for(auto i:nodeSet){
                    ll totSum=0;
                    for(int j=0;j<M;j++)
                        if(dis[j][i]<DIS)
                            totSum+=us_val[j];
                    totSum=min(totSum,1ll*nos_val[i]);
                    totSums.push_back(totSum);
                    totSumMax=max(totSumMax,totSum);
                }
                int ii=0;
                for(auto i:nodeSet){
                    //tmpNodes.push_back(make_pair(-(1.0*totSums[ii++]/totSumMax+ruduFac/max(rudu[i],1)),i));
                    tmpNodes.push_back(make_pair(-(1.0*totSums[ii++]/totSumMax+ruduFac*(1.0*(N-rudu[i])/N)),i));
                    //tmpNodes.push_back(make_pair(-1.0*totSums[ii++],i));
                }
                sort(tmpNodes.begin(),tmpNodes.end());
                printf("tmpNodes size %lu\n",tmpNodes.size());
                int goodi=tmpNodes[0].second;
                //if(-tmpNodes[0].first==ruduFac/max(rudu[goodi],1))break;
                if(-tmpNodes[0].first==ruduFac*(1.0*(N-rudu[goodi])/N))break;
                nodeSet.erase(goodi);
                ll sumTmpUser=0;
                for(int i=0;i<M;i++){
                    sumTmpUser+=us_val[i];
                }
                //TODO
                //if(-tmpNodes[0].first<=sumTmpUser*0.2){
                //    break;
                //}
                useNodes++;
                nodes_tims[goodi]++;
                printf("this round get goodi %d, use %.4f\n",goodi,tmpNodes[0].first);
                for(int i=0;i<M;i++){
                    if(dis[i][goodi]>=DIS)continue;
                    int toUse=min(us_val[i],nos_val[goodi]);
                    us_val[i]-=toUse;
                    nos_val[goodi]-=toUse;
                    if(toUse){
                        userTo[i].push_back(make_pair(goodi,toUse));
                    }
                }
                printf("round 1:\n");
                for(int i=0;i<M;i++)
                    printf("%d ",us_val[i]);
                printf("\n");
            }
        }
        totUserValLei+=totUserValNow;
        for(int i=0;i<N;i++)randPs.push_back(i);
        printf("=====================\n");
        int tmp_us_val[500];
        int tmp_nos_val[500];
        for(int i=0;i<M;i++)tmp_us_val[i]=us_val[i];
        for(int i=0;i<N;i++)tmp_nos_val[i]=nos_val[i];
        int tmpAns[500][500];
        for(int i=0;i<M;i++)
            for(int j=0;j<N;j++)
                tmpAns[i][j]=0;
        for(int i=0;i<M;i++){
            int rudusum=0;
            for(int j=0;j<N;j++)
                if(nos_val[j]&&dis[i][j]<DIS)
                    rudusum+=rudu[j];
            int canCnt=0;
            for(int j=0;j<N;j++)
                if(nos_val[j]&&dis[i][j]<DIS)
                    canCnt++;
            for(int j=0;j<N;j++){
                if(dis[i][j]>=DIS||nos_val[j]==0)continue;
                double FacMa=(canCnt-1)*rudusum;
                double FacSon=rudusum-rudu[j];
                int bestVal=us_val[i]*FacSon/FacMa;
                bestVal=min(bestVal,nos_val[j]);
                us_val[i]-=bestVal;
                nos_val[j]-=bestVal;
                if(bestVal){
                    tmpAns[i][j]+=bestVal;
                }
            }
            if(us_val[i]){
                vector<int>randids;
                for(int j=0;j<N;j++)randids.push_back(j);
                shuffle(randids.begin(),randids.end(),rng);
                for(auto j:randids){
                    if(dis[i][j]>=DIS||nos_val[j]==0)continue;
                    int babaVal=min(us_val[i],nos_val[j]);
                    us_val[i]-=babaVal;
                    nos_val[j]-=babaVal;
                    if(babaVal){
                        tmpAns[i][j]+=babaVal;
                    }
                    if(us_val[i]==0)break;
                    
                }
            }
        }
        int ok_new2=1;
        for(int i=0;i<M;i++)
            if(us_val[i])ok_new2=0;
        if(ok_new2==0){
            for(int i=0;i<M;i++)us_val[i]=tmp_us_val[i];
            for(int i=0;i<N;i++)nos_val[i]=tmp_nos_val[i];
            for(int i=0;i<M;i++){
                int canCnt=0;
                for(auto j:randPs)
                    if(dis[i][j]<DIS)canCnt++;
                int avgVal=ceil(1.0*us_val[i]/canCnt);
                int tmpVl[500];
                for(int j=0;j<N;j++)
                    tmpVl[j]=0;
                for(int t=0;;t++){
                    int preVal=us_val[i];
                    for(auto j:randPs){
                        if(dis[i][j]>=DIS)continue;
                        int toUse=min(min(avgVal,us_val[i]),nos_val[j]);
                        us_val[i]-=toUse;
                        nos_val[j]-=toUse;
                        tmpVl[j]+=toUse;
                    }
                    if(us_val[i]==preVal||us_val[i]==0)break;
                }
                for(auto j:randPs){
                    if(tmpVl[j])
                        userTo[i].push_back(make_pair(j,tmpVl[j]));
                }
            }
        }else{
            for(int i=0;i<M;i++){
                for(int j=0;j<N;j++){
                    if(tmpAns[i][j])
                        userTo[i].push_back(make_pair(j,tmpAns[i][j]));
                }
            }
        } 
        printf("round 2:\n");
        for(int i=0;i<M;i++)
            printf("%d ",us_val[i]);
        printf("\n");
        //for(int i=0;i<N;i++)
        //    printf("%d ",nos_val[i]);
        //printf("\n");
        printf("=====================\n");
        int ok=1;
        for(int i=0;i<M;i++)
            if(us_val[i]>0)ok=0;
        if(ok){
            int sumCost[500];
            for(int i=0;i<N;i++)sumCost[i]=0;
            for(int i=0;i<M;i++){
                string pat=users_name[i]+":";
                vector<string>tmp;
                for(auto it:userTo[i]){
                    tmp.push_back("<"+nodes_name[it.first]+","+to_string(it.second)+">");
                    sumCost[it.first]+=it.second;
                }
                if(tmp.size()){
                    for(int ii=0;ii<tmp.size()-1;ii++){
                        pat+=tmp[ii]+",";
                    }
                    pat+=tmp[tmp.size()-1];
                }
                ansPath.push_back(pat);
            }
            for(int i=0;i<N;i++)Co[i].push_back(sumCost[i]);
            return ansPath;
        }
        else printf("method 2 failed\n");
        }
        ansPath.clear();
        cnttt++;
        num=0;
        for(int i=0;i<Tn;i++)head[i]=-1;
        for(int i=0;i<M;i++){
            addEdge(0,i+1,users_val[i]);
            addEdge(i+1,0,0);
        }
        for(int i=0;i<M;i++){
            for(int j=0;j<N;j++){
                if(dis[i][j]<DIS){
                    addEdge(i+1,j+M+1,users_val[i]);
                    addEdge(j+M+1,i+1,0);
                }
            }
        }
        for(int i=0;i<N;i++){
            addEdge(i+M+1,Tn-1,nodes_val[i]);
            addEdge(Tn-1,i+M+1,0);
        }
        ll res=0;
        while(Bfs())
            res+=Dfs(0,1e15);
        ll tar=0;
        for(int i=0;i<M;i++)tar+=users_val[i];
        if(is_debug)printf("ANS %lld %lld\n",res,tar);
        if(res!=tar)return ansPath;
        int sumCost[500];
        for(int i=0;i<N;i++)sumCost[i]=0;
        for(int u=0;u<M;u++){
            string pat=users_name[u]+":";
            vector<string>tmp;
            for(int i=head[u+1];i!=-1;i=e[i].pre){
                if(e[i^1].t>0){
                    tmp.push_back("<"+nodes_name[e[i].v-1-M]+","+to_string(e[i^1].t)+">");
                    sumCost[e[i].v-1-M]+=e[i^1].t;
                }
            }
            if(tmp.size()){
                for(int i=0;i<tmp.size()-1;i++){
                    pat+=tmp[i]+",";
                }
                pat+=tmp[tmp.size()-1];
            }
            ansPath.push_back(pat);
        }
        for(int i=0;i<N;i++)Co[i].push_back(sumCost[i]);
        return ansPath;
    }

    int main() {
        std::srand ( unsigned ( std::time(0) ) );
        inputData();
        printf("M:%d,N:%d\n\n",M,N);
        printf("chu du:\n");
        for(int i=0;i<M;i++){
            printf("%s:",users_name[i].c_str());
            int cnt=0;
            for(int j=0;j<N;j++)
                if(dis[i][j]<DIS)cnt++;
            printf(" %d\n",cnt);
            chudu[i]=cnt;
        }
        printf("ru du:\n");
        for(int j=0;j<N;j++){
            printf("%s:",nodes_name[j].c_str());
            int cnt=0;
            for(int i=0;i<M;i++)
                if(dis[i][j]<DIS)cnt++;
            printf(" %d\n",cnt);
            rudu[j]=cnt;
            if(cnt<=M*0.2)bad_nodes[j]=1;
        }
        ofstream outstrm;
        if(is_local){
            outstrm.open("/Users/ylf9811/Downloads/CodeCraft2022-charge-main/output/solution.txt");
        }else{
            outstrm.open("/output/solution.txt");
        }
        if(!outstrm){
            printf("open out file fail\n");
        }
        vector<pair<ll,int>>Tis;
        for(int tt=0;tt<T;tt++){
            ll sum=0;
            for(auto it:G[tt])sum+=it;
            Tis.push_back(make_pair(-sum,tt));
        }
        sort(Tis.begin(),Tis.end());
        vector<pair<int,vector<string>>>AAns;
        for(int tt=0;tt<T;tt++){
            printf("day %d %d\n",tt,Tis[tt].second);
            int tid=Tis[tt].second;
            vector<string>res=Sol(G[tid]);
            AAns.push_back(make_pair(tid,res));
            if(is_debug){
                printf("---------------------------\n");
                for(auto it:res)printf("%s\n",it.c_str());
                printf("---------------------------\n");
            }
        }
        //if(cnttt>T*0.001)return 0;
        sort(AAns.begin(),AAns.end());
        for(auto it:AAns){
            auto res=it.second;
            for(auto item:res){
                outstrm<<item<<"\n";
            }
        }
        outstrm.close();
        if(is_local){
            printf("%d\n",T);
            printf("tims:\n");
            for(int i=0;i<N;i++)printf("%d ",nodes_tims[i]);
            printf("\n");
            ll totCost=0;
            int pos95=ceil(T*95/100.0);
            printf("pos %d\n",pos95);
            for(int i=0;i<N;i++){
                printf("node name:%s, random use time:%d, rudu:%d: ",nodes_name[i].c_str(),nodes_tims[i],rudu[i]);
                vector<pair<int,int>>tmpCo;
                for(int t=0;t<Co[i].size();t++){
                    tmpCo.push_back(make_pair(Co[i][t],t));
                }
                sort(tmpCo.begin(),tmpCo.end());
                for(auto it:tmpCo)printf("%d %d, ",it.first,it.second);
                printf("\n");
                totCost+=tmpCo[pos95-1].first;
                for(int t=4;t>=0;t--){
                    int uid=tmpCo[tmpCo.size()-t-1].second;
                    ll sum=0;
                    for(int j=0;j<M;j++)
                        if(dis[j][i]<DIS)sum+=G[uid][j];
                    printf("%lld %d , ",sum,uid);
                }
                printf("----------%d\n",tmpCo[pos95-1].first);
            }
            printf("cnttt %d\n",cnttt);
            printf("cost %lld\n",totCost);

        }
        return 0;
    }

