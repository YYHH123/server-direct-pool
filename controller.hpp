//
//  controller.hpp
//  partition of servers
//
//  Created by Yinghui Wang on 2018-09-27.
//  Copyright © 2018 Yinghui Wang. All rights reserved.
//

#ifndef controller_hpp
#define controller_hpp

#include <iostream>
#include "serverpool.hpp"
#include <list>
#include <array>
#include <string>
#include <vector>
#define lambda 0.5123
#define meansize 4.8
#define departure 1
#define arrival 0
#define Low 1
#define High 1000000
#define STIME 2000000
#define alpha 1.2
#define server_num 3
using namespace std;
class controller{
public:
    controller(std::string fname);
    ~controller();
    double current_t;
    double min_t;
    int start;
    int id_with_mint;
    double remain_arrival;
    std::string file_name;
    double cutoffs[server_num -1];
    vector<serverpool> m_pools;
    array<int,2> s={2,1};
    int system_state;
public:
    double findMinTime();
    void SITAE_pool(double);
    double getExpRanNum(double);
    void getNewState();
    bool updateState();
    int decreaseTime(double);
    void updateDepTime(double);
    void StoreWaitTime(double, int);
    void filewriter(bool);
    double ComputResult();
    double getPareto(double,double,double);
    
};

#endif /* controller_hpp */
