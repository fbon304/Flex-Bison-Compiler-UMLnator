
%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer_value;
	char * string_value;
	double double_value;
	char * id;
	Token token;

	/** Non-terminals. */

	Program * program;
	Tables * tables;
	Content * content;
	ContentElement * content_element;
	Attribute * attribute;
	Properties * properties;
	LocalConstraint * local_constraint;
	Constraint * constraint;
	Type * type;
	DefaultValue * default_value;
	NullCondition * null_condition;
	ConstraintValue * constraint_value;
	BooleanExpression * boolean_expression;
	BooleanValue * boolean_value;
	BooleanFactor * boolean_factor;
	Expression * expression;
	Function * function;
	Action * action;
	OnAction * on_action;
	CheckConstraint * check_constraint;
	Factor * factor;
	IsCondition * is_condition;
	TablesList * tables_list;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */

%destructor { releaseIsCondition($$); } <is_condition>
%destructor { releaseBooleanValue($$); } <boolean_value>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseBooleanExpression($$); } <boolean_expression>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseAction($$); } <action>
%destructor { releaseOnAction($$); } <on_action>
%destructor { releaseConstraintValue($$); } <constraint_value>
%destructor { releaseConstraint($$); } <constraint>
%destructor { releaseLocalConstraint($$); } <local_constraint>
%destructor { releaseFunction($$); } <function>
%destructor { releaseDefaultValue($$); } <default_value>
%destructor { releaseNullCondition($$); } <null_condition>
%destructor { releaseType($$); } <type>
%destructor { releaseProperties($$); } <properties>
%destructor { releaseAttribute($$); } <attribute>
%destructor { releaseContent($$); } <content>
%destructor { releaseContentElement($$); } <content_element>
%destructor { releaseTables($$); } <tables>
%destructor { releaseTablesList($$); } <tables_list>
%destructor { releaseBooleanFactor($$); } <boolean_factor>

/** Terminals. */

%token <id> ID
%token <integer_value> INTEGER_VALUE
%token <double_value> DOUBLE_VALUE
%token <string_value> STRING_VALUE

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
%token <token> REFERENCES
%token <token> DEFAULT
%token <token> INTEGER
%token <token> SMALLINT
%token <token> BIGINT
%token <token> REAL
%token <token> DOUBLE
%token <token> DATE
%token <token> TIMESTAMP
%token <token> INTERVAL
%token <token> TEXT
%token <token> SERIAL
%token <token> CHAR
%token <token> VARCHAR
%token <token> NUMBER
%token <token> FLOAT
%token <token> TIME
%token <token> CURRENT_TIMESTAMP
%token <token> AUTO_INCREMENT
%token <token> CURRENT_DATE
%token <token> CURRENT_TIME
%token <token> LOCALTIME
%token <token> LOCALTIMESTAMP
%token <token> GEN_RANDOM_UUID
%token <token> UUID_GENERATE_V4
%token <token> OR
%token <token> AND
%token <token> EQUALS
%token <token> NOT_EQUALS
%token <token> GREATER_THAN
%token <token> GREATER_THAN_EQUALS 
%token <token> LESS_THAN
%token <token> LESS_THAN_EQUALS
%token <token> IS
%token <token> ISNOT
%token <token> ISNULL
%token <token> NOTNULL
%token <token> DISTINCT_FROM
%token <token> TRUE
%token <token> FALSE
%token <token> ON
%token <token> DELETE
%token <token> UPDATE
%token <token> CASCADE
%token <token> SET
%token <token> NO_ACTION
%token <token> RESTRICT

%token <token> UNKNOWN

/** Non-terminals. */

