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

%token ENDOFFILE 0 "end of file"
%%

root : block{root=Node("root","root");root.push_back($1);}

block   : statements opt_laststatement {$$=Node("Block","");$$.push_back($1);if($2.tag != "undefined") $$.push_back($2);}

statements : /* empty */ {$$=Node("Statements","");}
        | statements statement opt_semicolon {$$=$1;$$.push_back($2);}
        
opt_laststatement: /* empty */ {$$=Node();}
                | laststatement opt_semicolon {$$=$1;}

laststatement   : RETURN optexplist {$$=Node("return","");$$.push_back($2);}
                | BREAK {$$=Node("break","");}
        


statement   : varlist EQUAL explist {$$=Node("=", "");$$.push_back($1);$$.push_back($3);}
            | functioncall          
            | DO block END          
            | WHILE exp DO block END
            | REPEAT block UNTIL exp
            | IF exp THEN block elseif else END
            | FOR NAME EQUAL exp COMMA exp optcommaexp DO block END
            | FOR namelist IN explist DO block END
            | FUNCTION funcname funcbody
            | LOCAL FUNCTION NAME funcbody
            | LOCAL namelist eqexplistopt 

eqexplistopt: /* empty */
            | EQUAL explist

funcname : funcnamebase optcolonname

funcnamebase : NAME 
	    | funcnamebase DOT NAME
	    
optcolonname: /* empty */
            | COLON NAME

funcbody: POPEN optparlist PCLOSE block END

optparlist:  /* empty */
        | parlist


optcommaexp  : /* empty */
        | COMMA exp

 
elseif  : /* empty */
        | elseif ELSEIF exp THEN block

else: /* empty */
    | ELSE block

var : NAME {$$=Node("Var", $1);}
    | prefixexp SBRACKETOPEN exp SBRACKETCLOSE 
    | prefixexp DOT NAME

varlist : var {$$=Node("varlist","");$$.push_back($1);}
        | varlist COMMA var {$$=$1;$$.push_back($3);}
        
exp : TRUE      {$$=Node("value", "true");}
    | FALSE     {$$=Node("value", "false");}
    | NIL       {$$=Node("value", "nil");}
    | NUMBER    {$$=Node("number", $1);}
    | STRING       
    | DOTDOTDOT       
    | prefixexp 
    | function
    | tableconstructor 
    | MINUS exp %prec UNARY
    | NOT exp %prec UNARY
    | HASH exp %prec UNARY
    | exp PLUS exp 
    | exp MULTIPLY exp 
    | exp MINUS exp 
    | exp DIVIDE exp 
    | exp POW exp 
    | exp MODULO exp 
    | exp LT exp 
    | exp LTE exp 
    | exp GT exp 
    | exp GTE exp 
    | exp EQ exp 
    | exp NE exp 
    | exp AND exp 
    | exp OR exp 
    | exp DOTDOT exp     
    
function : FUNCTION funcbody  

parlist : namelist 
        | namelist COMMA DOTDOTDOT 
        | DOTDOTDOT 
        

namelist: NAME 
        | namelist COMMA NAME  

explist : exp {$$=Node("Explist","");$$.push_back($1);}
        | explist COMMA exp {$$=$1;$$.push_back($3);}

prefixexp   : var 
            | functioncall 
            | POPEN exp PCLOSE 
            
functioncall: prefixexp args 
            | prefixexp COLON NAME args  
                
args: POPEN optexplist PCLOSE 
    | tableconstructor 
    | STRING 

optexplist  : /* empty */ 
            | explist 

				
tableconstructor : CBRACKETOPEN optfieldlist CBRACKETCLOSE 

optfieldlist    : /* empty */ 
                | fieldlist optfieldsep 
                
optfieldsep    : /* empty */ 
                | fieldsep 
                
fieldlist   : field 
            | fieldlist fieldsep field 

field   : SBRACKETOPEN exp SBRACKETCLOSE EQUAL exp 
        | NAME EQUAL exp 
        | exp 

fieldsep    : COMMA 
            | SEMICOLON 

opt_semicolon : /* empty */
                | SEMICOLON

