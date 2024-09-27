//
//  disjoint_set.h
//  TSP
//
//  Created by Sora Sugiyama on 9/27/24.
//

#ifndef disjoint_set_h
#define disjoint_set_h

#include <cstddef>
#include <vector>

namespace TSP{

class disjoint_set{
    std::vector<int>E;
    
    int Find(int x){
        int z=x,tmp;
        while(z!=E[z])z=E[z];
        while(x!=E[x]){
            tmp=x;
            x=E[x];
            E[tmp]=z;
        }
        return z;
    }
    
public:
    
    void resize(const int n){
        E.resize(n);
        for(int i=0;i<n;i++)E[i]=i;
    }
    
    void addElement(){
        const int NEW=(int)E.size();
        E.push_back(NEW);
    }
    
    void Union(int x,int y){
        x=Find(x);
        int tmp;
        while(y!=E[y]){
            tmp=y;
            y=E[y];
            E[tmp]=x;
        }
        E[y]=x;
    }
    
    bool Disjoint(int x,int y){
        return Find(x)!=Find(y);
    }
    
    std::size_t size(){
        return E.size();
    }
    
    void clear(){
        E.clear();
    }
};

}

#endif /* disjoint_set_h */
