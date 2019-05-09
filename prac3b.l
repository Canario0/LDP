%{
#define CLASS 300
#define ID 301
#define IDTIPO 30
%}
%option case-insensitive
%x MCOMMENT
let [A-Za-zñ_$]
dig [0-9]
id {let}({let}|{dig}|[.*]|[ ]*\[{dig}*\])*
frac [.]{dig}+
expo [eE][+-]?{dig}+
dec {dig}+({frac}|{expo})
num {dig}+
cadena \"[^"]*\"
lcomentario [/][/].*$
 // reservado (import|class|package|private|public|static|throws|return|final|const|instanceof|enum|if|else|switch|case|break|default|for|do|while|try|catch|finally|new|this|super)
 // operadores ("+"{1,2}|"-"{1,2}|!|~|"*"|"/"|%|<|>|<=|>=|==|!=|&|"^"|"|"{1,2}|&{1,2}|"?"|:|=|"+="|"-="|"*="|"/="|%=|&=|"^="|"|="|[,.;])
 // operando (null|true|false|boolean|byte|int|float|chat|double|long|short|signed|unsigned|void|{cadena}|{dec}|{num}|{var})
 /* mcomment: comentarios multilínea
    lcomment: comentarios simples
 */
 int line_number = 1;
%%
[ \t] printf("%s", yytext);
{lcomentario} {printf("Inline comment\n");}
[/][*] {BEGIN MCOMMENT;}
<MCOMMENT>(.|\n) ;
<MCOMMENT>[*][/] {BEGIN INITIAL; printf("Multiline comment\n");}
class { printf("%s", yytext); return CLASS;}
[{}] {return yytext[0];}
\n line_number++;
{id} {printf("ID: %s\n", yytext); return ID;}
. ;
