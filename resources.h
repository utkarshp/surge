#include <string.h>
#include <algorithm>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <functional>

extern double time_diff(tm,tm,tm,bool);


struct node
{
        int id;
        float weight;
	float secondary_weight;
        struct node* next;
	int train_id;
	
        node()
        {
                id=0;
                weight=0;
		secondary_weight=0;
                next=NULL;
		train_id = 0;
        }
        node(const int& x)
        {
                id=x;
                weight=0;
		secondary_weight=0;
                next=NULL;
		train_id =0;
        }
};

extern float  *dist;
extern int total_stations;

struct node_less
{
        bool operator () (const node & x, const node & y) const
        {
              return dist[x.id] > dist[y.id];                 //for making it a min-heap. For max-heap use "<" operator.
        }

};


struct station
{
        tm *arrival;
        tm *departure;
        tm *next_arrival;

	double **timecost;		
	int sizemore;
        int id;
        int *trains;
        int *next;
	float *cost;
        int total_trains;
	int *sortedindex;					//Sorted index is an array of indices sorted according to train number.
								//That is, if a train number, say 11345 is to come at 5th place if sorted
								//then sortedindex[5] will be equal to index of 11345 in original array trains.

        station(int total=0, int num = 0)
        {
                id=num;
		sizemore=0;
                total_trains=0;
                trains = (int *)malloc(sizeof(int)*total);
                 next  = (int *)malloc(sizeof(int)*total);
                 cost  = (float *)malloc(sizeof(float)*total);
                arrival= (tm *)malloc(sizeof(tm)*total);
                departure = (tm *)malloc(sizeof(tm)*total);
                next_arrival = (tm *)malloc(sizeof(tm)*total);
		int size = sizeof(tm)*total;
                
		memset(arrival,0,size);
		memset(departure,0,size);
		memset(next_arrival,0,size);
        }

        void assign_time(tm arrival_time, tm departure_time, tm next_arrival_time)
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
			arrival= (tm *)realloc(arrival,sizeof(tm)*total_trains);
			departure = (tm *)realloc(departure,sizeof(tm)*total_trains);
			next_arrival = (tm *)realloc(next_arrival,sizeof(tm)*total_trains);
			sizemore=1;
		}
        }

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
                        printf("%d\t%d\t\t%02d:%02d\t\t%02d:%02d\t\t%d\t\t%02d:%02d\t%f\n",id,trains[j],arrival[j].tm_hour,arrival[j].tm_min,departure[j].tm_hour,departure[j].tm_min,next[j],next_arrival[j].tm_hour,next_arrival[j].tm_min,cost[j]);
		
        }

	bool compare_function(const int& i, const int& j) { return trains[i] < trains[j] ;}  //Function to sort train number indices.

	void assign_time_cost()
	{
		sortedindex = (int *)malloc(sizeof(int)*total_trains);
		int i;
		for (i=0;i<total_trains;i++) sortedindex[i] = i;

		std::sort(&sortedindex[0], &sortedindex[total_trains], std::bind(&station::compare_function,this,std::placeholders::_1,std::placeholders::_2));
		/*for (i=0;i<total_trains;i++)
			std::cout << trains[sortedindex[i]] << "\n";
		std::cout << "enter\n";
		std::cin >> i;*/
		timecost = (double **)malloc(sizeof(double *)*total_trains);
		for (i=0;i<total_trains;i++) timecost[i] = (double *)malloc(sizeof(double) * total_trains);

		int j,index1,index2;
		for (i=0;i<total_trains;i++)
		{
			index1 = sortedindex[i];
			std::cout << i << ":- ";
			for(j=0;j<total_trains;j++)
			{
				index2 = sortedindex[j];
				timecost[i][j] = time_diff (arrival[index1], departure[index2], next_arrival[index2], index1 != index2);
				std::cout << timecost[i][j] << '\t';
			}//timecost[i][j] stores time cost or edge weight to arrive at station by train of index1 and departing from train of index2
			std::cout << '\n';
		}
		std::cout << '\n';

	}		
};

