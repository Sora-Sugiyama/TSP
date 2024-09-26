//
//  TSPLIBloader.h
//  TSP
//
//  Created by Sora Sugiyama on 9/26/24.
//

#ifndef TSPLIBloader_h
#define TSPLIBloader_h

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>

namespace TSP{

class TSPLIBloader{
public:
    std::string WeightType;
    int dimension;
    std::vector<std::pair<double,double> >coord;
    std::vector<int>optTour;
};

std::string PATH="";

void SetPATH(){
    std::cout<<"new path : "<<std::flush;
    std::cin>>PATH;
}

void makeFile(){
    std::system(("cd "+PATH).c_str());
    if(access("ALL_tsp.tar.gz",F_OK)==-1){
        std::cout<<"File path reset required"<<std::endl;
        SetPATH();
    }
    
    std::system("mkdir ALL_tsp");
    std::system("tar -zxvf ALL_tsp.tar.gz -C ALL_tsp");
    std::system("cd ALL_tsp");
    std::system("find . -name \'*.gz\' -exec gzip -d {} \\;");
}

TSPLIBloader ReadFile(const std::string fileName){
    std::ifstream FILE(PATH+"ALL_tsp/"+fileName+".tsp");
    std::ifstream optFile(PATH+"ALL_tsp/"+fileName+".opt.tour");
    TSPLIBloader ret;
    
    if(FILE.fail()||optFile.fail()){
        std::cout<<"File does not exist."<<std::endl;
        return ret;
    }
    
    std::string junk1,junk2;
    getline(FILE,junk1);
    getline(FILE,junk1);
    getline(FILE,junk1);
    
    FILE>>junk1>>ret.dimension;
    FILE>>junk1>>junk2>>ret.WeightType;
    
    getline(FILE,junk1);
    
    ret.coord.resize(ret.dimension);
    for(auto &it:ret.coord){
        FILE>>junk1>>it.first>>it.second;
    }
    FILE.close();
    
    getline(FILE,junk1);
    getline(FILE,junk1);
    getline(FILE,junk1);
    getline(FILE,junk1);
    
    ret.optTour.resize(ret.dimension);
    for(auto &it:ret.optTour){
        optFile>>it;
    }
    optFile.close();
    
    return ret;
}

}

#endif /* TSPLIBloader_h */
