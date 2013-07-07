%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>

using namespace std;
ofstream train;
int hour, minutes, train_id, issource, arrival, currdist, prevdist;
extern int codetoid(char *);

%}

text [A-Z]
number [0-9]

%s HOUR
%s MIN
%s CODE
%s EXTRACT
%s DISTANCE

%%

<INITIAL>">Day</td></tr>"	{BEGIN EXTRACT;}
<EXTRACT>"<tr><td>"		{arrival = 1; BEGIN CODE;}
<CODE>{text}+			{train_id = codetoid(yytext); if (train_id == -1) BEGIN EXTRACT; else { train << train_id << ' '; BEGIN HOUR;} }
<HOUR>Source			{issource = 1; arrival = 0;}
<HOUR>Destination		{issource = 0; arrival = 0; BEGIN MIN;}
<HOUR>{number}+			{hour = atoi(yytext); BEGIN MIN;}
<MIN>{number}+			{minutes = atoi(yytext);}
<MIN>"pm"			{if (hour != 12) hour = hour + 12;}
<MIN>"am"			{if (hour == 12) hour = 0;}
<MIN>"</td>"			{train << hour << ':' << minutes << ' '; if(issource == 1) {train << hour << ':' << minutes << ' '; issource = 0;} if (arrival == 0) BEGIN DISTANCE; else {arrival = 0; BEGIN HOUR;} }

<DISTANCE>{number}+		{currdist = atoi(yytext); train << currdist-prevdist << '\n'; prevdist = currdist; BEGIN EXTRACT;}
<EXTRACT>" Trains time table by cities"		{BEGIN INITIAL;}
.|\n	;
%%

int main(int argc, char * argv[])
{
	if (argc != 2) 
	{
		printf("Usage: <program name> <file to be created>\n");
		return 0;
	}
	
	train.open(argv[1]);

	arrival = hour = minutes = prevdist = currdist = 0;
	issource = 1;
	train_id = 0;

	yylex();

	return 1;
}
