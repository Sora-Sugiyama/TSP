//
//  LKH.h
//  TSP
//
//  Created by Sora Sugiyama on 10/22/24.
//

#ifndef LKH_h
#define LKH_h

#include <algorithm>
#include <vector>
#include <iostream>
#include "TSP.h"

namespace TSP{

class Helsagon{
    void NNM(std::vector<std::vector<int> >&para, problem &X){
        const int n=X.dimension;
        para.resize(n);
        std::vector<std::pair<double,int> >vp(n);
        int i,j;
        for(i=0;i<n;i++){
            para[i].resize(n);
            for(j=0;j<n;j++){
                vp[j]={X.W[i][j]+(i==j)*1e18,j};
            }
            sort(vp.begin(),vp.end());
            
            for(j=0;j<n;j++)para[i][j]=vp[j].second;
        }
    }
    
    std::vector<bool>use,vis;
    std::vector<int>nxt,pre,pos;
    std::vector<std::pair<int,int> >adj;
    std::vector<std::vector<int> >nnm;
    
public:
    
    void HelsagonNNM(problem &X){
        nnm.clear();
        NNM(nnm,X);
        adj.clear();
        adj.resize(X.dimension);
    }
    
    void edge(const int &u,const int &v,bool flag){
        if(flag){
            if(adj[v].first==-1)adj[v].first=u;
            else adj[v].second=u;
            
            if(adj[u].first==-1)adj[u].first=v;
            else adj[u].second=v;
            
        }else{
            adj[v]={(adj[v].first==u?-1:adj[v].first),(adj[v].second==u?-1:adj[v].second)};
            adj[u]={(adj[u].first==v?-1:adj[u].first),(adj[u].second==v?-1:adj[u].second)};
        }
    }
    
    // if construct==false, function do only the Hamiltonicity check.
    bool constructNewTour(int rout[],const int &n,bool construct=true){
        vis=std::vector<bool>(n,0);
        
        // Hamiltonicity Check
        int u=0,i;
        vis[u]=true;
        for(i=1;i<n;i++){
            if(adj[u].first<0||adj[u].second<0)return false;
            if(!vis[adj[u].first]){
                vis[adj[u].first]=true;
                u=adj[u].first;
            }else if(!vis[adj[u].second]){
                vis[adj[u].second]=true;
                u=adj[u].second;
            }else{
                // Tour can not be close
                return false;
            }
        }

        if(adj[u].first<0||adj[u].second<0)return false;
        if(adj[u].first!=0&&adj[u].second!=0)return false;
        if(!construct)return true;
        
        fill(vis.begin(),vis.end(),false);
        rout[0]=0;
        vis[0]=true;
        for(i=1;i<n;i++){
            u=rout[i-1];
            if(!vis[adj[u].first]){
                vis[adj[u].first]=true;
                rout[i]=adj[u].first;
            }else{
                vis[adj[u].second]=true;
                rout[i]=adj[u].second;
            }
        }
        return true;
    }

    double LKHbacktracking(int rout[],problem &X,int x,int y,double g1,double g2,int k){
        if(k>5)return -1;
        if(k>2){
            if(constructNewTour(rout, X.dimension,false))return -1;
        }
        int z1,z2,i,j;
        double found=-1;
        for(i=0;i<5;i++){
            z1=nnm[y][i];
            
            if(use[z1]||adj[z1].first==y||adj[z1].second==y)continue;
            use[z1]=true;
            for(j=0;j<2;j++){
                z2=j==0?pre[z1]:nxt[z1];
                
                if(use[z2]||adj[z2].first==x||adj[z2].second==x)continue;
                use[z2]=true;
                
                g1+=X.W[z1][z2];
                edge(z1,z2,false);
                
                g2+=X.W[y][z1];
                edge(y,z1,true);
                if(g1>g2+X.W[z2][x]){
                    edge(z2,x,true);
                    if(constructNewTour(rout,X.dimension)){
                        return loss(rout,X);
                    }
                    edge(z2,x,false);
                }
                found=LKHbacktracking(rout, X, x, z2, g1, g2, k+1);
                if(found>=0)return found;
                
                edge(y,z1,false);
                edge(z1,z2,true);
                use[z2]=false;
                g2-=X.W[y][z1];
                g1-=X.W[z1][z2];
            }
            use[z1]=false;
        }
        
        // fail
        return -1;
    }
  
    double LKHstep(int rout[], problem &X){
        const int n=X.dimension;
        int u,v,w;
        use=std::vector<bool>(n,false);
        pos=std::vector<int>(n,false);
        pre=std::vector<int>(n,false);
        nxt=std::vector<int>(n,false);
        
        int x1,x2,i;
        for(int i=0;i<n;i++){
            w=rout[i];u=rout[(i+1)%n];v=rout[(i-1+n)%n];
            nxt[rout[i]]=u;
            pre[rout[i]]=v;
            pos[rout[i]]=i;
            adj[w]={u,v};
        }

        // find k-opt
        double newTour=-1;
        for(i=0;i<n;i++){
            x1=rout[i];
            x2=nxt[x1];
            use[x1]=use[x2]=true;
            edge(x1,x2,false);
            newTour=LKHbacktracking(rout, X, x1, x2, X.W[x1][x2], 0.0, 2);
            if(newTour>0)return newTour;
            
            edge(x1,x2,true);
            use[x1]=use[x2]=false;
        }
        
        // fail
        return -1;
    }
    
};

void LKH(int rout[],problem &X,int step=100000,int pr=1){
    std::cout<<"*--------------------------------------"<<std::endl;
    Helsagon solver;
    solver.HelsagonNNM(X);
    double res=-1;
    for(int i=1;i<=step;i++){
        res=solver.LKHstep(rout,X);
        if(res<0){
            std::cout<<"Fail to find better tour.\n"<<std::endl;
            std::cout<<"Final Step : "<<i<<"\n"<<"Best tour : "<<loss(rout,X)<<std::endl;
            std::cout<<"---------------------------------------*"<<std::endl;
            return;
        }
        if(i%pr==0){
            std::cout<<"Step "<<i<<"\n"<<"Best tour : "<<res<<std::endl;
            std::cout<<"---------------------------------------"<<std::endl;
        }
    }
    if(res<0)std::cout<<"Fail to find better tour."<<std::endl;
    
    std::cout<<"Best tour : "<<loss(rout,X)<<std::endl;
    std::cout<<"--------------------------------------*"<<std::endl;
}

}

#endif /* LKH_h */
