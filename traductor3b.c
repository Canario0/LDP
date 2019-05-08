
#include "lex.yy.c"
#include <stdlib.h>
#include <string.h>
#define LONG 100 // longitud de los identificadores
int preanalisis;
char  id1[LONG], id2[LONG];
char  nombreClase[LONG];

void parea (int token) {
	if (preanalisis == token) {
		preanalisis = yylex();
	} else {
		printf ("línea %d: ", line_number);
		printf ("Componente léxico inesperado en %s\n", yytext);
		exit (EXIT_FAILURE);
	}
}

void error() {
		printf ("línea %d: ", line_number);
		printf ("Error sintáctico en %s\n ", yytext);
		exit (EXIT_FAILURE);		
	}

void before_class () {
	if (preanalisis == ID) {
		parea (ID); before_class();
	} else if (preanalisis == ';') {
		parea (';'); before_class();
	} else if (preanalisis == CLASS) {
	} else {
		printf ("before_class:");
		error();
	}
} 

void prog () {
	if (preanalisis == ID || preanalisis == ';' || preanalisis == CLASS) {
		before_class(); 
		parea (CLASS); 
		parea(ID);
		parea ('{');
	} else {
		printf ("prog:");
		error();
	}
}

int main(){
	preanalisis = yylex();
	prog();
	if (preanalisis == 0) // yylex() devuelve 0 en el fin de fichero
		printf ("OK\n")
		;
	else
		printf ("Sobra algo\n");
}
