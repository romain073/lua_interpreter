%skeleton "lalr1.cc" 
%defines
%define api.value.type variant
%define api.token.constructor
%code{
  #include <string>
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
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

%token EOF 0 "end of file"
%%
block   : chunk 

chunk   : statements opt_laststatement

statements : /* empty */ 
        | statements statement opt_semicolon 
        
opt_laststatement: /* empty */ 
                | laststatement opt_semicolon

laststatement   : RETURN optexplist
                | BREAK
        


statement   : varlist EQUAL explist 
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

var : NAME 
    | prefixexp SBRACKETOPEN exp SBRACKETCLOSE 
    | prefixexp DOT NAME

varlist : var 
        | varlist COMMA var 
        
exp : TRUE      
    | FALSE     
    | NIL       
    | NUMBER
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

explist : exp 
        | explist COMMA exp 

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

