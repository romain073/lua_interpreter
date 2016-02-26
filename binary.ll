%top{
#include "binary.tab.hh"
#define YY_DECL yy::parser::symbol_type yylex()
}
%option noyywrap nounput batch noinput


NBRS [0-9]+

%%

\-\-[^\n]*\n /* comment */
({NBRS}|{NBRS}?\.{NBRS})([Ee][-+]?{NBRS})?       { return yy::parser::make_NUMBER(yytext); }
true         { return yy::parser::make_TRUE(yytext); }
false         { return yy::parser::make_FALSE(yytext); }
nil         { return yy::parser::make_NIL(yytext); }
end         { return yy::parser::make_END(yytext); }
function         { return yy::parser::make_FUNCTION(yytext); }
do         { return yy::parser::make_DO(yytext); }
while         { return yy::parser::make_WHILE(yytext); }
repeat         { return yy::parser::make_REPEAT(yytext); }
until         { return yy::parser::make_UNTIL(yytext); }


for         { return yy::parser::make_FOR(yytext); }
in          { return yy::parser::make_IN(yytext); }

local          { return yy::parser::make_LOCAL(yytext); }

return          { return yy::parser::make_RETURN(yytext); }
break          { return yy::parser::make_BREAK(yytext); }

if         { return yy::parser::make_IF(yytext); }
then         { return yy::parser::make_THEN(yytext); }
else         { return yy::parser::make_ELSE(yytext); }
elseif         { return yy::parser::make_ELSEIF(yytext); }

:         { return yy::parser::make_COLON(yytext); }

not         { return yy::parser::make_NOT(yytext); }
\-         { return yy::parser::make_MINUS(yytext); }
#         { return yy::parser::make_HASH(yytext); }
		 
\+          { return yy::parser::make_PLUS(yytext); }
\*          { return yy::parser::make_MULTIPLY(yytext); }
\/          { return yy::parser::make_DIVIDE(yytext); }
\^          { return yy::parser::make_POW(yytext); }
\%          { return yy::parser::make_MODULO(yytext); }
\<          { return yy::parser::make_LT(yytext); }
\<=          { return yy::parser::make_LTE(yytext); }
\>          { return yy::parser::make_GT(yytext); }
\>=          { return yy::parser::make_GTE(yytext); }
==          { return yy::parser::make_EQ(yytext); }
~=          { return yy::parser::make_NE(yytext); }
and          { return yy::parser::make_AND(yytext); }
or          { return yy::parser::make_OR(yytext); }


\{         { return yy::parser::make_CBRACKETOPEN(yytext); }
\}         { return yy::parser::make_CBRACKETCLOSE(yytext); }
\(         { return yy::parser::make_POPEN(yytext); }
\)         { return yy::parser::make_PCLOSE(yytext); }
\[         { return yy::parser::make_SBRACKETOPEN(yytext); }
\]         { return yy::parser::make_SBRACKETCLOSE(yytext); }

[A-Za-z_][A-Za-z_0-9]*         { return yy::parser::make_NAME(yytext); }
;         { return yy::parser::make_SEMICOLON(yytext); }
=         { return yy::parser::make_EQUAL(yytext); }
,         { return yy::parser::make_COMMA(yytext); }
\'[^']*\'    { return yy::parser::make_STRING(yytext); }
\"[^"]*\"    { return yy::parser::make_STRING(yytext); }
\.    { return yy::parser::make_DOT(yytext); }
\.\.    { return yy::parser::make_DOTDOT(yytext); }
\.\.\.    { return yy::parser::make_DOTDOTDOT(yytext); }


\n           /* munch */
[ \t]           /* munch */
<<EOF>>       return yy::parser::make_EOF();
%%