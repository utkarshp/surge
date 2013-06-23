%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
First compile the lex file using flex <file>.lex then compile the c code using 
gcc lex.yy.c -o output.out. run the code using the command
output.out < file.html to parse file.html

*/
int current;

char **stations;
%}

capital [A-Z]

%s EXTRACT

%%

<INITIAL>"class=train-name>Place</th></tr>" 	{printf("Begun Extracting\n"); BEGIN EXTRACT;}
<EXTRACT>{capital}+/"</td>\n<td>"				{strcpy(stations[current],yytext); printf("%s\n",stations[current]); current++;}
.|\n ; 					
%%

int main ()
{
	int total=427;						//Total set to 427 for UP stations
//	printf("Number of Stations? ");
//	scanf("%d",total);

	stations = (char **)malloc(sizeof(char *)*total);
	int i;
	for (i=0;i<total;i++)
		stations[i] = (char *)malloc(sizeof(char)*5);
	current = 0;
	yylex();

	char filename[] = "station_codes.txt";			//File name For output File
//	printf("Please enter the filename\n");
//	scanf("%s",filename);
		
	FILE *fp = fopen(filename,"w");
	for (i=0;i<total;i++)
		fprintf(fp,"%d\t%s\n",i,stations[i]);
	fclose(fp);
}
