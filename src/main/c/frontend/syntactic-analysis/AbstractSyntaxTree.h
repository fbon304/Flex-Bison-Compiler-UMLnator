		#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum TablesListType TablesListType;
typedef enum TablesType TablesType;
typedef enum ContentType ContentType;
typedef enum AttributeType AttributeType;
typedef enum DataTypeType DataTypeType;
typedef enum NullConditionType NullConditionType;
typedef enum DefaultValueType DefaultValueType; 
typedef enum ConstraintValueType ConstraintValueType;
typedef enum PropertiesType PropertiesType;
typedef enum FunctionType FunctionType;
typedef enum ExpressionType ExpressionType;
typedef enum LocalConstraintType LocalConstraintType;
typedef enum ConstraintType ConstraintType;
typedef enum BooleanExpressionType BooleanExpressionType;
typedef enum BooleanFactorType BooleanFactorType;
typedef enum BooleanType BooleanType;
typedef enum IsConditionType IsConditionType;
typedef enum OnActionType OnActionType;
typedef enum ActionType ActionType;
typedef enum FactorType FactorType;
typedef enum ContentElementType ContentElementType;
typedef enum BooleanFactorType BooleanFactorType;

typedef struct Program Program;
typedef struct TablesList TablesList; 
typedef struct Tables Tables;
typedef struct Content Content;
typedef struct Attribute Attribute;
typedef struct LocalConstraint LocalConstraint;
typedef struct Constraint Constraint;
typedef struct Type Type;
typedef struct Properties Properties;
typedef struct DefaultValue DefaultValue;
typedef struct NullCondition NullCondition;
typedef struct Expression Expression;
typedef struct ConstraintValue ConstraintValue;
typedef struct BooleanExpression BooleanExpression;
typedef struct Function Function;
typedef struct Factor Factor;
typedef struct OnAction OnAction;
typedef struct CheckConstraint CheckConstraint;
typedef struct BooleanValue BooleanValue;
typedef struct IsCondition IsCondition;
typedef struct Action Action;
typedef struct ContentElement ContentElement;
typedef struct BooleanFactor BooleanFactor;

/**
 * Node types for the Abstract Syntax Tree (AST).
**/

enum TablesListType {
	SINGULAR,
	LIST
};

enum ContentType {
	ELEMENT,
	CONTENT_LIST
};

enum ContentElementType {
	ATTRIBUTE_TYPE,
	CONSTRAINT_TYPE
};

enum AttributeType {
	COLUMN,
	COLUMN_WITH_PROPERTIES
};

enum ConstraintType {
	NAMED_CONSTRAINT,
	UNNAMED_CONSTRAINT
};

enum PropertiesType {
	DEFAULT_VALUE,
	CONSTRAINT_CONDITION,
	NULL_CONDITION,
	NULL_CONDITION_DEFAULT_VALUE,
	DEFAULT_VALUE_CONSTRAINT,
	NULL_CONDITION_CONSTRAINT,
	COMPLETE
};

enum DataTypeType {
	INTEGER_DATATYPE, 
	SMALLINT_DATATYPE,
	BIGINT_DATATYPE,
	REAL_DATATYPE,
	DOUBLE_DATATYPE,
	DATE_DATATYPE,
	TIMESTAMP_DATATYPE,
	INTERVAL_DATATYPE,
	TEXT_DATATYPE,
	SERIAL_DATATYPE,
	CHAR_DATATYPE,
	VARCHAR_DATATYPE,
	FLOAT_DATATYPE,
	TIME_DATATYPE,
	NUMBER_DATATYPE,
	UNKNOWN_SQL_DATATYPE
};

enum NullConditionType {
	NOT_NULL_CONDITION,
	NUL_CONDITION
};

enum DefaultValueType {
	INTEGER_DEFAULT,
	DOUBLE_DEFAULT,
	STRING_DEFAULT,
	FUNCTION
};

