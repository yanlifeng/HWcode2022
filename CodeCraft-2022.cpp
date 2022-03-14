#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
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
vector<string>Sol(vector<int>users_val){
    vector<string>ansPath;
    num=0;
    Tn=N+M+2;
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
    while(Bfs())res+=Dfs(0,1e15);
    ll tar=0;
    for(int i=0;i<M;i++)tar+=users_val[i];
    if(is_debug)printf("ANS %lld %lld\n",res,tar);
    if(res!=tar)while(1){}
    for(int u=0;u<M;u++){
        string pat=users_name[u]+":";
        vector<string>tmp;
        for(int i=head[u+1];i!=-1;i=e[i].pre){
            if(e[i^1].t>0){
                tmp.push_back("<"+nodes_name[e[i].v-1-M]+","+to_string(e[i^1].t)+">");
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
    return ansPath;
}
int main() {
    inputData();    
    ofstream outstrm;
    if(is_local){
        outstrm.open("/Users/ylf9811/Downloads/huaweicode2022/solution.txt");
    }else{
        outstrm.open("/output/solution.txt");
    }
    if(!outstrm){
        printf("open out file fail\n");
    }
    for(int tt=0;tt<T;tt++){
        vector<string>res=Sol(G[tt]);
        if(is_debug){
            printf("---------------------------\n");
            for(auto it:res)printf("%s\n",it.c_str());
            printf("---------------------------\n");
        }
        for(int i=0;i<res.size()-1;i++){
            outstrm<<res[i]<<"\n";
        }
        outstrm<<res[res.size()-1];
        if(tt!=T-1)outstrm<<"\n";
    }

    outstrm.close();
    return 0;
}

