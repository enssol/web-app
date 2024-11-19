// Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
// SPDX-License-Identifier: AGPL-3.0-or-later

%{
#include <stdio.h>
#include <stdlib.h>

// Function prototype for error handling
void yyerror(const char *s);
%}

%token NUMBER

%%
input:
    | input line
    ;

line:
    '\n'
    | exp '\n' { printf("%d\n", $1); }
    | error '\n' { yyerror("Syntax error"); yyerrok; }
    ;

exp:
    NUMBER { $$ = $1; }
    | exp '+' exp { $$ = $1 + $3; }
    | exp '-' exp { $$ = $1 - $3; }
    | exp '*' exp { $$ = $1 * $3; }
    | exp '/' exp { $$ = $1 / $3; }
    ;

%%

int main(void) {
    printf("Enter expressions:\n");
    return yyparse();
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
