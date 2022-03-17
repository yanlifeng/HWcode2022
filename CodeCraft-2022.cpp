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
        for(int i=head[now];i!=-1;i=e[i].pre){
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
vector<string>Sol(vector<int>users_val,double limm,bool hasOut){
    vector<string>ansPath;
    Tn=N+M+2;
    ll usersSum=0;
    for(int i=0;i<M;i++)usersSum+=users_val[i];
    ll nodesSum=0;
    for(int i=0;i<N;i++)nodesSum+=nodes_val[i];
    printf("userSum %lld; nodeSum %lld\n",usersSum,nodesSum);
    if(usersSum<=nodesSum*0.05){
    //if(1){
        printf("now use method 2\n");
        vector<int>nos_val;
        for(int i=0;i<N;i++)
            nos_val.push_back(nodes_val[i]);
        vector<int>us_val;
        for(int i=0;i<M;i++)
            us_val.push_back(users_val[i]);
        vector<int>randPs0;
        vector<int>randPs1;
        vector<int>randPs2;
        for(int i=0;i<N;i++)
            randPs0.push_back(i);
        random_shuffle(randPs0.begin(),randPs0.end(),myrandom); 
        int p5pos=int(N*0.05);
        for(int i=0;i<p5pos;i++)
            randPs1.push_back(randPs0[i]);
        for(int i=p5pos;i<N;i++)
            randPs2.push_back(randPs0[i]);
        printf("random choose :\n");
        for(auto it:randPs1)printf("%d ",it);
        printf("\n");
        vector<pair<int,int>>userTo[500];
        for(int i=0;i<M;i++){
            for(auto j:randPs1){
                if(dis[i][j]>=DIS)continue;
                int toUse=min(us_val[i],nos_val[j]);
                us_val[i]-=toUse;
                nos_val[j]-=toUse;
                if(toUse){
                    userTo[i].push_back(make_pair(j,toUse));
                }
            }
        }
        printf("=====================\n");
        printf("round 1:\n");
        for(int i=0;i<M;i++)
            printf("%d ",us_val[i]);
        printf("\n");
        //for(int i=0;i<N;i++)
        //    printf("%d ",nos_val[i]);
        //printf("\n");
        for(int i=0;i<M;i++){
            for(auto j:randPs2){
                if(dis[i][j]>=DIS)continue;
                int toUse=min(us_val[i],nos_val[j]);
                us_val[i]-=toUse;
                nos_val[j]-=toUse;
                if(toUse){
                    userTo[i].push_back(make_pair(j,toUse));
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
        addEdge(i+M+1,Tn-1,int(limm*nodes_val[i]/100.0));
        addEdge(Tn-1,i+M+1,0);
    }
    ll res=0;
    while(Bfs())res+=Dfs(0,1e15);
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
    if(hasOut){
        for(int i=0;i<N;i++)Co[i].push_back(sumCost[i]);
    }
    return ansPath;
}
int main() {
    std::srand ( unsigned ( std::time(0) ) );
    inputData();    
    ofstream outstrm;
    if(is_local){
        outstrm.open("/Users/ylf9811/Downloads/CodeCraft2022-charge-main/output/solution.txt");
    }else{
        outstrm.open("/output/solution.txt");
    }
    if(!outstrm){
        printf("open out file fail\n");
    }
    for(int tt=0;tt<T;tt++){
        /*
           double l=0.01,r=100,anspos=-1;;
           for(int i=1;i<20;i++){
           double mid=(l+r)/2;
           vector<string>ress=Sol(G[tt],mid,0);
           if(ress.size()){
           r=mid;
           anspos=mid;
           }else {
           l=mid;
           }
           }
           vector<string>res=Sol(G[tt],anspos,1);
           */
        vector<string>res=Sol(G[tt],100,1);
        if(is_debug){
            printf("---------------------------\n");
            for(auto it:res)printf("%s\n",it.c_str());
            printf("---------------------------\n");
        }
        for(int i=0;i<res.size()-1;i++){
            outstrm<<res[i]<<"\n";
        }
        if(res.size())outstrm<<res[res.size()-1];
        if(tt!=T-1)outstrm<<"\n";
    }
    outstrm.close();
    if(is_local){
        ll totCost=0;
        int pos95=ceil(T*95/100.0);
        printf("pos %d\n",pos95);
        for(int i=0;i<N;i++){
            sort(Co[i].begin(),Co[i].end());
            totCost+=Co[i][pos95-1];
        }
        printf("cost %lld\n",totCost);

    }
    return 0;
}

