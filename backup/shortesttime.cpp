#include <iostream>
#include <functional>
#include "resources.h"
#include <boost/heap/fibonacci_heap.hpp>
#include <algorithm>

double *time_array;
extern float *dist;
extern double time_diff(mytm, mytm, mytm, bool);
extern mytm* arrive;
extern mytm* depart;

using namespace boost::heap;

typedef fibonacci_heap <node, compare<node_less_time> > FibonacciHeap;
typedef fibonacci_heap <node, compare<node_less_time> > :: handle_type HeapHandle;

double difftime(mytm end, mytm begin)
{
        double difference=0;
        difference = (end.getday() - begin.getday()) * 24;
        difference += end.gethour() - begin.gethour();
        difference += (end.getmin() - begin.getmin())/60.0;

        return difference;
}


void station::assign_time_weight(node *origin, mytm starting)
{
	node *temp = origin->next;
	std::pair <int *,int *> bounds;
	int *lower,newindex, findlower=1;
	while (temp != NULL)
	{

		findlower = 1;
		for (lower = &sortedindex[0]; lower < &sortedindex[total_trains] ; lower++)
			if (findlower)
			{
				if (next[*lower] == temp->id)
				{
					bounds.first = lower;
					findlower = 0;
				}
			}
			else if(next[*lower] > temp->id)
			{
				bounds.second = lower;
				break;
			}

		if (lower == &sortedindex[total_trains]) bounds.second = lower;

		double diff = time_diff(starting, departure[*bounds.first], next_arrival[*bounds.first],0);
		double currdiff = diff;
		newindex = *bounds.first;
		for(lower = bounds.first; lower < bounds.second; lower++)
		{
			currdiff = time_diff(starting, departure[*lower], next_arrival[*lower],0);
			if (currdiff<= diff && !unusable[*lower])
			{
				diff = currdiff;
				newindex = *lower;
			}
		}
			

		temp->train_id = trains[newindex];
		temp->weight = cost[newindex];
		temp->time_weight = time_diff(starting, departure[newindex], next_arrival[newindex], 0); //Not considering train change time.
		temp->arrival = next_arrival[newindex];
		temp->departure = departure[newindex];	
	
		temp = temp->next;
	}		
}


mytm time_dijkstra(station* all_stations,node *list, int n, int source, int dest, mytm starting, int *prev, int *trains)
{
        int i;
        float new_dist;
	double new_time;
        struct node current,*neighbour,temp;
        HeapHandle *handle;

        handle = (HeapHandle *)malloc(sizeof(HeapHandle)*n);

        for (i=0;i<n;i++)
                time_array[i]=INFINITY;

        dist[source]=0;
	time_array[source]=0;

	all_stations[source].assign_time_weight(&list[source], starting);

        FibonacciHeap Q;
        for (i=0;i<n;i++)
                handle[i]=Q.push(list[i]);

        while (!Q.empty())
        {
                current=Q.top();
                Q.pop();
                if( time_array[current.id] == INFINITY || current.id == dest) break;
                neighbour = current.next ;
                while (neighbour!=NULL)
                {
			if(neighbour->id == current.id) continue;
                        new_dist = dist[current.id] + neighbour->weight;
			new_time = time_array[current.id] + neighbour->time_weight;
                        if (new_time < time_array[neighbour->id])
                        {
                                dist[neighbour->id] = new_dist;
				time_array[neighbour->id] = new_time;
                                prev[neighbour->id] = current.id;
                                trains[neighbour->id] = neighbour->train_id;
				all_stations[neighbour->id].assign_time_weight(&list[neighbour->id], neighbour->arrival);
				arrive[neighbour->id] = neighbour->arrival;
				depart[neighbour->id] = neighbour->departure;
                                Q.update(handle[neighbour->id]);
                        }
			neighbour = neighbour->next;
		}
	}

	return arrive[dest];

}

void station::remove_edge(int train)
{
	if (sizeof(trains)/sizeof(int) == total_trains)
		 trains  = (int *)realloc(trains,sizeof(int)*(total_trains+1));

	trains[total_trains] = train;
	
	std::pair <int*, int*> bounds;

	bounds = std::equal_range(&trainsorted[0], &trainsorted[total_trains], total_trains, std::bind(&station::compare_train,this,std::placeholders::_1,std::placeholders::_2));

	unusable[*bounds.first] = 1;

}
