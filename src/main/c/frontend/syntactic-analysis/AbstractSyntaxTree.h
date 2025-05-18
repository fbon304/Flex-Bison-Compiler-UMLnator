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

//typedef enum ExpressionType ExpressionType;
//typedef enum FactorType FactorType;

typedef enum TablesType TablesType;
typedef enum ContentType ContentType;
typedef enum AttributesType AttributesType;
typedef enum AttributesType AttributesType;
typedef enum DataTypeType DataTypeType;
typedef enum ConstraintValueType ConstraintValueType;

//typedef struct Factor Factor;
//typedef struct Constant Constant;

typedef struct Program Program;
typedef struct Tables Tables;
typedef struct Content Content;
typedef struct Attributes Attributes;
typedef struct Attribute Attribute;
typedef struct Constraint Constraint;
typedef struct Constraints Constraints;
typedef struct Type Type;
typedef struct Properties Properties;
typedef struct DefaultValue DefaultValue;
typedef struct NullCondition NullCondition;
typedef struct Expression Expression;
typedef struct ConstraintValue ConstraintValue;
typedef struct BooleaExpression BooleanExpression;

/**
 * Node types for the Abstract Syntax Tree (AST).
/*
enum ExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION
};

enum FactorType {
	CONSTANT,
	EXPRESSION
};

struct Constant {
	int value;
};

struct Factor {
	union {
		Constant * constant;
		Expression * expression;
	};
	FactorType type;
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
	};	
	ExpressionType type;

};
*/

enum ConstraintValueType {
	EXPRESSION,
    BOOLEAN_EXPRESSION
};

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
	CONSTRAINT,
	NULL_CONDITION,
	NULL_CONDITION_DEFAULT_VALUE,
	DEFAULT_VALUE_CONSTRAINT,
	COMPLETE
};

enum DataTypeType {
	INTEGER, 
	SMALLINT,
	BIGINT,
	REAL,
	DOUBLE,
	DATE,
	TIMESTAMP,
	INTERVAL
};

enum ExpressionType {
	AND, 
	OR,
	NOT,
	
};

struct Type {
	char * name;
};

struct ConstraintValue {
	union {
		BooleanExpression * booleanExpression;
		Expression * expression;
	};
	ConstraintValueType type;
};

struct Constraint {
   char * name;
   struct ConstraintValue * value;
   struct Constraint * next;
};

struct Constraints {
    Constraint *first;
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

/*void releaseConstant(Constant * constant);
void releaseFactor(Factor * factor);*/

void releaseExpression(Expression * expression); // va?
void releaseType(Type * type);
void releaseDefaultValue(DefaultValue * defaultValue);
void releaseConstraintValue(ConstraintValue * constraintValue);
void releaseConstraint(Constraint * constraint);
void releaseConstraints(Constraints *constraints);
void releaseAttribute(Attribute * attribute);
void releaseAttributes(Attributes *attributes);
void releaseContent(Content * content);
void releaseTables(Tables *tables);
void releaseProgram(Program * program);

#endif
