//
//  job.cpp
//  partition of servers
//
//  Created by Yinghui Wang on 2018-09-27.
//  Copyright © 2018 Yinghui Wang. All rights reserved.
//

#include "job.hpp"

job::job(double p_t, double a_t)
{
    processing_t=p_t;
    arrive_t=a_t;
    remain_t=p_t;
}
job::~job()
{
}
