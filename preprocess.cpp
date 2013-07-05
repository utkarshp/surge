#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include "resources.h"
#define TIMEPASS 0


float *dist;
int total_stations;
int *reachable;
int **adjmatrix;			//Adjacency matrix representation of the graph
extern void print_graph(struct node *, const int &);
extern station *main_railway();
extern void create_edge(node *, const int &, const int &, const float &);
extern void create_edge(node *, const int &, const int &, const float &, const int &);
extern void time_dijkstra(station* all_stations,node *list, int n, int source, int dest, mytm starting, int *prev, int *trains);
extern void dijkstra(struct node *list,int n, int source, int target, int *prev, int *dist);
mytm *arrive;
mytm *depart;
extern double *time_array;
extern double difftime(mytm,mytm);
using namespace std;


int **makematrix(node graph[], int n)		// function to make an adjacency matrix like data structure for reachability
{

	int **reach;
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
	return reach;
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




	struct node *all_nodes;
	all_nodes = (node *)malloc(sizeof(node)*total_stations);

	int i,j, **reach;
	float weight;
	for (i=0;i<total_stations;i++)
		all_nodes[i] = node(i);

	for (i=0;i<total_stations;i++)
		for (j=0;j<all_stations[i].total_trains;j++)
			create_edge(all_nodes,i,all_stations[i].next[j],0);

	reachable = (int *)malloc(sizeof(int)*total_stations);
	int *visited= (int *)malloc(sizeof(int)*total_stations);

	print_graph(all_nodes,total_stations);

	for (i=0;i<total_stations;i++) all_stations[i].assign_time_cost();

	//	for (i=0;i<total_stations;i++)
	//		printf("%d --- %d %d\n",i,visited[i],reachable[i]);
	while (1)
	{
		printf("Please Enter source station ");
		scanf("%d",&source);
		printf("Please Enter Destination station id ");
		scanf("%d",&dest);

		reach = makematrix(all_nodes,total_stations);

		for (i=0;i<total_stations;i++)
			if (reach[source][i] == 1 && reach[i][dest] == 1) reachable[i] = 1;
			else reachable[i]=0;

		int k=0,neighbour;
		int total_reachable=0;
		for (i=0;i<total_stations;i++)
			if(reachable[i]) k=printf("%d ",i), total_reachable++;
		if(k==0)
		{
			printf("No possible route.\n");
			continue;
		}

		printf("\n%d\n",total_reachable);
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

		int choice;
		printf("For minimising cost, enter 1.\nFor minmizing time enter 2. ");
		scanf("%d",&choice);
		if(choice == 1)
		{
			int *prev = (int *)malloc(sizeof(int)*total_stations);
			int *trains = (int *)malloc(sizeof(int)*total_stations);

			dist = (float *)malloc(sizeof(float)*total_stations);

			print_graph(all_reachable,total_stations);
			dijkstra(all_reachable,total_stations,source,dest,prev,trains);

			for (j=0,i=dest;i!=source;i=prev[i],j++)
				if(i == prev[i])
				{
					printf("Not connected\n");
					break;
				}
				else
					printf("%d<--%d--<",i,trains[i]);
			printf("%d\n",source);
			
			free(dist);
			free(trains);
			free(prev);
				continue;
		}
		else if (choice == 2);
		{
        	        memset(&departure, 0, sizeof(mytm));
			int hour, min;
			printf("Please Enter Time of departure (hh:mm) ");
			scanf("%d:%d",&hour,&min);
			printf("You have entered: %d To %d  at ",source,dest);
			printf("%d:%d O'clock\n",hour,min);

			departure.update(hour,min,0);
			
			dist = (float *)malloc(sizeof(float)*total_stations);
			int *prev = (int *)malloc(sizeof(int)*total_stations);
			int *trains = (int *)malloc(sizeof(int)*total_stations);
			depart = (mytm *)malloc(sizeof(mytm)*total_stations);
			arrive = (mytm *)malloc(sizeof(mytm)*total_stations);
			time_array = (double *)malloc(sizeof(double)*total_stations);
			print_graph(all_reachable,total_stations);
			int size = sizeof(mytm)*total_stations;

			memset(arrive,0,size);
			memset(depart,0,size);

			int printflag=1;
			time_dijkstra(all_stations, all_reachable, total_stations, source, dest, departure, prev, trains);
			for (j=0,i=dest;i!=source;i=prev[i],j++)
				if(i == prev[i])
				{
					printf("Not connected\n");
					break;
				}
				else
				{
					if(printflag)
					{
						printf("%d<--",i);
						printf("%02d:%02d--",arrive[i].gethour(),arrive[i].getmin());
					}
					if (trains[i] == trains[prev[i]])
					{
						printflag=0;
						continue;
					}
					printflag=1;
					printf("--%d----%02d:%02d--<",trains[i],depart[i].gethour(), depart[i].getmin());
				}
			printf("%d\n",source);

			free(dist);
			free(prev);	
			free(trains);
			free(arrive);
			free(depart);
		}
	}
}
