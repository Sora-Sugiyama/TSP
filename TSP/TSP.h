//
//  TSP.h
//  TSP
//
//  Created by Sora Sugiyama on 9/26/24.
//

#ifndef TSP_h
#define TSP_h

#include <vector>
#include <random>
#include <algorithm>

namespace TSP{
class problem{
public:
    std::string WeightType;
    int dimension=0;
    std::vector<std::vector<double> >coord;
    std::vector<int>optTour;
    std::vector<std::vector<double> >W;
    
    void clear(){
        coord.clear();
        W.clear();
        optTour.clear();
        dimension=0;
        WeightType="";
    }
};

}

#include "TSPLIBloader.h"
#include "greedy.h"
#include "LKH.h"

#endif /* TSP_h */
