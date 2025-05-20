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
typedef enum TablesType TablesType;
typedef enum ContentType ContentType;
typedef enum AttributesType AttributesType;
typedef enum AttributeType AttributeType;
typedef enum DataTypeType DataTypeType;
typedef enum NullConditionType NullConditionType;
typedef enum DefaultValueType DefaultValueType; 
typedef enum ConstraintValueType ConstraintValueType;
typedef enum PropertiesType PropertiesType;
typedef enum FunctionType FunctionType;
typedef enum ExpressionType ExpressionType;
typedef enum LocalConstraintType LocalConstraintType;
typedef enum ConstraintsType ConstraintsType;
typedef enum ConstraintType ConstraintType;
typedef enum BooleanExpressionType BooleanExpressionType;
typedef enum BooleanType BooleanType;
typedef enum IsConditionType IsConditionType;
typedef enum OnActionType OnActionType;
typedef enum ActionType ActionType;
typedef enum FactorType FactorType;

typedef struct Program Program;
typedef struct Tables Tables;
typedef struct Content Content;
typedef struct Attributes Attributes;
typedef struct Attribute Attribute;
typedef struct LocalConstraint LocalConstraint;
typedef struct Constraint Constraint;
typedef struct Constraints Constraints;
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

/**
 * Node types for the Abstract Syntax Tree (AST).
**/

enum TablesType {
	MULTIPLE_TABLES,
	CONTENT
};

enum ContentType {
	ATTRIBUTES,
	CONSTRAINTS,
	ATTRIBUTES_CONSTRAINTS
};

enum AttributesType {
	ATTRIBUTE,
	MULTIPLE_ATTRIBUTE
};

enum AttributeType {
	COLUMN,
	COLUMN_WITH_PROPERTIES
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

enum ConstraintValueType {
	CHECK_CONSTRAINT_TYPE,
    PRIMARY_KEY_CONSTRAINT_TYPE,
	UNIQUE_CONSTRAINT_TYPE,
	FOREIGN_KEY_CONSTRAINT_TYPE,
	FOREIGN_KEY_DOUBLE_EXPRESSION_CONSTRAINT_TYPE,
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

enum ExpressionType {
	SIMPLE_EXPRESSION,
	COMPLEX_EXPRESSION
};

enum LocalConstraintType {
	PRIMARY_KEY_LCT,
	UNIQUE_LCT,
	FOREIGN_KEY_LCT,
	CHECK_LCT,
	FOREING_KEY_ATTRIBUTE_LCT
};

enum ConstraintType {
	NAMED_CONSTRAINT,
	UNNAMED_CONSTRAINT
};

enum BooleanExpressionType {
	BETWEEN_PARENTHESIS,
	AND_BOOLEANTYPE,
	OR_BOOLEANTYPE,
	EQUALS_BOOLEANTYPE,
	NOT_EQUALS_BOOLEANTYPE,
	LESS_THAN_BOOLEANTYPE,
	GREATER_THAN_BOOLEANTYPE,
	GREATER_THAN_EQUALS_BOOLEANTYPE,
	LESS_THAN_EQUALS_BOOLEANTYPE,
	THREE_POINTERS_BOOLEANTYPE,
	NUL_BOOLEANTYPE,
	NOTNULL_BOOLEANTYPE,
	ISNULL_BOOLEANTYPE,
	NOT_BOOLEANTYPE,
	BOOLEAN_VALUE_BOOLEANTYPE,
	DISTINCT_FROM_BOOLEANTYPE
};

enum FactorType {
	ID_FACTOR_TYPE,
	INTEGER_FACTOR_TYPE,
	DOUBLE_FACTOR_TYPE,
	STRING_FACTOR_TYPE
};

enum ConstraintsType {
	SINGLE_CONSTRAINT,
	DOUBLE_CONSTRAINT
};

enum BooleanType {
	BOOLEAN_TRUE,
	BOOLEAN_FALSE
};

enum IsConditionType {
	IS_CONDITION,
	IS_NOT_CONDITION
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

struct Factor {
	union {
		char * string;
		double double_factor;
		int integer_factor
	};
	FactorType type;
};

struct IsCondition {
	IsConditionType type;
};

struct BooleanValue {
	BooleanType type;
};

struct BooleanExpression {
	union {
		struct {
			BooleanExpression * boolean_expression_left;
			BooleanExpression * boolean_expression_right;
		};

		struct {
			BooleanExpression * boolean_expression_three_pointers;
			IsCondition * is_condition_three_pointers;
			BooleanValue * boolean_value;
		};

		struct {
			BooleanExpression * boolean_expression_with_is_condition;
			IsCondition * is_condition_with_boolean_expression;	
		};

		BooleanExpression * unique_boolean_expression;
		BooleanValue * unique_boolean_value;

		struct {
			Factor * factor_left;
			IsCondition * isCondition_with_two_factors;
			Factor * factor_right;
		};
	};
	BooleanExpressionType type;
};

struct CheckConstraint {
	BooleanExpression * booleanExpression;
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
			OnAction * onAction;
		};
		CheckConstraint * checkConstraint;
	};
	LocalConstraintType type;
};

struct Expression {
	char * id;
	Expression * expression;
	ExpressionType type;
};

struct Function {
	FunctionType functionType;
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

struct Constraints {
	union {
		struct {
			Constraint * constraint;
		};
		struct {
			Constraint * constraint1;
			Constraint * constraint2;
		};
	};
	ConstraintsType type;
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

struct Type {
	int param1;
	int param2;
	DataTypeType type;
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

struct Attributes {
	union {
		Attribute * head;
		struct {
			Attribute * attribute1;
			Attribute * attribute2;	
		};
	};
	AttributesType type;
};

struct Content {
	union {
		Attributes * onlyAttributes;
		Constraints * onlyConstraints;
		struct {
			Attributes * attributes;
			Constraints * constraints;
		};
	};
	ContentType type;
};

struct Tables {
	union {
		struct {
			Tables * tables1;
			Tables * tables2;
		};
		struct {
			char * id;
			Content * content;
		};
	};
	TablesType type;
};

struct Program {
	Tables * tables;
};

/**
 * Node recursive destructors.
 */

void releaseIsCondition(IsCondition * isCondition);
void releaseBooleanValue(BooleanValue * booleanValue);
void releaseFactor(Factor * factor);
void releaseBooleanExpression(BooleanExpression * booleanExpression);
void releaseExpression(Expression * expression);
void releaseAction(Action * action);
void releaseOnAction(OnAction * onAction);
void releaseConstraintValue(ConstraintValue * constraintValue);
void releaseConstraint(Constraint * constraint);
void releaseConstraints(Constraints *constraints);
void releaseLocalConstraint(LocalConstraint * localConstraint);
void releaseFunction(Function * function);
void releaseDefaultValue(DefaultValue * defaultValue);
void releaseNullCondition(NullCondition * nullCondition);
void releaseType(Type * type);
void releaseProperties(Properties * properties);
void releaseAttribute(Attribute * attribute);
void releaseAttributes(Attributes *attributes);
void releaseContent(Content * content);
void releaseTables(Tables *tables);
void releaseProgram(Program * program);
void releaseOnAction(OnAction * onAction);
void releaseAction(Action* action);
void releaseFactor(Factor * factor);
void releaseIsCondition(IsCondition * isCondition);

#endif