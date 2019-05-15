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
lcomentario [/][/].*$
 int line_number = 1;
%%
[ \t] ;
{lcomentario} {printf("Inline comment\n");}
[/][*] {BEGIN MCOMMENT;}
<MCOMMENT>(.|\n) ;
<MCOMMENT>[*][/] {BEGIN INITIAL; printf("Multiline comment\n");}
class {return CLASS;}
[{};()] {return yytext[0];}
\n line_number++;
{id} {/*printf("ID: %s\n", yytext);*/ return ID;}
. ;
