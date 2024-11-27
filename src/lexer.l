%{
#include "parser.h"
#include <stdio.h>
%}

%%
"if"         { return IF; }
"else"       { return ELSE; }
"while"      { return WHILE; }
[0-9]+       { yylval = atoi(yytext); return NUMBER; }
[+\-*/=()]   { return yytext[0]; }
[ \t\n]+     { /* skip whitespace */ }
.            { return ERROR; }
%%