%type <program> program
%type <tables_list> tables_list
%type <tables> tables
%type <content> content
%type <content_element> content_element
%type <attribute> attribute
%type <properties> properties
%type <local_constraint> local_constraint
%type <constraint> constraint
%type <default_value> default_value
%type <type> type
%type <null_condition> null_condition
%type <expression> expression
%type <constraint_value> constraint_value
%type <boolean_expression> boolean_expression
%type <function> function
%type <check_constraint> check_constraint
%type <action> action
%type <on_action> on_action
%type <boolean_value> boolean_value
%type <factor> factor
%type <is_condition> is_condition
%type <boolean_factor> boolean_factor

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
/*%left ADD SUB
%left MUL DIV*/

%left OR AND 
%nonassoc EQUALS NOT_EQUALS LESS_THAN GREATER_THAN LESS_THAN_EQUALS GREATER_THAN_EQUALS
%nonassoc DISTINCT_FROM
%nonassoc IS ISNOT ISNULL NOTNULL
%right NOT

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: tables_list																{ $$ = TablesListProgramSemanticAction(currentCompilerState(), $1); }
	;

tables_list: tables																	{ $$ = TableListSingleTableSemanticAction($1); }
	| tables_list tables															{ $$ = TableListMultiTableSemanticAction($1, $2); }
	;

tables: CREATE TABLE ID OPEN_PARENTHESIS content CLOSE_PARENTHESIS SEMICOLON		{ $$ = ContentTablesSemanticAction($3, $5); }
	;

content: content_element															{ $$ = SimpleContentSemanticAction($1); }										
	| content COMA content_element													{ $$ = MultiContentSemanticAction($1, $3); }										
	;

content_element: attribute															{ $$ = ContentElementAttributeSemanticAction($1); }										
	| constraint																	{ $$ = ContentElementConstraintSemanticAction($1); }
	;
	
attribute: ID type																	{ $$ = AttributeTypeSemanticAction($1, $2); }					
	| ID type properties															{ $$ = AttributeTypePropertiesSemanticAction($1, $2, $3); }	
	;

constraint: CONSTRAINT ID constraint_value											{ $$ = NamedConstraintValueSemanticAction($2, $3); }
	| constraint_value																{ $$ = UnnamedConstraintValueSemanticAction($1); }	
	;

properties:  default_value															{ $$ = SimpleDefaultPropertySemanticAction($1, DEFAULT_VALUE); }	
	| local_constraint																{ $$ = SimpleConstraintPropertySemanticAction($1, CONSTRAINT_CONDITION); }	
	| null_condition																{ $$ = SimpleNullPropertySemanticAction($1, NULL_CONDITION); }

	| default_value local_constraint												{ $$ = DoubleDefaultConstraintPropertySemanticAction($1, $2, DEFAULT_VALUE_CONSTRAINT); }
	| local_constraint default_value												{ $$ = DoubleDefaultConstraintPropertySemanticAction($2, $1, DEFAULT_VALUE_CONSTRAINT); }
	| default_value null_condition													{ $$ = DoubleDefaultNullPropertySemanticAction($1, $2, NULL_CONDITION_DEFAULT_VALUE); }	
	| null_condition default_value													{ $$ = DoubleDefaultNullPropertySemanticAction($2, $1, NULL_CONDITION_DEFAULT_VALUE); }	
	| local_constraint null_condition												{ $$ = DoubleConstraintNullPropertySemanticAction($1, $2, NULL_CONDITION_CONSTRAINT); }
	| null_condition local_constraint												{ $$ = DoubleConstraintNullPropertySemanticAction($2, $1, NULL_CONDITION_CONSTRAINT); }
	
	| local_constraint default_value null_condition									{ $$ = TriplePropertySemanticAction($2, $1, $3); }
	| local_constraint null_condition default_value									{ $$ = TriplePropertySemanticAction($3, $1, $2); }	
	| default_value local_constraint null_condition									{ $$ = TriplePropertySemanticAction($1, $2, $3); }
	| default_value null_condition local_constraint									{ $$ = TriplePropertySemanticAction($1, $3, $2); }
	| null_condition local_constraint default_value									{ $$ = TriplePropertySemanticAction($3, $2, $1); }	
	| null_condition default_value local_constraint									{ $$ = TriplePropertySemanticAction($2, $3, $1); }
	;
	
