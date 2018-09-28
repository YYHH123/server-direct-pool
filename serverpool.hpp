//
//  serverpool.hpp
//  partition of servers
//
//  Created by Yinghui Wang on 2018-09-27.
//  Copyright © 2018 Yinghui Wang. All rights reserved.
//

#ifndef serverpool_hpp
#define serverpool_hpp

#include <list>
#include <vector>
#include <iostream>
#include "job.hpp"

using namespace std;

class serverpool
{
public:
    serverpool(int,int);
    virtual ~serverpool();
    double ReProcessingtime;
    int name;
    int size;
    int status;
    double ArrivalTime;
    list<job> single_queue;
    vector<int> idleServer;
public:
    void ChangeStatus(int);
};
#endif /* serverpool_hpp */
