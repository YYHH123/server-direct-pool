#include "controller.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include "job.h"
#define alpha 1.2
#define server_num 3
using namespace std;
controller::controller(string f_name)
{
	file_name=f_name;
	current_t=0.0;
	system_state= 0;
	start=0;
	remain_arrival=getExpRanNum(lambda);
	min_t=remain_arrival;
	id_with_mintime=-1;
	for(int i=1;i<=server_num-1;i++)
	{
		cutoffs[i-1]=pow((i*pow(High,(1-alpha))+server_num-i)/server_num,1/(1-alpha));
		cout<<cutoffs[i-1]<<endl;
	}
	server s(server_num-1);
	m_server=s;
	for(int j=0; j<2;j++)
	{
		serverpool s(j);
		m_pool.push_back(s);
	}
	IdleServerId=0;
/*	for(int j=0;j<server_num;j++)
	{
		if(j==0)
		{
			flexible[j][0]=pow(((1-p)*pow(cutoffs[0],-alpha)+p*pow(Low,-alpha)),-1/alpha);
			flexible[j][1]=pow(((1-p)*pow(cutoffs[0],-alpha)+p*pow(cutoffs[1],-alpha)),-1/alpha);
			cout<<flexible[j][0]<<' '<<flexible[j][1]<<endl;
			continue;
		}
		else if(j==server_num-2)
		{
			flexible[j][0]=pow(((1-p)*pow(cutoffs[j],-alpha)+p*pow(cutoffs[j-1],-alpha)),-1/alpha);
			flexible[j][1]=pow(((1-p)*pow(cutoffs[j],-alpha)+p*pow(High,-alpha)),-1/alpha);
			cout<<flexible[j][0]<<' '<<flexible[j][1]<<endl;
			break;
		}
		flexible[j][0]=pow(((1-p)*pow(cutoffs[j],-alpha)+p*pow(cutoffs[j-1],-alpha)),-1/alpha);
		flexible[j][1]=pow(((1-p)*pow(cutoffs[j],-alpha)+p*pow(cutoffs[j+1],-alpha)),-1/alpha);
		cout<<flexible[j][0]<<' '<<flexible[j][1]<<endl;

	}*/
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
		/*	vector<server>::iterator it = server_farm.begin();
			for(; it != server_farm.end();it++)
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
void controller::SITAE_pool(double new_time)
{
	job	n_job(new_time,current_t);
	if(new_time<=cutoffs[1])
	{
		switch(IdleServerId)
		{
			case 0:
				m_pool[0].ArrivalTime=current_t;
				m_pool[0].ReProcessingtime=new_time;
				m_pool[0].ChangeStatus(1);
				IdleServerId=1;
				break;
			case 1:
				m_pool[1].ArrivalTime=current_t;
				m_pool[1].ReProcessingtime=new_time;
				m_pool[1].ChangeStatus(1);
				IdleServerId=3;
				break;
			case 2:
				m_pool[0].ArrivalTime=current_t;
				m_pool[0].ReProcessingtime=new_time;
				m_pool[0].ChangeStatus(1);
				IdleServerId=3;
				break;
			case 3:
				single_queue.push_back(n_job);
				break;
			default:
				cout<<"smth wrong with idleid"<<endl;			
		}
		
	}
	else
	{
	
		m_server.m_queue.push_back(n_job);
	}
}
int controller::decreaseTime(double elapse)
{
	for(int i = 0; i<2; i++)
		{
			if(m_pool[i].Status==1)
				m_pool[i].ReProcessingtime-=elapse;
		}
	if(!m_server.m_queue.empty())
		m_server.m_queue.front().remain_t-=elapse;
	return 0;
}
void controller::updateDepTime(double mtime)
{
	if(system_state == departure)
	{
		switch(id_with_mintime)
		{
			case 0:
				StoreWaitTime(current_t,0);
				if(single_queue.empty())
				{
					m_pool[0].ReProcessingtime=4294967296;
					m_pool[0].ChangeStatus(0);
					IdleServerId-=1;
				}
				else
				{
					m_pool[0].ArrivalTime=single_queue.front().arrive_t;
					m_pool[0].ReProcessingtime=single_queue.front().remain_t;
					m_pool[0].ChangeStatus(1);
					single_queue.pop_front();
				}
				break;
			case 1:
				StoreWaitTime(current_t,1);
				if(single_queue.empty())
				{
					m_pool[1].ReProcessingtime=4294967296;
					m_pool[1].ChangeStatus(0);
					IdleServerId-=2;
				}
				else
				{
					m_pool[1].ArrivalTime=single_queue.front().arrive_t;
					m_pool[1].ReProcessingtime=single_queue.front().remain_t;
					m_pool[1].ChangeStatus(1);
					single_queue.pop_front();
				}
				break;
			case 2:
				StoreWaitTime(current_t,2);
				m_server.m_queue.pop_front();

		}

	}
	/*	{
			if(id_with_mintime.size()>1)
				{
				int rannum = rand()%(id_with_mintime.size());
				int DepNo = id_with_mintime[rannum];
				StoreWaitTime(current_t,DepNo);
				decreaseTime(mtime);
				server_farm[DepNo].m_queue.pop_front();
				
				}
			else if(id_with_mintime.size() == 1)
				{
				int tmp = id_with_mintime[0];
				StoreWaitTime(current_t, tmp);
				decreaseTime(mtime);
				server_farm[tmp].m_queue.pop_front();
				}		
		}	*/
}
double controller::getExpRanNum(double rate)
{
	double UnivRanNum, ExpRanNum;
Here:	UnivRanNum = rand()/(double)(RAND_MAX);
	if(UnivRanNum == 1)
		goto Here;
	ExpRanNum = -1.0*(log(1-UnivRanNum))*(1.0/rate);
//	cout<<"ExpRanNum"<<ExpRanNum<<endl;
	return ExpRanNum;
}
double controller::findMinTime()
{
/*	top_time.clear();
	id_with_mintime.clear();
	double min = remain_arrival;
	top tmp = make_pair(min, -1);
	top_time.push_back(tmp);
	for(int i =0; i < server_num; i++)
		{
		if(!server_farm[i].m_queue.empty())
			{
				tmp = make_pair(server_farm[i].m_queue.front().remain_t, server_farm[i].m_id);
				top_time.push_back(tmp);
			}
			
		}
	top_time.sort(compt());
	double mintime = top_time.front().first;
	list<top>::iterator it = top_time.begin();
	while(((*it).first == mintime)&&(it!= top_time.end()))
		{
			id_with_mintime.push_back((*it).second);
			it++;
		}*/
	double mintime=remain_arrival;
	id_with_mintime=-1;
	for(int i=0; i<2;i++)
	{
		if(m_pool[i].ReProcessingtime<mintime)
		{
			mintime=m_pool[i].ReProcessingtime;
			id_with_mintime=i;
		}

	}
	if(!m_server.m_queue.empty()&&(m_server.m_queue.front().remain_t<mintime))
		{
			mintime=m_server.m_queue.front().remain_t;
			id_with_mintime=2;
		}
	return mintime;
}