type: INTEGER																		{ $$ = SimpleTypeSemanticAction(INTEGER_DATATYPE); }
	| SMALLINT																		{ $$ = SimpleTypeSemanticAction(SMALLINT_DATATYPE); }	
	| BIGINT																		{ $$ = SimpleTypeSemanticAction(BIGINT_DATATYPE); }
	| REAL																			{ $$ = SimpleTypeSemanticAction(REAL_DATATYPE); }	
	| DOUBLE 																		{ $$ = SimpleTypeSemanticAction(DOUBLE_DATATYPE); }	
	| DATE																			{ $$ = SimpleTypeSemanticAction(DATE_DATATYPE); }	
	| TIMESTAMP																		{ $$ = SimpleTypeSemanticAction(TIMESTAMP_DATATYPE); }		
	| INTERVAL																		{ $$ = SimpleTypeSemanticAction(INTERVAL_DATATYPE); }
	| TEXT																			{ $$ = SimpleTypeSemanticAction(TEXT_DATATYPE); }
	| SERIAL																		{ $$ = SimpleTypeSemanticAction(SERIAL_DATATYPE); }	
	| CHAR OPEN_PARENTHESIS INTEGER_VALUE CLOSE_PARENTHESIS							{ $$ = ComplexTypeSemanticAction(CHAR_DATATYPE, INTEGER_DATATYPE); }	
	| VARCHAR OPEN_PARENTHESIS INTEGER_VALUE CLOSE_PARENTHESIS						{ $$ = ComplexTypeSemanticAction(VARCHAR_DATATYPE, INTEGER_DATATYPE); }		
	| FLOAT OPEN_PARENTHESIS INTEGER_VALUE CLOSE_PARENTHESIS						{ $$ = ComplexTypeSemanticAction(FLOAT_DATATYPE, INTEGER_DATATYPE); }
	| TIME OPEN_PARENTHESIS INTEGER_VALUE CLOSE_PARENTHESIS							{ $$ = ComplexTypeSemanticAction(TIME_DATATYPE, INTEGER_DATATYPE); }	
	| NUMBER OPEN_PARENTHESIS INTEGER_VALUE COMA INTEGER_VALUE CLOSE_PARENTHESIS	{ $$ = DoubleComplexTypeSemanticAction(NUMBER_DATATYPE, INTEGER_DATATYPE, INTEGER_DATATYPE); }
	;

null_condition: NOT NUL																{ $$ = NullConditionSemanticAction(NOT_NULL_CONDITION); }
	| NUL																			{ $$ = NullConditionSemanticAction(NUL_CONDITION); }	
	;

default_value: DEFAULT INTEGER_VALUE												{ $$ = DefaultValueIntegerTerminalSemanticAction($2); }
	| DEFAULT DOUBLE_VALUE															{ $$ = DefaultValueDoubleTerminalSemanticAction($2); }
	| DEFAULT STRING_VALUE															{ $$ = DefaultValueStringTerminalSemanticAction($2); }	
	| DEFAULT function																{ $$ = DefaultValueNonTerminalSemanticAction($2); }	
	;

