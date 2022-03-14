#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
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
int node_cost[500][10010];

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
    string in_path="/Users/ylf9811/Downloads/huaweicode2022/smallData/";
    string out_path="/Users/ylf9811/Downloads/huaweicode2022/smallData/";
    string infile1="demand.csv";
    string infile2="site_bandwidth.csv";
    string infile3="qos.csv";
    string infile4="config.ini";
    string inname=in_path+infile1;
    printf("in %s\n",inname.c_str());
    ifstream instrm;
    string line_data;
    //=================================================================
    instrm.open(inname, ios::out | ios::in );
    instrm>>line_data;
    vector<string>tableTitle=splitByCom(line_data,',');
    M=0;
    for(int i=1;i<tableTitle.size();i++){
        mp_users[tableTitle[i]]=M;
        users_name[M]=tableTitle[i];
        M++;
    }
    T=0;
    while(instrm>>line_data){
        vector<string>res=splitByCom(line_data,',');
        for(int i=1;i<res.size();i++){
            G[T].push_back(stoi(res[i]));
        }
        T++;
    }
    instrm.close();
    //=============================================================
    inname=in_path+infile2;
    instrm.open(inname, ios::out | ios::in );
    instrm>>line_data;
    N=0;
    while(instrm>>line_data){
        vector<string>res=splitByCom(line_data,',');
        mp_nodes[res[0]]=N;
        nodes_name[N]=res[0];
        nodes_val[N]=stoi(res[1]);
        N++;
    }
    instrm.close();
    //=============================================================
    inname=in_path+infile3;
    instrm.open(inname, ios::out | ios::in );
    instrm>>line_data;
    tableTitle=splitByCom(line_data,',');
    while(instrm>>line_data){
        vector<string>res=splitByCom(line_data,',');
        for(int i=1;i<res.size();i++){
            int uid=mp_users[tableTitle[i]];
            int nid=mp_nodes[res[0]];
            dis[uid][nid]=stoi(res[i]);
        }
    }
    instrm.close();

    //=============================================================
    inname=in_path+infile4;
    instrm.open(inname, ios::out | ios::in );
    instrm>>line_data;
    instrm>>line_data;
    DIS=stoi(splitByCom(line_data,'=')[1]);
    instrm.close();
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
vector<int>Sol(vector<int>users_val,vector<int>users_sort){
    vector<int>res;
    int las[500];
    for(int i=0;i<N;i++)las[i]=nodes_val[i];
    for(int i=0;i<M;i++){



    }
    return res;
}
int main() {
    inputData();    
    vector<pair<int,int>>users_sort;
    for(int i=0;i<M;i++){
        int cnt=0;
        for(int j=0;j<N;j++)
            if(dis[i][j]<=DIS)cnt++;
        users_sort.push_back(make_pair(cnt,i));
    }
    sort(users_sort.begin(),users_sort.end());
    vector<int>users_sorted;
    for(auto it:users_sort)users_sorted.push_back(it.second);
    printf("--------------------------------\n");
    printf("sort users:\n");
    for(auto it:users_sort){
        printf("%d %d\n",it.second,it.first);
    }

    printf("--------------------------------\n");
    for(int tt=0;tt<T;tt++){
        vector<int>res=Sol(G[tt],users_sorted);
    }     
    return 0;
}