void controller::filewriter(bool begin)
{
	fstream file;
	file.clear();
	if(begin)
		{
			file.open(file_name.c_str(),fstream::out|fstream::trunc);
			file<<"Time:"<<current_t<<' '<<"state:"<<system_state<<' '<<"RemainAv:"<<remain_arrival<<endl;
		}
	else 
		{
			file.open(file_name.c_str(), fstream::out|fstream::app);
			file<<"Time:"<<current_t<<' '<<"state:"<<system_state<<' '<<"RemainAv:"<<remain_arrival<<endl;
		  /*	for(int i = 0; i<server_num; i++)
			{
				if(!server_farm[i].m_queue.empty())
					{
					file<<"Pro"<<i<<": ";
					list<job>::iterator it=server_farm[i].m_queue.begin();
					for(;it!=server_farm[i].m_queue.end();it++)
						file<<(*it).remain_t<<' ';
					file<<endl;
					}
			}*/
			for(int i=0;i<2;i++)
			{
				if(m_pool[i].Status==1)
				{
					file<<"pro"<<i<<": "<<m_pool[i].ReProcessingtime<<endl;
				}
			}
			if(IdleServerId==3 && !single_queue.empty())
			{
				file<<"singlequeue: ";
				list<job>::iterator it= single_queue.begin();
				for(;it!=single_queue.end();it++)
					file<<(*it).remain_t<<' ';
			}
			file<<endl;
			if(!m_server.m_queue.empty())
			{
				file<<"outqueue: ";
				list<job>::iterator it= m_server.m_queue.begin();
				for(;it!=m_server.m_queue.end();it++)
					file<<(*it).remain_t<<' ';
				file<<endl;
			}
/*			vector<double>::iterator it = WaitTimelist.begin();
			for(;it !=WaitTimelist.end();it++)
			{
				file<<*it<<' ';
			}
			file<<endl;*/
		}
	file.close();
}
void controller::getNewState()
{
	min_t = findMinTime();
//	vector <int>::iterator it = id_with_mintime.begin();
	/*for(;it!=IdWithMinTime.end();it++)
		{
		cout<<*it<<' ';
		}
	cout<<endl;
	cout<<"currentmin:"<<currentmintime<<endl;*/
/*	if((id_with_mintime.size() == 1)&&(id_with_mintime[0] == -1))
		system_state = arrival;
	else if(id_with_mintime[0] != -1)
		system_state = departure; */
//	cout<<"systemstate:"<<systemstate<<endl;
	if(id_with_mintime==-1)
		system_state=arrival;
	else if(id_with_mintime !=-1)
		system_state=departure;
}
void controller::StoreWaitTime(double ctime, int de)
{
	double waittime = 0.0;
	if(de<2)
	{
       waittime =ctime -m_pool[de].ArrivalTime;
	}
	else
		waittime=ctime-m_server.m_queue.front().arrive_t;
	//cout<<waittime<<endl;
	WaitTimelist.push_back(waittime);
}

double controller::ComputResult()
{
	double totalwaitingtime= 0.0;
	vector<double>::iterator it = WaitTimelist.begin();
	for(;it!= WaitTimelist.end();it++)
		{
			totalwaitingtime += *it;
		}
	double meanwaitingtime = totalwaitingtime/WaitTimelist.size();
	return meanwaitingtime;
}

double controller::getPareto(double L, double H, double alfa)
{
	double UnivRanNum, ParetoRanNum;
	UnivRanNum = rand()/(double)(RAND_MAX);
	//double para = pow(L/H, alfa);
	//ParetoRanNum = exp((-log(1-UnivRanNum+UnivRanNum*para)+alfa*log(L))/alfa);
	double temp = -(UnivRanNum*pow(H,alfa)-UnivRanNum*pow(L,alfa)-pow(H,alfa))/pow(H*L, alfa);
	ParetoRanNum = pow(temp,-1/alfa);
	return ParetoRanNum;
}