function: CURRENT_TIMESTAMP 														{ $$ = FunctionSemanticAction(CURRENT_TIMESTAMP_FUNCTION);}	
	| AUTO_INCREMENT																{ $$ = FunctionSemanticAction(AUTO_INCREMENT_FUNCTION);}
	| CURRENT_DATE																	{ $$ = FunctionSemanticAction(CURRENT_DATE_FUNCTION);}	
	| CURRENT_TIME																	{ $$ = FunctionSemanticAction(CURRENT_TIME_FUNCTION);}	
	| LOCALTIME																		{ $$ = FunctionSemanticAction(LOCALTIME_FUNCTION);}	
	| LOCALTIMESTAMP																{ $$ = FunctionSemanticAction(LOCALTIMESTAMP_FUNCTION);}
	| GEN_RANDOM_UUID OPEN_PARENTHESIS CLOSE_PARENTHESIS							{ $$ = FunctionSemanticAction(GEN_RANDOM_UUID_OPEN_AND_CLOSE_PARENTHESIS_FUNCTION);}
	| UUID_GENERATE_V4 OPEN_PARENTHESIS CLOSE_PARENTHESIS							{ $$ = FunctionSemanticAction(UUID_GENERATE_V4_OPEN_AND_CLOSE_PARENTHESIS_FUNCTION);}
	;

local_constraint: PRIMARY KEY														{ $$ = SimpleLocalConstraintSemanticAction(PRIMARY_KEY_LCT);}					
	| UNIQUE																		{ $$ = SimpleLocalConstraintSemanticAction(UNIQUE_LCT);}			
	| REFERENCES ID on_action														{ $$ = NameOnActionLocalConstraintSemanticAction($2, $3);}	
	| REFERENCES ID OPEN_PARENTHESIS ID CLOSE_PARENTHESIS on_action 				{ $$ = DoubleNameOnActionLocalConstraintSemanticAction($2, $4, $6);}
	| check_constraint																{ $$ = CheckLocalConstraintSemanticAction($1);}			
	;

constraint_value: check_constraint																										{ $$ = CheckConstraintValueSemanticAction($1); }	
	| PRIMARY KEY OPEN_PARENTHESIS expression CLOSE_PARENTHESIS																			{ $$ = SingleExpressionConstraintValueSemanticAction($4, PRIMARY_KEY_CONSTRAINT_TYPE); }
	| UNIQUE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS																				{ $$ = SingleExpressionConstraintValueSemanticAction($3, UNIQUE_CONSTRAINT_TYPE); }					
	| FOREIGN KEY OPEN_PARENTHESIS expression CLOSE_PARENTHESIS REFERENCES ID on_action													{ $$ = SingleForeignConstraintValueSemanticAction($4, $7, $8); }
	| FOREIGN KEY OPEN_PARENTHESIS expression CLOSE_PARENTHESIS REFERENCES ID OPEN_PARENTHESIS expression CLOSE_PARENTHESIS on_action  	{ $$ = DoubleForeignConstraintValueSemanticAction($4, $7, $9, $11); }
	; 

on_action: ON DELETE action															{ $$ = OnActionSemanticAction($3, DELETE_ON_ACTION); }
	| ON UPDATE action																{ $$ = OnActionSemanticAction($3, UPDATE_ON_ACTION); }	
	| ON DELETE action ON UPDATE action												{ $$ = DoubleActionSemanticAction($3, $6); }
	| ON UPDATE action ON DELETE action												{ $$ = DoubleActionSemanticAction($6, $3); }
	| %empty																		{ $$ = EmptyActionSemanticAction(); }	
	;

action: CASCADE																		{ $$ = ActionSemanticAction(CASCADE_ACTION); }	
	| SET NUL																		{ $$ = ActionSemanticAction(SET_NUL_ACTION); }	
	| SET DEFAULT																	{ $$ = ActionSemanticAction(SET_DEFAULT_ACTION); }		
	| NO_ACTION																		{ $$ = ActionSemanticAction(NO_ACTION_ACTION); }		
	| RESTRICT																		{ $$ = ActionSemanticAction(RESTRICT_ACTION); }		
	;

expression: ID 																		{ $$ = IdExpressionSemanticAction($1);}	
	| ID COMA expression															{ $$ = ComplexExpressionSemanticAction($1, $3);}	
	;

check_constraint: CHECK OPEN_PARENTHESIS boolean_expression CLOSE_PARENTHESIS		{ $$ = BooleanExpressionCheckConstraint($3);}
	;

