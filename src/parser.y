%{
#include "parser.h"
#include <stdio.h>
%}

%token IF ELSE WHILE NUMBER
%left '+' '-'
%left '*' '/'

%%
program:
    | program statement
    ;

statement:
      IF expression THEN statement ELSE statement
    | WHILE expression DO statement
    | expression
    ;

expression:
      NUMBER
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    ;

%%
int main() {
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}
