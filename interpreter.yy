%skeleton "lalr1.cc" 
%defines
%define api.value.type variant
%define api.token.constructor
%code requires{
    #include "headers/Node.h"
}

%code{
  #include <string>
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  Node root;
}
%token <std::string> NUMBER
%token <std::string> SEMICOLON
%token <std::string> NAME
%token <std::string> POPEN
%token <std::string> PCLOSE
%token <std::string> SBRACKETOPEN
%token <std::string> SBRACKETCLOSE
%token <std::string> CBRACKETOPEN
%token <std::string> CBRACKETCLOSE
%token <std::string> EQUAL
%token <std::string> COMMA
%token <std::string> TRUE
%token <std::string> FALSE
%token <std::string> NIL
%token <std::string> STRING
%token <std::string> DOT
%token <std::string> DOTDOT
%token <std::string> DOTDOTDOT
%token <std::string> END
%token <std::string> FUNCTION
%token <std::string> NOT
%token <std::string> MINUS
%token <std::string> HASH
%token <std::string> PLUS
%token <std::string> MULTIPLY
%token <std::string> DIVIDE
%token <std::string> POW
%token <std::string> MODULO
%token <std::string> LT
%token <std::string> LTE
%token <std::string> GT
%token <std::string> GTE
%token <std::string> EQ
%token <std::string> NE
%token <std::string> AND
%token <std::string> OR
%token <std::string> COLON

%token <std::string> DO
%token <std::string> WHILE
%token <std::string> REPEAT
%token <std::string> UNTIL

%token <std::string> FOR
%token <std::string> IN

%token <std::string> LOCAL

%token <std::string> IF
%token <std::string> THEN
%token <std::string> ELSE
%token <std::string> ELSEIF

%token <std::string> RETURN
%token <std::string> BREAK

%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO POW DOTDOT
%left LT GT EQ NE LTE GTE
%left AND OR
%right UNARY

%type <Node> block
%type <Node> statements
%type <Node> opt_laststatement
%type <Node> statement
%type <Node> laststatement
%type <Node> optexplist
%type <Node> varlist
%type <Node> explist
%type <Node> var
%type <Node> exp
%type <Node> prefixexp
%type <Node> functioncall
%type <Node> function
%type <Node> args
%type <Node> else
%type <Node> elseif
%type <Node> optcommaexp
%type <Node> namelist
%type <Node> funcname
%type <Node> funcbody
%type <Node> optparlist
%type <Node> parlist
%type <Node> funcnamebase
%type <Node> optcolonname
%type <Node> eqexplistopt
%type <Node> optfieldlist
%type <Node> fieldlist
%type <Node> field
%type <Node> tableconstructor

  

%token ENDOFFILE 0 "end of file"
%%

root : block {root=Node("root");root.push_back($1);}

block   : statements opt_laststatement {$$=Node("Block");$$.push_back($1,$2);}

statements : /* empty */ {$$=Node("Statements");}
        | statements statement opt_semicolon {$$=$1;$$.push_back($2);}
        
opt_laststatement: /* empty */ {$$=Node("pass");}
                | laststatement opt_semicolon {$$=$1;}

laststatement   : RETURN optexplist {$$=Node("return");$$.push_back($2);}
                | BREAK {$$=Node("break");}
        


statement   : varlist EQUAL explist             {$$=Node("affectation");$$.push_back($1,$3);}
            | functioncall                      {$$=$1;}
            | DO block END                      {$$=Node("do end");$$.push_back($2);}
            | WHILE exp DO block END            {$$=Node("while");$$.push_back($2,$4);}
            | REPEAT block UNTIL exp            {$$=Node("repeat");$$.push_back($2,$4);}
            | IF exp THEN block elseif else END {$$=Node("if");$$.push_back($2,$4);$$.push_back($5,$6);}
            | FOR NAME EQUAL exp COMMA exp optcommaexp DO block END     {$$=Node("forequal");$$.push_back($2,$4);$$.push_back($6,$7);$$.push_back($8);}
            | FOR namelist IN explist DO block END      {$$=Node("forin");$$.push_back($2,$4);$$.push_back($6);}
            | FUNCTION funcname funcbody        {$$=Node("functiondef");$$.push_back($2,$3);}
            | LOCAL FUNCTION NAME funcbody      {$$=Node("localfunctiondef", $3);$$.push_back($4);}
            | LOCAL namelist eqexplistopt       {$$=Node("local decl/aff");$$.push_back($2,$3);}

eqexplistopt: /* empty */               {$$=Node("pass");}
            | EQUAL explist             {$$=$2;}

funcname : funcnamebase optcolonname    {$$ = Node("funcname");$$.push_back($1,$2);}

funcnamebase : NAME                     {$$=Node("fnname", $1);}
	    | funcnamebase DOT NAME         {$$=$1;$$.value = $$.value + "." + $3;}
	    
optcolonname: /* empty */               {$$=Node("pass");}
            | COLON NAME                {$$=Node("fname",$2);}

funcbody: POPEN optparlist PCLOSE block END     {$$=Node("fnbody");$$.push_back($2,$4);}

optparlist:  /* empty */                        {$$=Node("pass");}
        | parlist                               {$$=$1;}


optcommaexp  : /* empty */      {$$=Node("pass");}
        | COMMA exp             {$$=$2;}

 
