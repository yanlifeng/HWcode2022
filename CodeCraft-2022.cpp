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
#include <assert.h>
#define ll long long
const bool is_debug=0;
const bool is_local=0;
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
        in_path="/Users/ylf9811/data_45/data/";
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
int nosMaxVal[500];
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
vector<int>nos_val;
vector<int>us_val;
const double nodesLow=0.9;
const double nodesUp=0.95;
double nodesNow=nodesLow;
const double nodesScale=nodesUp-nodesLow;
bool user_cover[500];
int user_cover_all;
int user_cover_val;
double ruduFac=1.0;

ll SolMaxFlow(int limm){
    num=0;
    for(int i=0;i<Tn;i++)head[i]=-1;
    for(int i=0;i<M;i++){
        addEdge(0,i+1,us_val[i]);
        addEdge(i+1,0,0);
    }
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            if(dis[i][j]<DIS){
                addEdge(i+1,j+M+1,us_val[i]);
                addEdge(j+M+1,i+1,0);
            }
        }
    }
    for(int i=0;i<N;i++){
        //addEdge(i+M+1,Tn-1,max(min(nos_val[i],nosMaxVal[i]),min(limm,nos_val[i])));
        addEdge(i+M+1,Tn-1,min(limm,nos_val[i]));
        addEdge(Tn-1,i+M+1,0);
    }
    ll res=0;
    while(Bfs())
        res+=Dfs(0,1e15);
    return res;
}

ll SolMaxFlowNoLimit(vector<int>users_val){
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
    return res;
}


vector<string>Sol(vector<int>users_val){
    printf("\n\n============================================================================= \n\n");
    //init
    vector<string>ansPath;
    Tn=N+M+2;
    nos_val.clear();
    us_val.clear();
    for(int i=0;i<N;i++)
        nos_val.push_back(nodes_val[i]);
    for(int i=0;i<M;i++)
        us_val.push_back(users_val[i]);
    int nodesSum[500][500];
    for(int i=0;i<M;i++)
        for(int j=0;j<N;j++)
            nodesSum[i][j]=0;
    int nodeALim=N-ceil(N*nodesNow);
    int Tlim=T-ceil(T*0.95);
    nodesNow+=nodesScale/T;
    printf("nodesNow %lf\n",nodesNow);
    int useNodes=0;
    set<int>nodeSet;
    for(int i=0;i<N;i++)
        if(nodes_tims[i]<Tlim)
            nodeSet.insert(i);

    //=============================================================================
    //round1
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
            tmpNodes.push_back(make_pair(-(1.0*totSums[ii++]/totSumMax+ruduFac*(1.0*(N-rudu[i])/N)),i));
        }
        sort(tmpNodes.begin(),tmpNodes.end());
        int goodi=tmpNodes[0].second;
        if(-tmpNodes[0].first==ruduFac*(1.0*(N-rudu[goodi])/N))break;
        nodeSet.erase(goodi);

        useNodes++;
        nodes_tims[goodi]++;
        printf("this round get goodi %d, use %.4f\n",goodi,tmpNodes[0].first);
        vector<pair<int,int>> tmpUserNode;
        for (int i=0;i<M;i++) {
            tmpUserNode.push_back(make_pair(chudu[i],i));
        }
        sort(tmpUserNode.begin(),tmpUserNode.end());
        for(auto it:tmpUserNode){
            int i=it.second;
            if(dis[i][goodi]>=DIS)continue;
            int toUse=min(us_val[i],nos_val[goodi]);
            us_val[i]-=toUse;
            nos_val[goodi]-=toUse;
            nodesSum[i][goodi]+=toUse;
        }
    }
    printf("after round 1:\n");
    for(int i=0;i<M;i++)
        printf("%d ",us_val[i]);
    printf("\n");


    //=============================================================================
    //round2
    ll tar=0;
    for(int i=0;i<M;i++)tar+=us_val[i];
    int nodesMax=0;
    for(int i=0;i<M;i++)nodesMax=max(nodesMax,nos_val[i]);
    int anspos=-1;
    int delt=max(1,nodesMax/1000);
    for(int limm=1;limm<=nodesMax;limm+=delt){
        if(SolMaxFlow(limm)==tar){
            anspos=limm;
            break;
        }
    }
    assert(anspos!=-1);
    ll res=SolMaxFlow(anspos);
    //=============================================================================
    //round3
    if(res!=tar){
        printf("roud12 failed, now rerun MacFlow!\n");
        SolMaxFlowNoLimit(users_val);
        for(int i=0;i<M;i++)
            for(int j=0;j<N;j++)
                nodesSum[i][j]=0;
    }
    //=============================================================================
    //output
    int sumCost[500];
    for(int i=0;i<N;i++)sumCost[i]=0;
    for(int u=0;u<M;u++){
        for(int i=head[u+1];i!=-1;i=e[i].pre){
            if(e[i^1].t>0){
                nosMaxVal[e[i].v-1-M] = max(nosMaxVal[e[i].v-1-M],e[i^1].t);
                nodesSum[u][e[i].v-1-M]+=e[i^1].t;
            }
        }
    }
    for(int i=0;i<M;i++){
        string pat=users_name[i]+":";
        vector<string>tmpAns;
        for(int j=0;j<N;j++)
            if(nodesSum[i][j]){
                tmpAns.push_back("<"+nodes_name[j]+","+to_string(nodesSum[i][j])+">");
                sumCost[j]+=nodesSum[i][j];
            }
        if(tmpAns.size()){
            for(int i=0;i<tmpAns.size()-1;i++){
                pat+=tmpAns[i]+",";
            }
            pat+=tmpAns[tmpAns.size()-1];
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
            printf("----------%d\n",tmpCo[pos95-1].first);
        }
        printf("cost %lld\n",totCost);

    }
    return 0;
}