enum FunctionType {
	CURRENT_TIMESTAMP_FUNCTION, 
	AUTO_INCREMENT_FUNCTION,
	CURRENT_DATE_FUNCTION,
	CURRENT_TIME_FUNCTION,
	LOCALTIME_FUNCTION,
	LOCALTIMESTAMP_FUNCTION,
	GEN_RANDOM_UUID_OPEN_AND_CLOSE_PARENTHESIS_FUNCTION,
	UUID_GENERATE_V4_OPEN_AND_CLOSE_PARENTHESIS_FUNCTION
};

enum LocalConstraintType {
	PRIMARY_KEY_LCT,
	UNIQUE_LCT,
	FOREIGN_KEY_LCT,
	FOREING_KEY_DOUBLE_NAME_LCT,
	CHECK_LCT
};


enum ConstraintValueType {
	CHECK_CONSTRAINT_TYPE,
    PRIMARY_KEY_CONSTRAINT_TYPE,
	UNIQUE_CONSTRAINT_TYPE,
	FOREIGN_KEY_CONSTRAINT_TYPE,
	FOREIGN_KEY_DOUBLE_EXPRESSION_CONSTRAINT_TYPE,
};

enum OnActionType {
	DELETE_ON_ACTION,
	UPDATE_ON_ACTION,
	ON_DELETE_ON_UPDATE_ON_ACTION,
	LAMBDA_ON_ACTION
};

enum ActionType{
	CASCADE_ACTION,
	SET_NUL_ACTION,
	SET_DEFAULT_ACTION,
	NO_ACTION_ACTION,
	RESTRICT_ACTION
};

enum ExpressionType {
	SIMPLE_EXPRESSION,
	COMPLEX_EXPRESSION
};

enum BooleanExpressionType {
	AND_BOOLEANTYPE,
	OR_BOOLEANTYPE,
	EQUALS_BOOLEANTYPE,
	NOT_EQUALS_BOOLEANTYPE,
	LESS_THAN_BOOLEANTYPE,
	GREATER_THAN_BOOLEANTYPE,
	GREATER_THAN_EQUALS_BOOLEANTYPE,
	LESS_THAN_EQUALS_BOOLEANTYPE,
	NOTNULL_BOOLEANTYPE,
	ISNULL_BOOLEANTYPE,
	BOOLEAN_FACTOR_BOOLEANTYPE
};

enum BooleanFactorType {
	BOOLEAN_EXPRESSION_PARENTHESIS_TYPE,
	NOT_BOOLEAN_EXPRESSION,
	FACTOR_CONDITION_FACTOR,
	FACTOR_CONDITION_NUL,
	DISTINCT_FROM_FACTORS,
	FACTOR_TYPE
};

enum FactorType {
	ID_FACTOR_TYPE,
	INTEGER_FACTOR_TYPE,
	DOUBLE_FACTOR_TYPE,
	STRING_FACTOR_TYPE,
	BOOLEAN_TRUE,
	BOOLEAN_FALSE
};

enum IsConditionType {
	IS_CONDITION,
	IS_NOT_CONDITION
};

struct IsCondition {
	IsConditionType type;
};

struct Factor {
	union {
		char * string;
		double double_factor;
		int integer_factor;
	};
	FactorType type;
};

struct BooleanFactor {
	union {
		BooleanExpression * booleanExpression;
		Factor * factor;
		struct {
			Factor * factor_left;
			IsCondition * isCondition_with_two_factors;
			Factor * factor_right;
		};
		struct {
			Factor * factor_with_is_condition;
			IsCondition * is_condition_with_factor;	
		};
	};
	BooleanFactorType type;
};

struct BooleanExpression {
	union {
		struct {
			BooleanExpression * boolean_expression_left;
			BooleanExpression * boolean_expression_right;
		};
		BooleanExpression * unique_boolean_expression;
		BooleanFactor * boolean_factor;
	};
	BooleanExpressionType type;
};

