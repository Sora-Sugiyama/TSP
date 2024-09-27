//
//  greedy.h
//  TSP
//
//  Created by Sora Sugiyama on 9/26/24.
//

#ifndef greedy_h
#define greedy_h

#include "disjoint_set.h"
#include <algorithm>
#include <vector>
#include <iostream>

namespace TSP{

double greedy(int rout[],problem &X){
    std::vector<std::pair<int,int> >edges;
    edges.resize(X.dimension*(X.dimension-1)>>1);
    disjoint_set ds;
    ds.resize(X.dimension);
    
    int i,j,idx=0;
    for(i=0;i<X.dimension;i++){
        for(j=0;j<i;j++){
            edges[idx++]={j,i};
        }
    }
    
    std::sort(edges.begin(),edges.end(),[&X](const std::pair<int,int>&a,const std::pair<int,int>&b){
        return X.W[a.first][a.second]<X.W[b.first][b.second];
    });
    
    idx=0;
    std::vector<std::vector<int> >G;
    G.resize(X.dimension);
    
    for(auto &e:edges){
        if(G[e.first].size()>=2||G[e.second].size()>=2)continue;
        if(ds.Disjoint(e.first,e.second)){
            ds.Union(e.first,e.second);
            G[e.first].push_back(e.second);
            G[e.second].push_back(e.first);
        }
    }
    
    int u=0;
    for(i=0;i<X.dimension;i++){
        if(G[i].size()==1){u=i;break;}
    }
    
    std::vector<bool>vis=std::vector<bool>(X.dimension,false);
    vis[u]=true;
    while(1){
        bool stop=true;
        rout[idx++]=u;
        for(int &v:G[u]){
            if(!vis[v]){
                vis[v]=true;
                stop=false;
                u=v;
                break;
            }
        }
        if(stop)break;
    }
    
    if(X.dimension!=idx){
        std::cout<<"FAIL"<<std::endl;
    }
    return loss(rout,X);
}

}

#endif /* greedy_h */
