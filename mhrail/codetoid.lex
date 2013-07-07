%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int id;

%}

number [0-9]
text [A-Z]

%%

{number}+	{id = atoi(yytext);}
{text}+		{printf("if (strcmp(code,\"%s\")== 0) return %d;\n",yytext,id);}
\n ;
