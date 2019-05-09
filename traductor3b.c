
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"
#define LONG 100  // longitud de los identificadores
int preanalisis;
char id1[LONG], id2[LONG];
char nombreClase[LONG];

void parea(int);
void error();
void prog();
void before_class();
void body();
void func();
void func1();
void func2();
void code();

int main() {
  preanalisis = yylex();
  prog();
  if (preanalisis == 0)  // yylex() devuelve 0 en el fin de fichero
    printf("OK\n");
  else
    printf("Sobra algo\n");
}

void parea(int token) {
  if (preanalisis == token) {
    preanalisis = yylex();
  } else {
    printf("línea %d: ", line_number);
    printf("Componente léxico inesperado en %s\n", yytext);
    exit(EXIT_FAILURE);
  }
}

void error() {
  printf("línea %d: ", line_number);
  printf("Error sintáctico en %s\n ", yytext);
  exit(EXIT_FAILURE);
}

void before_class() {
  if (preanalisis == ID) {
    parea(ID);
    before_class();
  } else if (preanalisis == ';') {
    parea(';');
    before_class();
  } else if (preanalisis == CLASS) {
  } else {
    printf("before_class:");
    error();
  }
}

void func1() {
  if (preanalisis == ID) {
    parea(ID);
    func1();
  } else if (preanalisis == ';' || preanalisis == '(' || preanalisis == ')') {
  } else {
    printf("func1:");
    error();
  }
}

void code() {
  if (preanalisis == ID) {
    strcpy (id1, yytext);
    parea(ID);
    code();
  } else if (preanalisis == ';') {
    parea(';');
    code();
  } else if (preanalisis == '(') {
    printf("%s\n", id1);
    parea('(');
    code();
    parea(')');
    code();
  } else if (preanalisis == '{') {
    parea('{');
    code();
    parea('}');
    code();
  } else if (preanalisis == ')' || preanalisis == '}') {
  } else {
    printf("code:");
    error();
  }
}

void func() {
  if (preanalisis == '{') {
    parea('{');
    code();
    parea('}');
    body();
  } else if (preanalisis == ';') {
    parea(';');
    body();
  } else {
    printf("func:");
    error();
  }
}

void func2() {
  if (preanalisis == '(') {
    parea('(');
    func1();
    parea(')');
    func();
  } else if (preanalisis == ';') {
    parea(';');
    // printf("VAR GLOBAL");
    body();
  } else {
    printf("func2:");
    error();
  }
}

void body() {
  if (preanalisis == ID) {
    parea(ID);
    parea(ID);
    func1();
    func2();
  } else if (preanalisis == '}') {
  } else {
    printf("clase:");
    error();
  }
}

void prog() {
  if (preanalisis == ID || preanalisis == ';' || preanalisis == CLASS) {
    before_class();
    parea(CLASS);
    parea(ID);
    parea('{');
    body();
    parea('}');
  } else {
    printf("prog:");
    error();
  }
}
