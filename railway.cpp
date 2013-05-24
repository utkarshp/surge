#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#define NAMELENGTH 20
#define NUMBERLENGTH 8
#define TEMP all_stations[current]

using namespace std;

struct station
{
private:
	tm *arrival;
	tm *departure;
	int total_trains;
	int id;
	int *trains;
	int *next;

public:

	station(int total=0, int num = 0)
	{
		id=num;
		total_trains=0;
		trains = (int *)malloc(sizeof(int)*total);
		 next  = (int *)malloc(sizeof(int)*total);
		arrival= (tm *)malloc(sizeof(tm)*total);
		departure = (tm *)malloc(sizeof(tm)*total);
	}

	void assign_time(tm arrival_time, tm departure_time)
	{
		arrival[total_trains] = arrival_time;
		departure[total_trains] =departure_time;
		total_trains++;
	}

	void add_train(int train_no,int upcoming)
	{
		trains[total_trains] = train_no;
		next[total_trains] = upcoming;
	}

	void print()
	{
		int j;
		for (j=0;j<total_trains;j++)
			printf("%d\t%d\t\t%d:%d\t\t%d:%d\t\t%d\n",id,trains[j],arrival[j].tm_hour,arrival[j].tm_min,departure[j].tm_hour,departure[j].tm_min,next[j]);	
	}
};

int main()
{
	FILE *fp,*railway;
	fp = fopen("railway_id","r");
	int total_trains=0;	
	int total_stations;
	int j;
	char c;

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

	int current,next;
	tm arrive, depart;

	printf("Enter the total no. of stations ");
	scanf("%d",&total_stations);	

	station all_stations[total_stations];
	for (i=0;i<total_stations;i++)
		all_stations[i] = station(total_trains,i);

	int stations;
	fp = fopen("railway_id","r");
	i=0;
	int space;
	while(1)
	{
	//	if(feof(fp)) break;
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
		
//		if (!feof(fp) && j<0) continue;
		railway = fopen(train_num[i],"r");
		fscanf(railway,"%d",&stations);
		fscanf(railway,"%d",&current);
		for (j=1;j<stations;j++)
		{
			fscanf(railway,"%d:%d %d:%d\n%d",&arrive.tm_hour,&arrive.tm_min,&depart.tm_hour,&depart.tm_min,&next);
			all_stations[current].add_train(atoi(train_num[i]),next);
			TEMP.assign_time(arrive,depart);

			current = next;
		}
		fscanf(railway,"%d:%d %d:%d",&arrive.tm_hour,&arrive.tm_min,&depart.tm_hour,&depart.tm_min);
//printf("Outof while\n");
                all_stations[current].add_train(atoi(train_num[i]),next);
                TEMP.assign_time(arrive,depart);

		fclose(railway);
//printf("Closed\n");
		i++;
//printf("Incremented\n");
	}	
//printf("Outta bigger while\n");	
	fclose(fp);

	printf("Station\tTrain No.\tArrival\t\tDeparture Next Station\n");
	for (i=0;i<total_stations;i++)
	{
		printf("----------------------------%d----------------------------\n",i);
		all_stations[i].print();
	}
}
