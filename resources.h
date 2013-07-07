#include <string.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <functional>

extern float  *dist;
extern int total_stations;
extern double *time_array;



class mytm
{
        int tm_hour;
        int tm_min;
        int tm_day;

public:
        mytm()
        {
                tm_hour=0;
                tm_min=0;
                tm_day=0;
        }

        void update(int hour, int min, int day)
        {
                tm_min = min%60;
                hour = hour + min/60;
                tm_hour = hour%24;
                day = day + hour/24;
                tm_day = day;
        }

        void print()
        {
                char prev;
                prev = std::cout.fill('0');
                std::cout.width(2);
                std::cout << tm_hour;
                std::cout << ":";
                std::cout.width(2);
                std::cout << tm_min;
                std::cout.fill(prev);

        }

        void incrby1(bool hour, bool min, bool day)
        {
                if(hour) update(tm_hour + 1, tm_min, tm_day);
                if(min) update(tm_hour, tm_min + 1, tm_day);
                if(day) update(tm_hour, tm_min, tm_day + 1);
        }

        int gethour() {return tm_hour;}
        int getmin() {return tm_min;}
        int getday() {return tm_day;}
};

struct pathstop
{
	int id;
	int train;
	int mustprint;
	mytm arrival;
	mytm departure;

	void print()
	{
		arrival.print(); 
		std::cout << "-->";
		std::cout << id;
		if (train == -1) return;
		std::cout << ">--";
		departure.print();
		std::cout << "----" << train << "----";
	}
};

struct node
{
        int id;
        float weight;
	double time_weight;
        struct node* next;
	int train_id;
	mytm arrival;
	mytm departure;
	
        node()
        {
                id=0;
                weight=0;
		time_weight=0;
                next=NULL;
		train_id = 0;
		memset(&arrival, 0, sizeof(mytm));
		memset(&departure, 0, sizeof(mytm));
        }
        node(const int& x)
        {
                id=x;
                weight=0;
		time_weight=0;
                next=NULL;
		train_id =0;
		memset(&arrival, 0, sizeof(mytm));
		memset(&departure, 0, sizeof(mytm));
        }
};


struct node_less
{
        bool operator () (const node & x, const node & y) const
        {
              return dist[x.id] > dist[y.id];                 //for making it a min-heap. For max-heap use "<" operator.
        }

};

struct node_less_time
{
        bool operator () (const node & x, const node & y) const
        {
              return time_array[x.id] > time_array[y.id];                 //for making it a min-heap. For max-heap use "<" operator.
        }

};


extern double difftime(mytm,mytm);
extern double time_diff(mytm,mytm,mytm,bool);

struct station
{
        mytm *arrival;
        mytm *departure;
        mytm *next_arrival;

	bool *unusable;					//unusable is an array indicating whether a particular train is usable or not.
	double **timecost;				//Reason could be that it has been removed as an edge during Yen's algorithm.
	int sizemore;
        int id;
        int *trains;
        int *next;
	float *cost;
        int total_trains;
	int *sortedindex;					//Sorted index is an array of indices sorted according to train number.
	int *trainsorted;					//That is, if a train number, say 11345 is to come at 5th place if sorted
								//then sortedindex[5] will be equal to index of 11345 in original array trains.

        station(int total=0, int num = 0)
        {
                id=num;
		sizemore=0;
                total_trains=0;
                 next  = (int *)malloc(sizeof(int)*total);
                trains = (int *)malloc(sizeof(int)*total);
                 cost  = (float *)malloc(sizeof(float)*total);
                arrival= (mytm *)malloc(sizeof(mytm)*total);
                departure = (mytm *)malloc(sizeof(mytm)*total);
                unusable  = (bool *)malloc(sizeof(bool)*total);
                next_arrival = (mytm *)malloc(sizeof(mytm)*total);
		int size = sizeof(mytm)*total;
                
		memset(arrival,0,size);
		memset(next_arrival,0,size);
		memset(departure,0,size);
		memset( unusable,0,sizeof(bool)*total);
        }

