%skeleton "lalr1.cc" 
%defines
%define api.value.type variant
%define api.token.constructor
%code requires{
    #include "headers/Node.cpp"
}

%code{
  #include <string>
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  Node* root;
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

%left LT GT EQ NE LTE GTE
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO POW DOTDOT
%left AND OR
%right UNARY

%type <Node*> block
%type <Node*> statements
%type <Node*> opt_laststatement
%type <Node*> statement
%type <Node*> laststatement
%type <Node*> optexplist
%type <Node*> varlist
%type <Node*> explist
%type <Node*> var
%type <Node*> exp
%type <Node*> prefixexp
%type <Node*> functioncall
%type <Node*> function
%type <Node*> args
%type <Node*> else
%type <Node*> elseif
%type <Node*> optcommaexp
%type <Node*> namelist
%type <Node*> funcname
%type <Node*> funcbody
%type <Node*> optparlist
%type <Node*> parlist
%type <Node*> funcnamebase
%type <Node*> optcolonname
%type <Node*> eqexplistopt
%type <Node*> optfieldlist
%type <Node*> fieldlist
%type <Node*> field
%type <Node*> tableconstructor
%type <Node*> str

  

%token ENDOFFILE 0 "end of file"
%%

root : block                                    {root=$1;}

block   : statements opt_laststatement          {$$=(new Node("Block"))->add($1)->add($2);}

statements : /* empty */                        {$$=new Node("Statements");}
        | statements statement opt_semicolon    {$$=$1->add($2);}
        
opt_laststatement: /* empty */                  {$$=new Node("pass");}
                | laststatement opt_semicolon   {$$=$1;}

laststatement   : RETURN optexplist             {$$=(new Node("return"))->add($2);}
                | BREAK                         {$$=new Node("break");}
        


statement   : varlist EQUAL explist             {$$=(new Node("affectation"))->add($1)->add($3);}
            | functioncall                      {$$=$1;}
            | DO block END                      {$$=(new Node("do end"))->add($2);}
            | WHILE exp DO block END            {$$=(new Node("while"))->add($2)->add($4);}
            | REPEAT block UNTIL exp            {$$=(new Node("repeat"))->add($2)->add($4);}
            | IF exp THEN block elseif else END {$$=(new Node("if"))->add($2)->add($4)->add($5)->add((new Node("else"))->add($6));}
            | FOR NAME EQUAL exp COMMA exp optcommaexp DO block END     {$$=(new Node("forequal"))->add((new Node("name", $2)))->add($4)->add($6)->add($7)->add($9);}
            | FOR namelist IN explist DO block END      {$$=((new Node("forin")))->add($2)->add($4)->add($6);}
            | FUNCTION funcname funcbody        {$$=(new Node("functiondef"))->add($2)->add($3);}
            | LOCAL FUNCTION NAME funcbody      {$$=(new Node("localfunctiondef", $3))->add($4);}
            | LOCAL namelist eqexplistopt       {$$=(new Node("local decl/aff"))->add($2)->add($3);}

eqexplistopt: /* empty */                       {$$=new Node("pass");}
            | EQUAL explist                     {$$=$2;}

funcname : funcnamebase optcolonname            {$$ = (new Node("funcname"))->add($1)->add($2);}

funcnamebase : NAME                             {$$=new Node("fnname", $1);}
	    | funcnamebase DOT NAME                 {$$=$1;$$->value = $$->value + "." + $3;}
	    
optcolonname: /* empty */                       {$$=new Node("pass");}
            | COLON NAME                        {$$=new Node("fname",$2);}

funcbody: POPEN optparlist PCLOSE block END     {$$=(new Node("fnbody"))->add($2)->add($4);}

optparlist:  /* empty */                        {$$=new Node("pass");}
        | parlist                               {$$=$1;}


optcommaexp  : /* empty */                      {$$=new Node("pass");}
        | COMMA exp                             {$$=$2;}

 
elseif  : /* empty */                           {$$=new Node("elseif");}
        | elseif ELSEIF exp THEN block          {$$=$1->add($3)->add($5);}

else: /* empty */                               {$$=new Node("pass");}
    | ELSE block                                {$$=$2;}

var : NAME                                      {$$=new Node("Var", $1);}
    | prefixexp SBRACKETOPEN exp SBRACKETCLOSE  {$$=(new Node("tableretrieve"))->add($1)->add($3);}
    | prefixexp DOT NAME                        {$$=(new Node("propretrieve"))->add($1)->add(new Node("Var", $3));}

varlist : var                                   {$$=(new Node("varlist"))->add($1);}
        | varlist COMMA var                     {$$=$1->add($3);}
        
exp : TRUE                      {$$=new Node("value", "true");}
    | FALSE                     {$$=new Node("value", "false");}
    | NIL                       {$$=new Node("value", "nil");}
    | NUMBER                    {$$=new Node("number", $1);}
    | str                       {$$=$1;}  
    | DOTDOTDOT                 {$$=new Node("DOTDOTDOT", $1);}
    | prefixexp                 {$$=$1;}
    | function                  {$$=$1;}
    | tableconstructor          {$$=$1;}
    | MINUS exp %prec UNARY     {$$=(new Node($1, ""))->add($2);}
    | NOT exp %prec UNARY       {$$=(new Node($1, ""))->add($2);}
    | HASH exp %prec UNARY      {$$=(new Node($1, ""))->add($2);}
    | exp PLUS exp              {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp MULTIPLY exp          {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp MINUS exp             {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp DIVIDE exp            {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp POW exp               {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp MODULO exp            {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp LT exp                {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp LTE exp               {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp GT exp                {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp GTE exp               {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp EQ exp                {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp NE exp                {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp AND exp               {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp OR exp                {$$=(new Node($2, ""))->add($1)->add($3);}
    | exp DOTDOT exp            {$$=(new Node($2, ""))->add($1)->add($3);}
    
function : FUNCTION funcbody            {$$=(new Node("anonfunc"))->add($2);}

parlist : namelist                      {$$=(new Node("parlist"))->add($1);}
        | namelist COMMA DOTDOTDOT      {$$=(new Node("parlist", $3))->add($1);}
        | DOTDOTDOT                     {$$=new Node("parlist", $1);}
        

namelist: NAME                          {$$=(new Node("namelist"))->add(new Node("name", $1));}
        | namelist COMMA NAME           {$$=$1->add(new Node("name", $3));}

explist : exp {$$=(new Node("Explist"))->add($1);}
        | explist COMMA exp {$$=$1->add($3);}

prefixexp   : var                       {$$=$1;}
            | functioncall              {$$=$1;}
            | POPEN exp PCLOSE          {$$=$2;}
            
functioncall: prefixexp args            {$$=(new Node("functioncall"))->add($1)->add($2);}
            | prefixexp COLON NAME args {$$=(new Node("functioncall"))->add($1)->add(new Node("name",$3))->add($4);}
                
args: POPEN optexplist PCLOSE           {$$=(new Node("args"))->add($2);}
    | tableconstructor                  {$$=(new Node("tableargs"))->add($1);}
    | str                            {$$=(new Node("str_arg"))->add($1);}

str: STRING                             {$$=new Node("string", $1.erase($1.length()-1,1).erase(0,1));}

optexplist  : /* empty */               {$$=new Node("pass");}
            | explist                   {$$=$1;}

				
tableconstructor : CBRACKETOPEN optfieldlist CBRACKETCLOSE {$$=(new Node("tableconstructor"))->add($2);}

optfieldlist    : /* empty */                       {$$=new Node("pass");}
                | fieldlist optfieldsep             {$$=$1;}
                
optfieldsep    : /* empty */ 
                | fieldsep 
                
fieldlist   : field                                 {$$=(new Node("fieldlist"))->add($1);}
            | fieldlist fieldsep field              {$$=$1->add($3);}

field   : SBRACKETOPEN exp SBRACKETCLOSE EQUAL exp  {$$=(new Node("brackets"))->add($2)->add($5);}
        | NAME EQUAL exp                            {$$=(new Node("name", $1))->add($3);}
        | exp                                       {$$=(new Node("simple"))->add($1);}

fieldsep    : COMMA 
            | SEMICOLON 

opt_semicolon : /* empty */
                | SEMICOLON