struct CheckConstraint {
	BooleanExpression * booleanExpression;
};

struct Expression {
	char * id;
	Expression * expression;
	ExpressionType type;
};


struct Action {
	ActionType type;
};

struct OnAction {
	union {
		Action * action;
		struct {
			Action * deleteAction;
			Action * updateAction;
		};
	};
	OnActionType type;
};

struct ConstraintValue {
	union {
		CheckConstraint * checkConstraint;
		Expression * expression;
		struct {
			char * id;
			Expression * singleExpression;
			OnAction * onActionSingle;
		};
		struct {
			char * idComplex;
			Expression * mainExpression;
			Expression * secondExpression;
			OnAction * onActionDouble;
		};
	};
	ConstraintValueType type;
};

struct LocalConstraint {
	union {
		struct {
			char * id;
			OnAction * onAction;
		};
		struct {
			char * id1;
			char * id2;
			OnAction * onActionComplex;
		};
		CheckConstraint * checkConstraint;
	};
	LocalConstraintType type;
};

struct Function {
	FunctionType functionType;
};

struct DefaultValue {
	union {
		int integer_value;
		double double_value;
		char * string_value;
		Function * function; 
	};
	DefaultValueType type;
};


struct NullCondition {
	NullConditionType type;
};

struct Type {
	int param1;
	int param2;
	DataTypeType type;
};

struct Properties {
	union {
		DefaultValue * defaultValue;
		LocalConstraint * constraint;
		NullCondition * nullCondition;
		struct {
			DefaultValue * defaultValueDC;
			LocalConstraint * constraintDC;
		};
		struct {
			DefaultValue * defaultValueDN;
			NullCondition * nullConditionDN;
		};
		struct {
			LocalConstraint * constraintCN;
			NullCondition * nullConditionCN;
		};
		struct {
			LocalConstraint * constraintCDN;
			DefaultValue * defaultValueCDN;
			NullCondition * nullConditionCDN;
		};
	};
	PropertiesType type;
};

struct Constraint {
	union {
		struct {
			char * id;
			ConstraintValue * constraintValue;
		};
		ConstraintValue * singleConstraintValue;
	};
	ConstraintType type;
	
};

struct Attribute {
	union {
		struct {
			char * id;              
    		Type * datatype;
		};
		struct {
			char * p_id;              
    		Type * p_type;
			Properties * properties;
		};
	};
	AttributeType type;
};

struct ContentElement {
	union {
		Attribute * attribute;
		Constraint * constraint;
	};
	ContentElementType contentElementType;
};

struct Content {
	Content * content;
	ContentElement * content_element;
	ContentType type;
};

struct Tables {
	char * id;
	Content * content;
};

struct TablesList {
	TablesList * tablesList;
	Tables * tables;
	TablesListType type;
};

struct Program {
	TablesList * tablesList;
};

/**
 * Node recursive destructors.
 */

void releaseIsCondition(IsCondition * isCondition);
void releaseFactor(Factor * factor);
void releaseBooleanFactor(BooleanFactor * booleanFactor);
void releaseBooleanExpression(BooleanExpression * booleanExpression);
void releaseExpression(Expression * expression);
void releaseAction(Action * action);
void releaseOnAction(OnAction * onAction);
void releaseConstraintValue(ConstraintValue * constraintValue);
void releaseConstraint(Constraint * constraint);
void releaseLocalConstraint(LocalConstraint * localConstraint);
void releaseFunction(Function * function);
void releaseDefaultValue(DefaultValue * defaultValue);
void releaseNullCondition(NullCondition * nullCondition);
void releaseType(Type * type);
void releaseProperties(Properties * properties);
void releaseAttribute(Attribute * attribute);
void releaseContentElement(ContentElement * contentElement);
void releaseContent(Content * content);
void releaseTablesList(TablesList *tablesList);
void releaseTables(Tables *tables);
void releaseProgram(Program * program);

#endif