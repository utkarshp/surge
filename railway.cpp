#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "resources.h"
#define NAMELENGTH 20
#define NUMBERLENGTH 8
#define TEMP all_stations[current]
#define PATH "./uprail/trains/"

extern int total_stations;

using namespace std;

station *main_railway()
{
	FILE *fp,*railway;
	fp = fopen((string(PATH) + string("railway_id")).c_str(),"r");
	int total_trains=0;	
	int j;
	char c;
	int stations;

	while(!feof(fp))
	{
		fscanf(fp,"%c",&c);
		if(c=='\n') total_trains++;
	}
	total_trains -= 2;
	fclose(fp);

	printf("Total trains = %d\n",total_trains);

	char train_num[total_trains][NUMBERLENGTH];
	int i;
	char train_name[total_trains][NAMELENGTH];

	int current,next,previous;
	mytm arrive, depart, next_arrive;


	memset(&arrive,0,sizeof(mytm));
	memset(&depart,0,sizeof(mytm));
	memset(&next_arrive,0,sizeof(mytm));


	printf("Enter the total no. of stations ");
	scanf("%d",&total_stations);	

	station *all_stations;

	all_stations = (station *)malloc(sizeof(station)*total_stations);
	for (i=0;i<total_stations;i++)
		all_stations[i] = station(total_trains,i);

	fp = fopen((string(PATH)+string("railway_id")).c_str(),"r");
	i=0;
	int space;
	float cost;
	int hour1,hour2,min1,min2;
	while(!feof(fp))
	{
		space = 0;
		j=0;
		fscanf(fp,"%s",train_num[i]);
		fscanf(fp,"%s",train_name[i]);
	/*	if(c=='N') break;
		while(c!='\n' && j < NAMELENGTH)
		{
			if(space == 0) train_num[i][j]=c;
			else train_name[i][j] = c;
			j++;
			fscanf(fp,"%c",&c);
			if(c==' ')
			{
				space=1;
				train_num[i][j] = '\0';
				j=0;
			}
		}
		train_name[i][j] = '\0';*/

		

		railway = fopen((string(PATH) + string(train_num[i])).c_str(),"r");
		if (railway == NULL) break;
		stations=0;
		while(!feof(railway))
		{
			fscanf(railway,"%c",&c);
			if(c=='\n') stations++;
		}
		stations -= 2;
		fclose(railway);
		railway = fopen((string(PATH) + string(train_num[i])).c_str(),"r");
		//	printf("Stations for train %d = %d\n",i,stations);
		fscanf(railway,"%d %d:%d",&current,&hour1,&min1);
		arrive.update(hour1,min1,0);
		for (j=0;j<stations;j++)
		{
			fscanf(railway,"%d:%d %f\n%d %d:%d",&hour1,&min1,&cost,&next,&hour2,&min2);
			depart.update(hour1,min1,0);
			next_arrive.update(hour2,min2,0);
			all_stations[current].add_train(atoi(train_num[i]),next);
			if (j>0) all_stations[previous].assign_cost(cost);
			TEMP.assign_time(arrive,depart,next_arrive);
			previous = current;
			current = next;
			arrive = next_arrive;
		}
		fscanf(railway,"%d:%d %f",&hour1,&min1,&cost);
		depart.update(hour1,min1,0);
		all_stations[current].add_train(atoi(train_num[i]),next);
		TEMP.assign_time(arrive,depart,next_arrive);
		if (j>0) all_stations[previous].assign_cost(cost);

		fclose(railway);
		i++;
	}	
	fclose(fp);

	printf("Station\tTrain No.\tArrival\t\tDeparture, Next Station \tArrival\tCost\n");
	for (i=0;i<total_stations;i++)
	{
		printf("---------------------------------------------%d---------------------------------------------\n",i);
		all_stations[i].print();
	}

	return all_stations;
}

/*
int main()
{
	struct station *all_stations=main_railway();
	*	int i;
		for (i=0;i<total_stations;i++)
		{
		printf("---------------------------------%d---------------------------------\n",i);
		all_stations[i].print();
		}
	return 0;
}*/
