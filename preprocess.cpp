#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include "resources.h"
#define TIMEPASS 0


float *dist;
int total_stations;
int **reach;
int **adjmatrix;			//Adjacency matrix representation of the graph
extern void print_graph(struct node *, const int &);
extern station *main_railway();
extern void create_edge(node *, const int &, const int &, const float &);
extern void create_edge(node *, const int &, const int &, const float &, const int &);
extern void time_dijkstra(station* all_stations,node *list, int n, int source, int dest, mytm starting);
extern void dijkstra(struct node *list,int n, int source, int target);
mytm *arrive;
mytm *depart;
int *previous;
int *trains;
extern double *time_array;
extern double difftime(mytm,mytm);

using namespace std;

typedef std::vector<pathstop> Vector;
typedef std::vector<pathstop>::iterator Iterator;

extern Vector journeyplan( station *, int, int, int, mytm, int, float i=0, double j=0 );
Vector yenksp(station *all_stations, int total_stations, int source, int dest, mytm starting, float cost);
extern void printpath(Vector);


void **makematrix(node graph[], int n)		// function to make an adjacency matrix like data structure for reachability
{

	reach = (int **)malloc(sizeof(int *)*n);
	adjmatrix = (int **)malloc(sizeof(int *)*n);
	int i;
	for (i=0;i<n;i++)
	{
		reach[i] = (int *)malloc(sizeof(int)*n);		//reach[i][j] will be 1 if there is a path from i to j
		adjmatrix[i] = (int *)malloc(sizeof(int)*n);
	}

	int j;
	node *temp;

	for (i=0;i<n;i++)
	{
		temp = graph[i].next;
		reach[i][i] = adjmatrix[i][i] = 1;
		while (temp!=NULL)
		{
			j = temp->id;
			reach[i][j] = 1;
			adjmatrix[i][j] = 1;
			temp = temp->next;				//First creating an adjacency matrix
		}
	}

	int k;
	for (k=0;k<n;k++)						//Floyd-Warshall algorithm for reachability
		for(i=0;i<n;i++)
			if (reach[i][k] == 1)
				for(j=0;j<n;j++)
					reach[i][j] = reach[i][j] || reach[i][k] && reach[k][j];

	for (i=0;i<n;i++)
		reach[i][i] = 1;	
}




double time_diff(mytm current, mytm begin, mytm end, bool trainchanged) //current is time at which one reaches a station begin is at which one departs
{								  //end is time at which one reaches the next station.

	double difference1 = difftime(begin,current);		//difftime is used as difftime(end, beginning);
	if(difference1 < TIMEPASS)
	{
		if (!(difference1 >= 0 && !trainchanged))
		{
			begin.incrby1(0,0,1);
			end.incrby1(0,0,1);
			difference1 = difftime(begin,current);
		}
	}
	double difference2 = difftime(end,begin);
	if(difference2 < 0)
	{
		end.incrby1(0,0,1);
		difference2 = difftime(end,begin);
	}
	return difference1+difference2;
	
}

main()
{
	int source, dest;
	mytm departure;

	station *all_stations = main_railway();

	Vector path;
	Iterator it;

	struct node *all_nodes;
	all_nodes = (node *)malloc(sizeof(node)*total_stations);

	int i,j;
	float weight;
	for (i=0;i<total_stations;i++)
		all_nodes[i] = node(i);

	for (i=0;i<total_stations;i++)
		for (j=0;j<all_stations[i].total_trains;j++)
			create_edge(all_nodes,i,all_stations[i].next[j],0);

	int *visited= (int *)malloc(sizeof(int)*total_stations);

	for (i=0;i<total_stations;i++) all_stations[i].assign_time_cost();

	makematrix(all_nodes,total_stations);
	while (1)
	{
		printf("Please Enter source station ");
		scanf("%d",&source);
		printf("Please Enter Destination station id ");
		scanf("%d",&dest);

		for (i=0;i<total_stations;i++) all_stations[i].restore();

		int choice;
		printf("For minimising cost, enter 1.\nFor minmizing time enter 2.");
		scanf("%d",&choice);
		if(choice == 1)
		{
			previous = (int *)malloc(sizeof(int)*total_stations);
			trains = (int *)malloc(sizeof(int)*total_stations);

			dist = (float *)malloc(sizeof(float)*total_stations);

			journeyplan(all_stations, total_stations, source, dest, departure, choice);
			for (j=0,i=dest;i!=source;i=previous[i],j++)
				if(i == previous[i])
				{
					printf("Not connected\n");
					break;
				}
				else
					printf("%d<--%d--<",i,trains[i]);
			printf("%d\n",source);
			
			free(dist);
			free(trains);
			free(previous);
				continue;
		}
		
		else if (choice == 2)
		{
        	        memset(&departure, 0, sizeof(mytm));
			int hour, min;
			printf("Please Enter Time of departure (hh:mm) ");
			scanf("%d:%d",&hour,&min);
			printf("You have entered: %d To %d  at ",source,dest);
			printf("%d:%d O'clock\n",hour,min);

			departure.update(hour,min,0);
			
			dist = (float *)malloc(sizeof(float)*total_stations);
			previous = (int *)malloc(sizeof(int)*total_stations);
			trains = (int *)malloc(sizeof(int)*total_stations);
			depart = (mytm *)malloc(sizeof(mytm)*total_stations);
			arrive = (mytm *)malloc(sizeof(mytm)*total_stations);
			time_array = (double *)malloc(sizeof(double)*total_stations);
			int size = sizeof(mytm)*total_stations;

			memset(arrive,0,size);
			memset(depart,0,size);

			path = journeyplan(all_stations, total_stations, source, dest, departure, choice);
			printpath(path);

			free(dist);
			free(previous);	
			free(trains);
			free(arrive);
			free(depart);
			continue;
		}
		else if(choice == 3)
		{
                        dist = (float *)malloc(sizeof(float)*total_stations);
                        previous = (int *)malloc(sizeof(int)*total_stations);
                        trains = (int *)malloc(sizeof(int)*total_stations);
                        depart = (mytm *)malloc(sizeof(mytm)*total_stations);
                        arrive = (mytm *)malloc(sizeof(mytm)*total_stations);
                        time_array = (double *)malloc(sizeof(double)*total_stations);
                        int size = sizeof(mytm)*total_stations;

                        memset(arrive,0,size);
                        memset(depart,0,size);
                        memset(&departure, 0, sizeof(mytm));
                        int hour, min;
                        printf("Please Enter Time of departure (hh:mm) ");
                        scanf("%d:%d",&hour,&min);
                        printf("You have entered: %d To %d  at ",source,dest);
                        printf("%d:%d O'clock\n",hour,min);

                        departure.update(hour,min,0);

			int cost;
			std::cout << "Please enter Maximum cost you can afford ";
			std::cin >> cost;
			path = yenksp(all_stations, total_stations, source, dest,departure, cost);

			int printflag = 1;
                        if (path.empty()) continue;
                        path.begin()->print();
                        for (it = path.begin() + 1; it != path.end(); it++)
                                if (it->train != (it-1)->train || it->mustprint)
                                        it->print();
                        std::cout << '\n';
			std::cout << "Total time required = " << (path.back()).timecost << " hours\n";
			std::cout << "Total Cost = " << (path.back()).cost << " Rs\n";

                        free(dist);
                        free(previous);
                        free(trains);
                        free(arrive);
                        free(depart);
			continue;


		}
	}

}
