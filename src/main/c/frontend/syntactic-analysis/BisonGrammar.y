%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	char * id;
	Token token;

	/** Non-terminals. */

	Type * type;
	DefaultValue * default_value;
	ConstraintValue * constraint_value;
	BooleanExpression * boolean_expression;
	Constraint * constraint;
	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
	Attributes * attributes;
	Constraints * constraints;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>

/** Terminals. */
/*
%token <integer> INTEGER
%token <token> ADD
%token <token> CLOSE_PARENTHESIS
%token <token> DIV
%token <token> MUL
%token <token> OPEN_PARENTHESIS
%token <token> SUB 
*/

/*Nuestro*/
%token <token> CREATE
%token <token> TABLE
%token <token> SEMICOLON
%token <id> ID
%token <token> COMA
%token <token> NOT
%token <token> NULL
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> CONSTRAINT
%token <integer> INTEGER
%token <token> CHECK
%token <token> PRIMARY
%token <token> KEY
%token <token> FOREIGN
%token <token> UNIQUE

%token <token> UNKNOWN

/** Non-terminals. */

/*
%type <constant> constant
%type <factor> factor
*/

/* Nuestro */
%type <program> program
%type <expression> content
%type <attributes> attributes
%type <attributes> attribute
%type <constraints> constraints
%type <constraint> constraint
%type <default_value> default_value
%type <type> type
%type <expression> expression
%type <constraint_value> constraint_value
%type <boolean_expression> boolean_expression

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
/*%left ADD SUB
%left MUL DIV*/

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: tables																			{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

tables: CREATE TABLE ID OPEN_PARENTHESIS content CLOSE_PARENTHESIS SEMICOLON tables
	| SEMICOLON
	| %empty
	;
	
content: attributes
	| constraints
	| attributes COMA constraints 
	| %empty
	;

attributes: attribute
	| attribute COMA attribute
	;
	
attribute: ID type
	| ID type default_value
	| ID type constraint
	| ID type default_value constraint
	;


// TODO
constraints: 
	;

constraint: CONSTRAINT ID constraint_value
	;

constraint_value: CHECK OPEN_PARENTHESIS boolean_expression CLOSE_PARENTHESIS
	| PRIMARY KEY OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
	| FOREIGN KEY OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
	| UNIQUE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
	; 

expression: 
	;

default_value:
	;

type:
	;

boolean_expression:
	;


/*
expression: expression[left] ADD expression[right]								{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]									{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]									{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]									{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor																	{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS							{ $$ = ExpressionFactorSemanticAction($2); }
	| constant																	{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;

%%
*/