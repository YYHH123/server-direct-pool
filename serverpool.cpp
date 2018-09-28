//
//  serverpool.cpp
//  partition of servers
//
//  Created by Yinghui Wang on 2018-09-27.
//  Copyright © 2018 Yinghui Wang. All rights reserved.
//

#include "serverpool.hpp"
serverpool::serverpool(int id, int servernumber)
{
    name = id;
    size = servernumber;
    status = 0;
    ReProcessingtime = 4294967296;
    ArrivalTime = 0.0;
    for(int i=0; i< servernumber;i++)
        idleServer.push_back(i);
}
serverpool::~serverpool()
{
}
void serverpool::ChangeStatus(int state)
{
    
    status = state;
}
