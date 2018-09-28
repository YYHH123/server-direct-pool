//
//  job.hpp
//  partition of servers
//
//  Created by Yinghui Wang on 2018-09-27.
//  Copyright © 2018 Yinghui Wang. All rights reserved.
//

#ifndef job_hpp
#define job_hpp

#include <stdio.h>

class job
{
public:
    double processing_t;
    double arrive_t;
    double remain_t;
public:
    job(double,double);
    virtual ~job();
};
#endif

