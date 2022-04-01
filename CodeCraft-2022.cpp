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
const bool is_local=1;
using namespace std;
int N,M,T,V,DIS,Nflows[9000];
int dis[500][500];
int users_val[9000][40][110];
int nodes_val[140];
map<string,int>mp_users;
string users_name[500];
map<string,int>mp_nodes;
string nodes_name[500];
map<string,int>mp_times;
string times_name[9000];
map<string,int>mp_times_flows[9000];
string times_flows_name[9000][500];
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
        in_path="/Users/ylf9811/Downloads/hw_round2/data/";
    }else{
        in_path="/data/";
    }
    string infile1="demand.csv";
    string infile2="site_bandwidth.csv";
    string infile3="qos.csv";
    string infile4="config.ini";
    string inname=in_path+infile1;
    string line_data;
    //===================================
    freopen(inname.c_str(),"r",stdin);
    cin>>line_data;
    vector<string>tableTitle=splitByCom(line_data,',');
    M=0;
    for(int i=2;i<tableTitle.size();i++){
        mp_users[tableTitle[i]]=M;
        users_name[M]=tableTitle[i];
        M++;
    }
    T=0;
    while(cin>>line_data){
        vector<string>res=splitByCom(line_data,',');
        if(!mp_times.count(res[0])){
            mp_times[res[0]]=T;
            times_name[T]=res[0];
            T++;
        }
        int tid=mp_times[res[0]];
        if(!mp_times_flows[tid].count(res[1])){
            mp_times_flows[tid][res[1]]=Nflows[tid];
            times_flows_name[tid][Nflows[tid]]=res[1];
            Nflows[tid]++;
        }
        int fid=mp_times_flows[tid][res[1]];
        for(int i=2;i<res.size();i++){
            users_val[tid][i-2][fid]=stoi(res[i]);
        }
    }
    fclose(stdin);
    cin.clear();
    //=====================================
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
    cin>>line_data;
    V=stoi(splitByCom(line_data,'=')[1]);
    fclose(stdin);
    cin.clear();
    //============================
    printf("N %d M %d T %d\n",N,M,T);
    /*1
    printf("Nflows: ");
    for(int i=0;i<T;i++){
        printf("%d ",Nflows[i]);
    }
    printf("\n");
    printf("users val:\n");
    for(int i=0;i<T;i++){
        for(int j=0;j<M;j++){
            for(int k=0;k<Nflows[i];k++){
                printf("%d %d %d %d\n",i,j,k,users_val[i][j][k]);
            }
        }
    }
    printf("nodes val:\n");
    for(int i=0;i<N;i++){
        printf("%d ",nodes_val[i]);
    }
    printf("\n");
    printf("DIS %d\n",DIS);
    printf("V %d\n",V);
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
    */
}
void baselineSolve(){
    ofstream outstrm;
    if(is_local){
        outstrm.open("/Users/ylf9811/Downloads/hw_round2/output/solution.txt");
    }else{
        outstrm.open("/output/solution.txt");
    }
    int nos_val[500];
    int us_val[40][110];

    for(int t=0;t<T;t++){
        int Nf=Nflows[t];
        for(int nid=0;nid<N;nid++)
            nos_val[nid]=nodes_val[nid];
        for(int uid=0;uid<M;uid++){
            for(int fid=0;fid<Nf;fid++){
                us_val[uid][fid]=users_val[t][uid][fid];
            }
        }
        for(int uid=0;uid<M;uid++){
            outstrm<<users_name[uid]<<":";
            vector<string>nodes_ans[140];
            for(int fid=0;fid<Nflows[t];fid++){
                int ok=0;
                for(int nid=0;nid<N;nid++){
                    if(dis[uid][nid]>=DIS)continue;
                    if(us_val[uid][fid]==0){
                        ok=1;
                        break;
                    }
                    if(nos_val[nid]>=us_val[uid][fid]){
                        ok=1;
                        nos_val[nid]-=us_val[uid][fid];
                        nodes_ans[nid].push_back(times_flows_name[t][fid]);
                        break;
                    }
                }
                assert(ok);
            }
            int las_id=-1;
            for(int nid=N-1;nid>=0;nid--)
                if(nodes_ans[nid].size()){
                    las_id=nid;
                    break;
                }
            for(int nid=0;nid<=las_id;nid++){
                if(nodes_ans[nid].size()==0)continue;
                outstrm<<"<"<<nodes_name[nid]<<",";
                int nn=nodes_ans[nid].size();
                for(int i=0;i<nn;i++){
                    if(i<nn-1)outstrm<<nodes_ans[nid][i]<<",";
                    else outstrm<<nodes_ans[nid][i];
                }
                if(nid<las_id)outstrm<<">,";
                else outstrm<<">";
            }
            outstrm<<"\n";
        }
    }
    outstrm.close();
}
int main() {
    inputData();
    baselineSolve();
    return 0;
}
