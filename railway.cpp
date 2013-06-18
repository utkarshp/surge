#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include "resources.h"
#define NAMELENGTH 20
#define NUMBERLENGTH 8
#define TEMP all_stations[current]

extern int total_stations;

using namespace std;

station *main_railway()
{
	FILE *fp,*railway;
	fp = fopen("railway_id","r");
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
	tm arrive, depart, next_arrive;

	
	memset(&arrive,0,sizeof(tm));
	memset(&depart,0,sizeof(tm));
	memset(&next_arrive,0,sizeof(tm));


	printf("Enter the total no. of stations ");
	scanf("%d",&total_stations);	

	station *all_stations;

	all_stations = (station *)malloc(sizeof(station)*total_stations);
	for (i=0;i<total_stations;i++)
		all_stations[i] = station(total_trains,i);

	fp = fopen("railway_id","r");
	i=0;
	int space;
	float cost;
	while(1)
	{
		space = 0;
		j=0;
		fscanf(fp," %c",&c);
		if(c=='N') break;
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
		train_name[i][j] = '\0';
		
		railway = fopen(train_num[i],"r");
	stations=0;
	while(!feof(railway))
	{
		fscanf(railway,"%c",&c);
		if(c=='\n') stations++;
	}
	stations -= 2;
	fclose(railway);
		railway = fopen(train_num[i],"r");
//	printf("Stations for train %d = %d\n",i,stations);
	fscanf(railway,"%d %d:%d",&current,&arrive.tm_hour,&arrive.tm_min);
		for (j=0;j<stations;j++)
		{
			fscanf(railway,"%d:%d %f\n%d %d:%d",&depart.tm_hour,&depart.tm_min,&cost,&next,&next_arrive.tm_hour,&next_arrive.tm_min);
			all_stations[current].add_train(atoi(train_num[i]),next);
			if (j>0) all_stations[previous].assign_cost(cost);
			TEMP.assign_time(arrive,depart,next_arrive);
			previous = current;
			current = next;
			arrive = next_arrive;
		}
		fscanf(railway,"%d:%d %f",&depart.tm_hour,&depart.tm_min,&cost);
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
*	return 0;
}*/