boolean_expression: boolean_expression AND boolean_expression						{ $$ = DoubleBooleanExpressionSemanticAction($1, $3, AND_BOOLEANTYPE);}	
	| boolean_expression OR boolean_expression										{ $$ = DoubleBooleanExpressionSemanticAction($1, $3, OR_BOOLEANTYPE);}	
	| boolean_expression EQUALS boolean_expression /* = */							{ $$ = DoubleBooleanExpressionSemanticAction($1, $3, EQUALS_BOOLEANTYPE);}	
	| boolean_expression NOT_EQUALS boolean_expression /* <> o != */				{ $$ = DoubleBooleanExpressionSemanticAction($1, $3, NOT_EQUALS_BOOLEANTYPE);}
	| boolean_expression LESS_THAN boolean_expression								{ $$ = DoubleBooleanExpressionSemanticAction($1, $3, LESS_THAN_BOOLEANTYPE);}
	| boolean_expression GREATER_THAN boolean_expression							{ $$ = DoubleBooleanExpressionSemanticAction($1, $3, GREATER_THAN_BOOLEANTYPE);}	
	| boolean_expression GREATER_THAN_EQUALS boolean_expression						{ $$ = DoubleBooleanExpressionSemanticAction($1, $3, GREATER_THAN_EQUALS_BOOLEANTYPE);}
	| boolean_expression LESS_THAN_EQUALS boolean_expression     					{ $$ = DoubleBooleanExpressionSemanticAction($1, $3, LESS_THAN_EQUALS);}
	| boolean_expression NOTNULL													{ $$ = SimpleBooleanExpressionSemanticAction($1, NOTNULL_BOOLEANTYPE);}
	| boolean_expression ISNULL														{ $$ = SimpleBooleanExpressionSemanticAction($1, ISNULL_BOOLEANTYPE);}		
	| boolean_factor																{ $$ = BooleanFactorBooleanExpressionSemanticAction($1); }
	;


boolean_factor: OPEN_PARENTHESIS boolean_expression CLOSE_PARENTHESIS				{ $$ = BooleanExpressionFactorSemanticAction($2, BOOLEAN_EXPRESSION_PARENTHESIS_TYPE);}
	| boolean_factor is_condition boolean_value										{ $$ = TriplePointerBooleanFactorSemanticAction($1, $2, $3);}	
	| boolean_factor is_condition NUL												{ $$ = IsConditionBooleanFactorSemanticAction($1, $2);}	
	| NOT boolean_expression														{ $$ = BooleanExpressionFactorSemanticAction($2, NOT_BOOLEAN_EXPRESSION);}
	| boolean_value																	{ $$ = BooleanValueBooleanFactorSemanticAction($1);}
	| factor is_condition DISTINCT_FROM factor										{ $$ = FactorsBooleanFactorSemanticAction($1, $2, $4);}
	| factor																		{ $$ = FactorBooleanFactorSemanticAction($1);}
	;

factor: ID 																			{ $$ = StringFactorSemanticAction($1, ID_FACTOR_TYPE);}
	| INTEGER_VALUE																	{ $$ = IntegerFactorSemanticAction($1);}
	| DOUBLE_VALUE																	{ $$ = DoubleFactorSemanticAction($1);}
	| STRING_VALUE																	{ $$ = StringFactorSemanticAction($1, STRING_FACTOR_TYPE);}
	;

boolean_value: TRUE																	{ $$ = BooleanValueSemanticAction(BOOLEAN_TRUE);}		
	| FALSE																			{ $$ = BooleanValueSemanticAction(BOOLEAN_FALSE);}	
	;

is_condition: IS																	{ $$ = IsConditionSemanticAction(IS_CONDITION);}
	| ISNOT																		{ $$ = IsConditionSemanticAction(IS_NOT_CONDITION);}		
	;

%%