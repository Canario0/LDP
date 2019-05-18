
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"
#define LONG 500  // longitud de los identificadores
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
void addFunc(); // añade un nombre a la lista de funciones definida
void printDefinitions();
void addCall(); // añade el par funcCaller, funcCalled a la lista de llamadas
void printCalls();
void printNodes();// imprime las llamadas

typedef struct nodeDefinitions {
  char name[LONG];
  struct nodeDefinitions *next;
} definitions;

typedef struct nodeCalls {
  char caller[LONG];
  char called[LONG];
  struct nodeCalls *next;
} calls;

int preanalisis;
char funcCaller[LONG], funcCalled[LONG];
definitions *funcDefinidas;
definitions *funcDefinidasFin;

calls *funcCalls;
calls *funcCallsFin;
FILE *out_file;

int main() {
  out_file = fopen("out", "w");
  if (out_file == NULL) {
    printf("Error! Could not open file\n");
    exit(-1);
  }
  funcDefinidas = malloc(sizeof(definitions));
  if (funcDefinidas == NULL) {
    printf("Error reservando memoria\n");
    exit(-1);
  }
  funcDefinidasFin = funcDefinidas;

  funcCalls = malloc(sizeof(calls));
  if (funcCalls == NULL) {
    printf("Error reservando memoria\n");
    exit(-1);
  }
  funcCallsFin = funcCalls;

  fprintf(out_file, "strict digraph G {\n\tnode [shape=box];\n");
  preanalisis = yylex();
  prog();
  if (preanalisis == 0) {  // yylex() devuelve 0 en el fin de fichero
    printf("OK\n");
    printNodes();
    fprintf(out_file, "}\n");
    // printDefinitions();
    // printCalls();
  } else
    printf("Sobra algo\n");
}

void printNodes() {
  struct nodeDefinitions *func = funcDefinidas;
  calls *aux = funcCalls;
  while (aux != funcCallsFin) {
    while (func != funcDefinidasFin) {
      if (strcmp(aux->called, func->name)==0) {
        fprintf(out_file,"\t%s -> %s\n", aux->caller, aux->called);
        break;
      }
      func = func->next;
    }
    func = funcDefinidas;
    aux = aux->next;
  }
}

void printDefinitions() {
  struct nodeDefinitions *aux;
  aux = funcDefinidas;
  while (aux != funcDefinidasFin) {
    printf("%s\n", aux->name);
    aux = aux->next;
  }
}

void printCalls() {
  struct nodeCalls *aux;
  aux = funcCalls;
  while (aux != funcCallsFin) {
    printf("%s -> %s\n", aux->caller, aux->called);
    aux = aux->next;
  }
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
    strcpy(funcCaller, yytext);
    parea(ID);
    func1();
  } else if (preanalisis == '{') {
    parea('{');
    llaves();
    parea('}');
    parea(';');
    body();
  } else if (preanalisis == ';' || preanalisis == '(' || preanalisis =='}') {
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
    strcpy(funcCalled, yytext);
    parea(ID);
    code();
  } else if (preanalisis == ';') {
    parea(';');
    code();
  } else if (preanalisis == '(') {
    addCall();
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
    addFunc();
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

void addFunc() {
  strcpy(funcDefinidasFin->name, funcCaller);
  struct nodeDefinitions *aux = malloc(sizeof(definitions));
  funcDefinidasFin->next = aux;
  funcDefinidasFin = funcDefinidasFin->next;
}

void addCall() {
  strcpy(funcCallsFin->caller, funcCaller);
  strcpy(funcCallsFin->called, funcCalled);
  struct nodeCalls *aux = malloc(sizeof(calls));
  funcCallsFin->next = aux;
  funcCallsFin = funcCallsFin->next;
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
  } else if (preanalisis == '}') {
  } else {
    printf("func2:");
    error();
  }
}

void body() {
  if (preanalisis == ID) {
    parea(ID);
    strcpy(funcCaller, yytext);
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
