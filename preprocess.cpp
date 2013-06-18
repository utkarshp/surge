#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<time.h>
#include "resources.h"
#define TIMEPASS 0


float *dist;
int total_stations;
int *reachable;
int **adjmatrix;			//Adjacency matrix representation of the graph
extern void print_graph(struct node *, const int &);
extern station *main_railway();
extern void create_edge(node *, const int &, const int &, const float &);
extern void dijkstra(struct node *list,int n, int source, int target, int *prev, float *dist);

using namespace std;


int dfs(node graph[], int source, int dest, int visited[])
{
//printf("Dfs: %d to %d\n",source,dest);
	if(source == dest) return reachable[dest] = 1;
	visited[source] = 1;
	node *neighbour=graph[source].next;

	for(;neighbour!=NULL; neighbour = neighbour->next)	
		if (! visited[neighbour->id])
			if(dfs(graph,neighbour->id,dest,visited) == 1) reachable[source] = 1;

	return reachable[source];

}

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
			for(j=0;j<n;j++)
				reach[i][j] = reach[i][j] || reach[i][k] && reach[k][j];

	for (i=0;i<n;i++)
		reach[i][i] = -1;					//No self edge. but to distinguish from unreachability.

}

double time_diff(tm current, tm begin, tm end)
{
	double difference1 = difftime(mktime(&begin),mktime(&current));
	if(difference1 < TIMEPASS)
	{
		begin.tm_mday++;
		end.tm_mday++;
		difference1 = difftime(mktime(&begin),mktime(&current));
	}
	double difference2 = difftime(mktime(&end),mktime(&begin));
	if(difference2 < 0)
	{
		end.tm_mday++;
		difference2 = difftime(mktime(&end),mktime(&begin));
	}

	return difference1+difference2;
	
}

main()
{
	int source, dest;
	tm departure;

	station *all_stations = main_railway();




	struct node *all_nodes;
	all_nodes = (node *)malloc(sizeof(node)*total_stations);

	int i,j;
	float weight;
	for (i=0;i<total_stations;i++)
		all_nodes[i] = node(i);

	for (i=0;i<total_stations;i++)
		for (j=0;j<all_stations[i].total_trains;j++)
			create_edge(all_nodes,i,all_stations[i].next[j],0);

	reachable = (int *)malloc(sizeof(int)*total_stations);
	int *visited= (int *)malloc(sizeof(int)*total_stations);

	print_graph(all_nodes,total_stations);

//	for (i=0;i<total_stations;i++)
//		printf("%d --- %d %d\n",i,visited[i],reachable[i]);
	printf("Please Enter source station ");
	scanf("%d",&source);
	printf("Please Enter Destination station id ");
	scanf("%d",&dest);

	dfs(all_nodes,source,dest,visited);

//	for (i=0;i<total_stations;i++)
//		printf("%d --- %d %d\n",i,visited[i],reachable[i]);

//	print_graph(all_nodes,total_stations);
	int k=0,neighbour;
	int total_reachable=0;
	for (i=0;i<total_stations;i++)
		if(reachable[i]) k=printf("%d ",i), total_reachable++;
	if(k==0)
	{
		 printf("No possible route.\n");
		 return 0;
	}

	printf("\n%d\n",total_reachable);
	node *all_reachable = (node *)malloc(sizeof(node)*total_stations);
       for (i=0;i<total_stations;i++)
                all_reachable[i] = node(i);
	int choice;
	printf("For minimising cost, enter 1.\n");
	scanf("%d",&choice);
	if(choice == 1)
	{
		for (i=0;i<total_stations;i++)
			if (!reachable[i]) continue;
			else
				for (j=0;j<all_stations[i].total_trains;j++)
				{
					neighbour = all_stations[i].next[j];
					create_edge(all_reachable,i,neighbour,all_stations[i].cost[j]);
				}
		int *prev = (int *)malloc(sizeof(int)*total_stations);
		dist = (float *)malloc(sizeof(float)*total_stations);

		print_graph(all_reachable,total_stations);

		dijkstra(all_reachable,total_stations,source,dest,prev,dist);
		return 1;
	}
	else return 0;
	printf("Please Enter Time of departure (hh:mm) ");
	scanf("%d:%d",&departure.tm_hour,&departure.tm_min);
	printf("You have entered: %d To %d  at ",source,dest);
	printf("%d:%d O'clock\n",departure.tm_hour,departure.tm_min);
}
