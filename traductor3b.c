
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"
#define LONG 500  // longitud de los identificadores
int preanalisis;
char id1[LONG], id2[LONG];
char nombreClase[LONG];
FILE *out_file;

void parea(int);
void error();
void prog();
void before_class();
void body();
void func();
void func1();
void inter();
void llaves();
void func2();
void code();
void code2();
void params();

int main() {
  out_file = fopen("out", "w");
  if (out_file == NULL) {
    printf("Error! Could not open file\n");
    exit(-1);
  }
  fprintf(out_file, "graph G {\n\tnode [shape=box];\n");
  preanalisis = yylex();
  prog();
  if (preanalisis == 0) {  // yylex() devuelve 0 en el fin de fichero
    printf("OK\n");
    fprintf(out_file, "}\n");
  } else
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

void inter() {
  if (preanalisis == ID) {
    parea(ID);
    func1();
  } else if (preanalisis == '{') {
    parea('{');
    code2();
    parea('}');
    body();
  } else {
    printf("inter:");
    error();
  }
}

void func1() {
  if (preanalisis == ID) {
    strcpy(id1, yytext);
    parea(ID);
    func1();
  } else if (preanalisis == ';' || preanalisis == '(' || preanalisis == '{') {
  } else {
    printf("func1:");
    error();
  }
}

void params() {
  if (preanalisis == ID) {
    parea(ID);
    params();
  } else if (preanalisis == ')') {
  } else {
    printf("params:");
    error();
  }
}

void code() {
  if (preanalisis == ID) {
    strcpy(id2, yytext);
    parea(ID);
    code();
  } else if (preanalisis == ';') {
    parea(';');
    code();
  } else if (preanalisis == '(') {
    fprintf(out_file, "\t\"%s\" -- \"%s\";\n", id1, id2);
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

void code2() {
  if (preanalisis == ID) {
    parea(ID);
    code2();
  } else if (preanalisis == ';') {
    parea(';');
    code2();
  } else if (preanalisis == '(') {
    parea('(');
    code2();
    parea(')');
    code2();
  } else if (preanalisis == '{') {
    parea('{');
    code2();
    parea('}');
    code2();
  } else if (preanalisis == ')' || preanalisis == '}') {
  } else {
    printf("code2:");
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

void llaves() {
  if (preanalisis == '{') {
    parea('{');
    llaves();
    parea('}');
    llaves();
  } else if (preanalisis == '}') {
  } else {
    printf("llaves:");
    error();
  }
}

void func2() {
  if (preanalisis == '(') {
    parea('(');
    params();
    parea(')');
    func();
  } else if (preanalisis == ';') {
    parea(';');
    body();
  }else if (preanalisis == '{'){
    parea('{');
    llaves();
    parea('}');
    parea(';');
    body();
  }else if (preanalisis == '}') {
  } else {
    printf("func2:");
    error();
  }
}

void body() {
  if (preanalisis == ID) {
    parea(ID);
    strcpy(id1, yytext);
    // parea(ID);
    // func1();
    inter();
    func2();
  } else if (preanalisis == '{') {
    parea('{');
    code2();
    parea('}');
    body();
  } else if (preanalisis == '}' || preanalisis == ';' || preanalisis == '(') {
  } else {
    printf("body:");
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
