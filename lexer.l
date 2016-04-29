%{
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include "types.hpp"
#include "syntax.tab.hpp"
#include <string>
#include <readline/readline.h>
static char* residue_string=NULL;
static char* input_line=NULL;
extern std::string begin_line;
#define YY_INPUT(buf,result,max_size) \
	if ( YY_CURRENT_BUFFER_LVALUE->yy_is_interactive ) \
		{ \
		if(!residue_string)\
		{\
			input_line=readline(begin_line.c_str());\
			residue_string=input_line;\
		}\
		size_t n; \
		if(input_line)\
		for ( n = 0; n < max_size-1 && residue_string[n]!='\0'; ++n ) \
		{\
			buf[n] = residue_string[n]; \
		}\
        buf[n++]='\n';\
		if(n==max_size && (input_line[n]!='\0' || input_line[n]!=EOF))\
			residue_string=&input_line[n+1];\
        else \
        {\
            residue_string=NULL;\
            free(input_line);\
        }\
		result = n; \
		} \
	else \
		{ \
		errno=0; \
		while ( (result = fread(buf, 1, max_size, yyin))==0 && ferror(yyin)) \
			{ \
			if( errno != EINTR) \
				{ \
				YY_FATAL_ERROR( "input in flex scanner failed" ); \
				break; \
				} \
			errno=0; \
			clearerr(yyin); \
			} \
		}

%}

%%
[a-zA-Z-][a-zA-Z0-9-]*  yylval.str=strdup(yytext); return WORD;
\'[a-zA-Z0-9/ -]*\'		yylval.str=strdup(yytext); return FILENAME;
[0-9]+                  yylval.num=atoi(yytext);   return NUM;
[0-9]+"."[0-9]*         yylval.num=atof(yytext);   return NUM;
\.                      return DOT;
[ \t]+                  return ' ';/* пропускаем пробелы и табы, возвращаем 1 пробел*/;
[ \t]*\([ \t]*  		return '(';
[ \t]*\)[ \t]*  		return ')';
[ \t]*\,[ \t]*  		return ',';
[ \t]*\;[ \t]*  		return ';';
\n				  		return '\n';
\+						return PLUS;
\*						return MULT;
\/						return DIV;
\|						return CONVEY;
&&						return AND;
\|\|					return OR;
\'.*\'					return ANYTHING;
%%
