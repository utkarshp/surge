%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

int id,flag;
string train_name;

ofstream train_ids ;
ofstream names ;
%}

text [a-zA-Z_]
number [0-9]

%s EXTRACT
%s NUMBER
%s AFTNUM
%s NAME

%%

<INITIAL>"Dest. Arr. Time"	{BEGIN EXTRACT;}
<EXTRACT>"target='_blank'>"	{BEGIN NUMBER;}
<NUMBER>{number}+		{if (strlen(yytext) == 5) {train_ids << yytext << "\n";  names << yytext << '\t'; flag=1;} else flag=0; BEGIN AFTNUM;}
<AFTNUM>"target='_blank'>"	{BEGIN NAME;}
<NAME>{text}+			{train_name = train_name + string(yytext);}
<NAME>" "			{train_name = train_name + "_";}
<NAME>"</a></td>"		{if (flag) names << train_name << '\n'; train_name = string(); BEGIN EXTRACT;}
<EXTRACT>"Previous Page" 	{BEGIN INITIAL;}
.|\n	;

%%

int  main()
{
	train_ids.open("just_id");
	names.open("railway_id");
	
	yylex();

	train_ids.close();
	names.close();

	return 1;
}
