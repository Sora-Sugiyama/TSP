//
//  weight.h
//  TSP
//
//  Created by Sora Sugiyama on 9/26/24.
//

#ifndef weight_h
#define weight_h

#include "TSP.h"
#include <cmath>
#include <algorithm>

namespace TSP{

double distEUC(std::vector<double>&u,std::vector<double>&v,int dim){
    double ret=0;
    for(int i=0;i<dim;i++)ret+=(u[i]-v[i])*(u[i]-v[i]);
    return sqrt(ret);
}

double distMAN(std::vector<double>&u,std::vector<double>&v,int dim){
    double ret=0;
    for(int i=0;i<dim;i++)ret+=abs(int(u[i])-int(v[i]));
    return ret;
}

double distMAX(std::vector<double>&u,std::vector<double>&v,int dim){
    double ret=0;
    for(int i=0;i<dim;i++)ret=std::max(ret,double(abs(int(u[i])-int(v[i]))));
    return ret;
}

const double PI=3.1415926535897932384626433832795028842;
const double RRR=6378.388;

double distGEO(std::vector<double>&u,std::vector<double>&v){
    double lat1,lng1,lat2,lng2;
    lat1=PI*(int(u[0])+5.0*(u[0]-int(u[0]))/3.0)*180;
    lng1=PI*(int(u[1])+5.0*(u[1]-int(u[1]))/3.0)*180;
    lat2=PI*(int(v[0])+5.0*(v[0]-int(v[0]))/3.0)*180;
    lng2=PI*(int(v[1])+5.0*(v[1]-int(v[1]))/3.0)*180;
    
    double q1=std::cos(lng1-lng2),q2=std::cos(lat1-lat2),q3=std::cos(lat1+lat2);
    return RRR*acos(0.5*((1.0+q1)*q2-(1.0-q1)*q3))+1.0;
}

double distATT(std::vector<double>&u,std::vector<double>&v){
    const double xd=u[0]-v[0],yd=u[1]-v[1];
    double r=sqrt((xd*xd+yd*yd)/10.0);
    double t=int(r);
    if(t<r)r=r+1.0;
    return r;
}

void makeWeightMatrix(problem &X){
    int i,j;
    for(i=0;i<X.dimension;i++){
        for(j=0;j<X.dimension;j++){
            if(X.WeightType=="EUC_2D")X.W[i][j]=distEUC(X.coord[i],X.coord[j],2);
            else if(X.WeightType=="EUC_3D")X.W[i][j]=distEUC(X.coord[i],X.coord[j],3);
            else if(X.WeightType=="MAX_2D")X.W[i][j]=distMAX(X.coord[i],X.coord[j],2);
            else if(X.WeightType=="MAX_3D")X.W[i][j]=distMAX(X.coord[i],X.coord[j],3);
            else if(X.WeightType=="MAN_2D")X.W[i][j]=distEUC(X.coord[i],X.coord[j],2);
            else if(X.WeightType=="MAN_3D")X.W[i][j]=distEUC(X.coord[i],X.coord[j],3);
            else if(X.WeightType=="CEIL_2D")X.W[i][j]=std::round(distEUC(X.coord[i],X.coord[j],2));
            else if(X.WeightType=="GEO")X.W[i][j]=distGEO(X.coord[i],X.coord[j]);
            else if(X.WeightType=="ATT")X.W[i][j]=distATT(X.coord[i],X.coord[j]);
        }
    }
}

}

#endif /* weight_h */
