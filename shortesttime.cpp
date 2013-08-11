#include <iostream>
#include <functional>
#include "resources.h"
#include <boost/heap/fibonacci_heap.hpp>
#include <vector>
#include <algorithm>

double *time_array;
extern float *dist;
extern double time_diff(mytm, mytm, mytm, bool);
extern mytm* arrive;
extern int *previous;
extern int *trains;
extern mytm* depart;
extern void create_edge(node *, const int &, const int &, const float &, const int &);
extern void dijkstra(struct node *list,int n, int source, int target);
extern int **reach;
using namespace boost::heap;

typedef fibonacci_heap <node, compare<node_less_time> > FibonacciHeap;
typedef fibonacci_heap <node, compare<node_less_time> > :: handle_type HeapHandle;
typedef std::vector<pathstop> Vector;
typedef std::vector<pathstop>::iterator Iterator;

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


mytm time_dijkstra(station* all_stations,node *list, int n, int source, int dest, mytm starting, float initdist=0, double inittime=0)
{
        int i;
        float new_dist;
	double new_time;
        struct node current,*neighbour,temp;
        HeapHandle *handle;

        handle = (HeapHandle *)malloc(sizeof(HeapHandle)*n);

        for (i=0;i<n;i++)
                time_array[i]=INFINITY;

        dist[source]=initdist;
	time_array[source]=inittime;

	all_stations[source].assign_time_weight(&list[source], starting);
	arrive [source] = starting;

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
                                previous[neighbour->id] = current.id;
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


Vector makepath(int source, int dest)
{
	int i;
	Vector path;
	Iterator it;
	pathstop temp;
	it = path.begin();
	temp.id = dest;
	temp.cost = dist[dest];
	temp.arrival = arrive[dest];
	temp.timecost = time_array[dest];
	temp.train = -1;
	path.insert(it,temp);
	for (i=dest; i!=source; i=previous[i])
	{ 
		temp.id = previous[i];
		temp.train = trains[i];
		temp.arrival = arrive[previous[i]];
		temp.departure = depart[i];
		temp.cost = dist[previous[i]];
		temp.timecost = time_array[temp.id];
		it = path.begin();
		path.insert(it, temp);
	}

	return path;
}

Vector journeyplan(station *all_stations, int total_stations, int source, int dest, mytm starting, int choice, float initdist=0, double inittime=0)
{
	int i,j;
	
	mytm midtime;
	Vector answer;
	
	int k=0,neighbour;
	int total_reachable=0;
	int *reachable = (int *)malloc(sizeof(int)*total_stations);
	if(choice/2==0)	printf ("\n");
	for (i=0;i<total_stations;i++)
		if (reach[source][i] == 1 && reach[i][dest] == 1) reachable[i] = 1;
		else reachable[i]=0;
	for (i=0;i<total_stations;i++)
		if(reachable[i])
		{
			if (choice == 1 || choice ==2)
				 k=printf("%d ",i);
			 total_reachable++;
		}
	if (choice == 0 || choice == 2) std::cout << "\n";
	if(total_reachable==0)
	{
		printf("No possible route.\n");
		free(reachable);
		return answer;
	}

	node *all_reachable = (node *)malloc(sizeof(node)*total_stations);
	for (i=0;i<total_stations;i++)
		all_reachable[i] = node(i);
	for (i=0;i<total_stations;i++)
		if (!reachable[i]) continue;
		else
			for (j=0;j<all_stations[i].total_trains;j++)
			{
				neighbour = all_stations[i].next[j];
				if (!reachable[neighbour]) continue;
				create_edge(all_reachable,i,neighbour,all_stations[i].cost[j],all_stations[i].trains[j]);
			}
	int middle;

	if (choice == 3 || choice == 4) middle = -1;
	do
	{
		if(middle == -1)
			if (choice == 2 || choice == 4)
			{
				midtime = time_dijkstra(all_stations, all_reachable, total_stations, source, dest, starting,initdist,inittime);
				answer = makepath(source, dest);
				free(reachable);
				return answer;
			}

			else if (choice == 1 || choice == 3)
			{
				dijkstra(all_reachable,total_stations,source,dest);
				answer = makepath(source,dest);
				free(reachable);
				return answer;
			}
		std::cout << "\n\nEnter a node which you would like to visit from above in the path from " << source << " to " << dest << "\n";
		std::cout << "Or enter -1 to consider all possibilities\n";
		std::cin >> middle;
	}
	while (!reachable[middle]);

	answer = journeyplan(all_stations, total_stations, source, middle, starting, choice);

	for (i=0;i<total_stations;i++)
		previous[i] = 0;

	Vector another = journeyplan(all_stations, total_stations, middle, dest, (answer.end()-1)->arrival, choice);
	
	answer.erase(answer.end());
	another.begin()->mustprint = 1;
	answer.insert(answer.end(),another.begin(),another.end());
	free(reachable);
	return answer;
}
