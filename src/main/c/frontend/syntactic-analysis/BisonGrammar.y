%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer_value;
	double double_value;
	char * date_time_value;
	char * id;
	Token token;

	/** Non-terminals. */

	Program * program;
	Tables * tables;
	Content * content;
	Attributes * attributes;
	Attribute * attribute;
	Properties * properties;
	Constraints * constraints;
	Constraint * constraint;
	Type * type;
	DefaultValue * default_value;
	NullCondition * null_condition;
	ConstraintValue * constraint_value;
	BooleanExpression * boolean_expression;
	Expression * expression;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
//TODO
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
*/

%destructor { releaseProperties($$); } <properties>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseBooleanExpression($$); } <boolean_expression>
%destructor { releaseType($$); } <type>
%destructor { releaseDefaultValue($$); } <default_value>
%destructor { releaseConstraintValue($$); } <constraint_value>
%destructor { releaseConstraint($$); } <constraint>
%destructor { releaseConstraints($$); } <constraints>
%destructor { releaseAttribute($$); } <attribute>
%destructor { releaseAttributes($$); } <attributes>
%destructor { releaseContent($$); } <content>
%destructor { releaseTables($$); } <tables>

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
%token <id> ID
%token <integer_value> INTEGER_VALUE
%token <double_value> DOUBLE_VALUE
%token <date_time_value> DATE_VALUE
%token <date_time_value> TIME_VALUE
%token <date_time_value> TIMESTAMP_VALUE

%token <token> CREATE
%token <token> TABLE
%token <token> SEMICOLON
%token <token> COMA
%token <token> NOT
%token <token> NUL
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> CONSTRAINT
%token <token> CHECK
%token <token> PRIMARY
%token <token> KEY
%token <token> FOREIGN
%token <token> UNIQUE
%token <token> INTEGER
%token <token> SMALLINT
%token <token> BIGINT
%token <token> REAL
%token <token> DOUBLE
%token <token> DATE
%token <token> TIMESTAMP
%token <token> INTERVAL
%token <token> TEXT
%token <token> CHAR
%token <token> VARCHAR
%token <token> NUMBER
%token <token> FLOAT
%token <token> TIME
	
%token <token> UNKNOWN

/** Non-terminals. */

%type <program> program
%type <tables> tables
%type <content> content
%type <attributes> attributes
%type <attribute> attribute
%type <properties> properties
%type <constraints> constraints
%type <constraint> constraint
%type <default_value> default_value
%type <type> type
%type <null_condition> null_condition
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

program: tables																		{ $$ = TablesProgramSemanticAction(currentCompilerState(), $1); }
	;

tables: tables tables																{ $$ = TableGenerateSemanticAction($1, $2); }
	| CREATE TABLE ID OPEN_PARENTHESIS content CLOSE_PARENTHESIS SEMICOLON			{ $$ = ContentTablesSemanticAction($3, $5); }
	;

content: attributes																	{ $$ = AttributesContentSemanticAction($1); }
	| constraints																	{ $$ = ConstraintsContentSemanticAction($1); }	
	| attributes COMA constraints 													{ $$ = AttributesAndConstraintsContentSemanticAction($1, $3); }
	| %empty																		{ $$ = EmptyContentSemanticAction(); }				
	;

attributes: attribute																{ $$ = AttributesAttributeSemanticAction($1); }
	| attribute COMA attribute														{ $$ = MultipleAttributesSemanticAction($1, $3); }
	;
	
attribute: ID type																	{ $$ = AttributeTypeSemanticAction($1, $2); }					
	| ID type properties															{ $$ = AttributeTypePropertiesSemanticAction($1, $2, $3); }	

