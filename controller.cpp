//
//  controller.cpp
//  partition of servers
//
//  Created by Yinghui Wang on 2018-09-27.
//  Copyright © 2018 Yinghui Wang. All rights reserved.
//

#include "controller.hpp"
#include <fstream>
#include <algorithm>
#include <math.h>
#include "job.hpp"
using namespace std;
controller::controller(string f_name){
    file_name = f_name;
    current_t = 0.0;
    start =0;
    id_with_mint = -1;
    remain_arrival = getExpRanNum(lambda);
    min_t = remain_arrival;
    for(int i=1; i<=server_num-1; i++)
    {
        cutoffs[i-1] =pow((i*pow(High,(1-alpha))+server_num-i)/server_num,1/(1-alpha));
        cout<<cutoffs[i-1]<<endl;
    }
    for(int j=0; j<s.size();j++){
        serverpool sp(j,s[j]);
        m_pools.push_back(sp);
    }
    
    
}
controller::~controller()
{
}
bool controller::updateState()
{
    
    if(start==0)
    {
        current_t+=min_t;
        remain_arrival=getExpRanNum(lambda);
        double NewProTime=getPareto(Low,High,alpha);
        SITAE_pool(NewProTime);
        start=1;
    }
    else if(start==1)
    {
        
        current_t+=min_t;
        if(current_t>STIME)
        {
            double MeanWaitingTime = ComputResult();
            fstream file;
            file.clear();
            file.open("E:\\project data\\SITAE and JSQ\\mgcpool106.txt",fstream::out|fstream::app);
            file<<MeanWaitingTime<<endl;
            /*    vector<server>::iterator it = server_farm.begin();
             for(; it != server_  farm.end();it++)
             {
             file<<"id: "<<(*it).m_id<<' '<<"qlength: "<<(*it).m_queue.size()<<' ';
             }
             file<<endl;*/
            file.close();
            return true;
        }
        else if(system_state==arrival)
        {
            remain_arrival=getExpRanNum(lambda);
            decreaseTime(min_t);
            double NewProTime=getPareto(Low,High,alpha);
            SITAE_pool(NewProTime);
        }
        else if(system_state==departure)
        {
            remain_arrival-=min_t;
            decreaseTime(min_t);
            updateDepTime(min_t);
        }
    }
    getNewState();
    return false;
    
}

void controller:: SITAE_pool(<#double#> new_time){
    job n_job(new_time, current_t);
    
}
