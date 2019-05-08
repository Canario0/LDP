%{
#include <set>
#include<string>
#include <math.h> 
using namespace std;    
%}
%option case-insensitive
%x MCOMMENT
let [a-zA-Z]
dig [0-9]
var {let}({let}|{dig})*
frac [.]{dig}+
expo [eE][+-]?{dig}+
dec {dig}+({frac}|{expo})
num {dig}+
cadena \"[^"]*\"
lcomentario [/][/].*$
reservado (import|class|package|private|public|static|throws|return|final|const|instanceof|enum|if|else|switch|case|break|default|for|do|while|try|catch|finally|new|this|super)
operadores ("+"{1,2}|"-"{1,2}|!|~|"*"|"/"|%|<|>|<=|>=|==|!=|&|"^"|"|"{1,2}|&{1,2}|"?"|:|=|"+="|"-="|"*="|"/="|%=|&=|"^="|"|="|[,.;])
operando (null|true|false|boolean|byte|int|float|chat|double|long|short|signed|unsigned|void|{cadena}|{dec}|{num}|{var})
 /* mcomment: comentarios multilínea
    lcomment: comentarios simples
 */
 int mcomment =0 ; int lcomment = 0; int N1 = 0; int N2 = 0;
 set<string> operandos; set<string> operadores;
%%
{lcomentario} {lcomment++;}
[/][*] {BEGIN MCOMMENT;}
<MCOMMENT>(.|\n) ;
<MCOMMENT>[*][/] {mcomment++; BEGIN INITIAL;}

{reservado} {N1++; operadores.insert(yytext);}
{operando} {N2++; operandos.insert(yytext);}
{operadores} {N1++; operadores.insert(yytext);}
[[{(] {N1++;  operadores.insert(yytext);}
.|\n ;
%%

int main( int argc, char** argv ){
    if ( argc > 1 )
        yyin = fopen( argv[1], "r" );
    else{
        printf("Missing argument\n");
        exit(1);
    }
    yylex();
    int n1= operadores.size();
    int n2= operandos.size();
    int N = N1 +  N2;
    int n = n1 + n2;
    printf ("%d %d %d %d\n",n1, n2, N1, N2);
    float V = N *log2(n);
    float L = (2/(float)n1)*(n2/(float)N2);
    float D = 1/L;
    float E = V*D;
    printf("%f %f %f %f %f %f %f\n", V, L, D, L*V, E, E/18, (pow(E, 2/3.0)/3000));
    printf ("%d %d\n",lcomment, mcomment);
    return 0;
}