properties:  default_value																{ $$ = SimpleDefaultPropertySemanticAction($1, DEFAULT_VALUE); }	
	| constraint																		{ $$ = SimpleConstraintPropertySemanticAction($1, CONSTRAINT_CONDITION); }	
	| null_condition																	{ $$ = SimpleNullPropertySemanticAction($1, NULL_CONDITION); }

	| default_value constraint															{ $$ = DoubleDefaultConstraintPropertySemanticAction($1, $2, DEFAULT_VALUE_CONSTRAINT); }
	| constraint default_value															{ $$ = DoubleDefaultConstraintPropertySemanticAction($2, $1, DEFAULT_VALUE_CONSTRAINT); }
	| default_value null_condition														{ $$ = DoubleDefaultNullPropertySemanticAction($1, $2, NULL_CONDITION_DEFAULT_VALUE); }	
	| null_condition default_value														{ $$ = DoubleDefaultNullPropertySemanticAction($2, $1, NULL_CONDITION_DEFAULT_VALUE); }	
	| constraint null_condition															{ $$ = DoubleConstraintNullPropertySemanticAction($1, $2, NULL_CONDITION_CONSTRAINT); }
	| null_condition constraint															{ $$ = DoubleConstraintNullPropertySemanticAction($2, $1, NULL_CONDITION_CONSTRAINT); }
	
	| constraint default_value null_condition											{ $$ = TriplePropertySemanticAction($2, $1, $3); }
	| constraint null_condition default_value											{ $$ = TriplePropertySemanticAction($3, $1, $2); }	
	| default_value constraint null_condition											{ $$ = TriplePropertySemanticAction($1, $2, $3); }
	| default_value null_condition constraint											{ $$ = TriplePropertySemanticAction($1, $3, $2); }
	| null_condition constraint default_value											{ $$ = TriplePropertySemanticAction($3, $2, $1); }	
	| null_condition default_value constraint											{ $$ = TriplePropertySemanticAction($2, $3, $1); }
	;
	
type: INTEGER																			{ $$ = SimpleTypeSemanticAction(INTEGER_DATATYPE); }
	| SMALLINT																			{ $$ = SimpleTypeSemanticAction(SMALLINT_DATATYPE); }	
	| BIGINT																			{ $$ = SimpleTypeSemanticAction(BIGINT_DATATYPE); }
	| REAL																				{ $$ = SimpleTypeSemanticAction(REAL_DATATYPE); }	
	| DOUBLE 																			{ $$ = SimpleTypeSemanticAction(DOUBLE_DATATYPE); }	
	| DATE																				{ $$ = SimpleTypeSemanticAction(DATE_DATATYPE); }	
	| TIMESTAMP																			{ $$ = SimpleTypeSemanticAction(TIMESTAMP_DATATYPE); }		
	| INTERVAL																			{ $$ = SimpleTypeSemanticAction(INTERVAL_DATATYPE); }
	| TEXT																				{ $$ = SimpleTypeSemanticAction(TEXT_DATATYPE); }	
	| CHAR OPEN_PARENTHESIS INTEGER_VALUE CLOSE_PARENTHESIS								{ $$ = ComplexTypeSemanticAction(CHAR_DATATYPE, INTEGER_DATATYPE); }	
	| VARCHAR OPEN_PARENTHESIS INTEGER_VALUE CLOSE_PARENTHESIS							{ $$ = ComplexTypeSemanticAction(VARCHAR_DATATYPE, INTEGER_DATATYPE); }		
	| FLOAT OPEN_PARENTHESIS INTEGER_VALUE CLOSE_PARENTHESIS							{ $$ = ComplexTypeSemanticAction(FLOAT_DATATYPE, INTEGER_DATATYPE); }
	| TIME OPEN_PARENTHESIS INTEGER_VALUE CLOSE_PARENTHESIS								{ $$ = ComplexTypeSemanticAction(TIME_DATATYPE, INTEGER_DATATYPE); }	
	| NUMBER OPEN_PARENTHESIS INTEGER_VALUE COMA INTEGER_VALUE CLOSE_PARENTHESIS		{ $$ = DoubleComplexTypeSemanticAction(NUMBER_DATATYPE, INTEGER_DATATYPE, INTEGER_DATATYPE); }
	;

null_condition: NOT NUL																	{ $$ = NullConditionSemanticAction(NOT_NULL_CONDITION); }
	| NUL																				{ $$ = NullConditionSemanticAction(NUL_CONDITION); }	
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

constant: INTEGER																{ $$ = IntegerConstantSemanticAction($1); }
	;

%%
*/