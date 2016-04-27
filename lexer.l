%{
#include <stdio.h>
#include <stdint.h>
#include "syntax.tab.hpp"
%}

%%
[a-zA-Z][a-zA-Z0-9]*    yylval.str=strdup(yytext); return WORD;
\'[a-zA-Z0-9/ -]*\'		yylval.str=strdup(yytext); return FILENAME;
[0-9]+                  yylval.num=atoi(yytext);   return NUM;
[0-9]+"."[0-9]*         yylval.num=atof(yytext);   return NUM;
[,;()\n]                return *yytext;
\.                      return DOT;
[ \t]+                  /* пропускаем пробелы и табы*/;
\+						return PLUS;
\-						return MINUS;
\*						return MULT;
\/						return DIV;
\|						return CONVEY;
&&						return AND;
\|\|					return OR;
\'.*\'					return ANYTHING;
%%