elseif  : /* empty */                           {$$=Node("elseif");}
        | elseif ELSEIF exp THEN block          {$$=$1;$$.push_back($3,$5);}

else: /* empty */                               {$$=Node("pass");}
    | ELSE block                                {$$=Node("else");$$.push_back($2);}

var : NAME                                      {$$=Node("Var", $1);}
    | prefixexp SBRACKETOPEN exp SBRACKETCLOSE  {$$=Node("tableretrieve");$$.push_back($1,$3);}
    | prefixexp DOT NAME                        {$$=Node("propretrieve");$$.push_back($1,$3);}

varlist : var                                   {$$=Node("varlist","");$$.push_back($1);}
        | varlist COMMA var                     {$$=$1;$$.push_back($3);}
        
exp : TRUE                      {$$=Node("value", "true");}
    | FALSE                     {$$=Node("value", "false");}
    | NIL                       {$$=Node("value", "nil");}
    | NUMBER                    {$$=Node("number", $1);}
    | STRING                    {$$=Node("string", $1);}  
    | DOTDOTDOT                 {$$=Node("DOTDOTDOT", $1);}
    | prefixexp                 {$$=$1;}
    | function                  {$$=$1;}
    | tableconstructor          {$$=$1;}
    | MINUS exp %prec UNARY     {$$=Node($1, "");$$.push_back($2);}
    | NOT exp %prec UNARY       {$$=Node($1, "");$$.push_back($2);}
    | HASH exp %prec UNARY      {$$=Node($1, "");$$.push_back($2);}
    | exp PLUS exp              {$$=Node($2, "");$$.push_back($1,$3);}
    | exp MULTIPLY exp          {$$=Node($2, "");$$.push_back($1,$3);}
    | exp MINUS exp             {$$=Node($2, "");$$.push_back($1,$3);}
    | exp DIVIDE exp            {$$=Node($2, "");$$.push_back($1,$3);}
    | exp POW exp               {$$=Node($2, "");$$.push_back($1,$3);}
    | exp MODULO exp            {$$=Node($2, "");$$.push_back($1,$3);}
    | exp LT exp                {$$=Node($2, "");$$.push_back($1,$3);}
    | exp LTE exp               {$$=Node($2, "");$$.push_back($1,$3);}
    | exp GT exp                {$$=Node($2, "");$$.push_back($1,$3);}
    | exp GTE exp               {$$=Node($2, "");$$.push_back($1,$3);}
    | exp EQ exp                {$$=Node($2, "");$$.push_back($1,$3);}
    | exp NE exp                {$$=Node($2, "");$$.push_back($1,$3);}
    | exp AND exp               {$$=Node($2, "");$$.push_back($1,$3);}
    | exp OR exp                {$$=Node($2, "");$$.push_back($1,$3);}
    | exp DOTDOT exp            {$$=Node($2, "");$$.push_back($1,$3);}
    
function : FUNCTION funcbody            {$$=Node("anonfunc");$$.push_back($2);}

parlist : namelist                      {$$=Node("parlist");$$.push_back($1);}
        | namelist COMMA DOTDOTDOT      {$$=Node("parlist", $3);$$.push_back($1);}
        | DOTDOTDOT                     {$$=Node("parlist", $1);}
        

namelist: NAME                  {$$=Node("namelist");$$.push_back(Node("name", $1));}
        | namelist COMMA NAME   {$$=$1;$$.push_back(Node("name", $3));}

explist : exp {$$=Node("Explist","");$$.push_back($1);}
        | explist COMMA exp {$$=$1;$$.push_back($3);}

prefixexp   : var                       {$$=$1;}
            | functioncall              {$$=$1;}
            | POPEN exp PCLOSE          {$$=Node("exp");}
            
functioncall: prefixexp args            {$$=Node("functioncall");$$.push_back($1,$2);}
            | prefixexp COLON NAME args {$$=Node("functioncall");$$.push_back($1,$3);$$.push_back($4);}
                
args: POPEN optexplist PCLOSE {$$=Node("args");$$.push_back($2);}
    | tableconstructor {$$=Node("tableargs").push_back($1);}
    | STRING {$$=Node("strargs", $1.erase($1.length()-1,1).erase(0,1));}

optexplist  : /* empty */ {$$=Node("pass");}
            | explist {$$=$1;}

				
tableconstructor : CBRACKETOPEN optfieldlist CBRACKETCLOSE {$$=Node("tableconstructor");$$.push_back($2);}

optfieldlist    : /* empty */                       {$$=Node("pass");}
                | fieldlist optfieldsep             {$$=$1;}
                
optfieldsep    : /* empty */ 
                | fieldsep 
                
fieldlist   : field                                 {$$=Node("fieldlist");$$.push_back($1);}
            | fieldlist fieldsep field              {$$=$1;$$.push_back($3);}

field   : SBRACKETOPEN exp SBRACKETCLOSE EQUAL exp  {$$=Node("brackets").push_back($2,$5);}
        | NAME EQUAL exp                            {$$=Node("name", $1).push_back($3);}
        | exp                                       {$$=Node("simple").push_back($1);}

fieldsep    : COMMA 
            | SEMICOLON 

opt_semicolon : /* empty */
                | SEMICOLON