        void assign_time(mytm arrival_time, mytm departure_time, mytm next_arrival_time)
        {
                arrival[total_trains] = arrival_time;
                departure[total_trains] =departure_time;
                next_arrival[total_trains] = next_arrival_time;
                total_trains++;

		if(total_trains >= total_stations || sizemore==1)
		{
			trains = (int *)realloc(trains,sizeof(int)*total_trains);
			next  = (int *)realloc(next,sizeof(int)*total_trains);
			cost  = (float *)realloc(cost,sizeof(float)*total_trains);
			arrival= (mytm *)realloc(arrival,sizeof(mytm)*total_trains);
			departure = (mytm *)realloc(departure,sizeof(mytm)*total_trains);
			unusable  = (bool *)realloc( unusable,sizeof(bool)*total_trains);
			next_arrival = (mytm *)realloc(next_arrival,sizeof(mytm)*total_trains);
			memset(&arrival[total_trains-1],0,sizeof(mytm));
			memset(&departure[total_trains-1],0,sizeof(mytm));
			memset(&next_arrival[total_trains-1],0,sizeof(mytm));
			memset(&unusable[total_trains],0,sizeof(bool));
			sizemore=1;
		}
        }

	void restore()		//Restore usability of all trains
	{
		memset( unusable,0,sizeof(bool)*total_trains);
	}

	void remove_edge(int train);
	
        void add_train(int train_no,int upcoming)
        {
                trains[total_trains] = train_no;
                next[total_trains] = upcoming;
        }

	void assign_cost(float money)
	{
		if (money < 0)
		{
			std::cout << "You have entered negative Money\n";
			std::cin >> money;
		}
		cost[total_trains-1] = money;
	}
        void print()
        {
                int j;
                for (j=0;j<total_trains;j++)
                        printf("%d\t%d\t\t%02d:%02d\t\t%02d:%02d\t\t%d\t\t%02d:%02d\t%f\n",id,trains[j],arrival[j].gethour(),arrival[j].getmin(),departure[j].gethour(),departure[j].getmin(),next[j],next_arrival[j].gethour(),next_arrival[j].getmin(),cost[j]);
		
        }


	bool compare_train(const int& i, const int& j) { return trains[i] < trains[j] ;}  //Function to sort indices according to train numbers.
	bool compare_cost(const int& i, const int& j) { return cost[i] < cost[j] ;}  //Function to sort indices according to cost.
	bool comp_arr(const int& i, const int& j) { return difftime(next_arrival[i],next_arrival[j]) < 0;}  //According next_arrival
	bool compare_neighbours(const int& i, const int& j) { return next[i] < next[j] ;}  //Function to sort indices according to neighbours.
	bool compare_departure(const int& i, const int& j) {return difftime(departure[i],departure[j]) <0;} //According to departure


	void assign_time_weight(node *origin, mytm starting);

	void assign_time_cost()
	{
		sortedindex = (int *)malloc(sizeof(int)*total_trains);
		trainsorted = (int *)malloc(sizeof(int)*total_trains);
		int i;
		for (i=0;i<total_trains;i++) sortedindex[i] = trainsorted[i] = i;

		std::sort(&sortedindex[0], &sortedindex[total_trains], std::bind(&station::compare_train,this,std::placeholders::_1,std::placeholders::_2));
		std::stable_sort(&sortedindex[0], &sortedindex[total_trains], std::bind(&station::compare_cost,this,std::placeholders::_1,std::placeholders::_2));
		std::stable_sort(&sortedindex[0], &sortedindex[total_trains], std::bind(&station::comp_arr,this,std::placeholders::_1,std::placeholders::_2));
		std::stable_sort(&sortedindex[0], &sortedindex[total_trains], std::bind(&station::compare_departure,this,std::placeholders::_1,std::placeholders::_2));
		std::stable_sort(&sortedindex[0], &sortedindex[total_trains], std::bind(&station::compare_neighbours,this,std::placeholders::_1,std::placeholders::_2));
		std::sort(&trainsorted[0], &trainsorted[total_trains], std::bind(&station::compare_train,this,std::placeholders::_1,std::placeholders::_2));
/*		timecost = (double **)malloc(sizeof(double *)*total_trains);
		for (i=0;i<total_trains;i++) timecost[i] = (double *)malloc(sizeof(double) * total_trains);

		int j,index1,index2;
		for (i=0;i<total_trains;i++)
		{
			index1 = sortedindex[i];
			for(j=0;j<total_trains;j++)
			{
				index2 = sortedindex[j];
				timecost[i][j] = time_diff (arrival[index1], departure[index2], next_arrival[index2], index1 != index2);
			}//timecost[i][j] stores time cost or edge weight to arrive at station by train of index1 and departing from train of index2
		}*/

	}		
};

