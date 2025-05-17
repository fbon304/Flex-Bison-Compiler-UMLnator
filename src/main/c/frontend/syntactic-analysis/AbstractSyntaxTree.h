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

typedef enum ConstraintValueType ConstraintValueType;

//typedef struct Factor Factor;
//typedef struct Constant Constant;

typedef struct Program Program;
typedef struct Tables Tables;
typedef struct Content Content;
typedef struct Attributes Attributes;
typedef struct Attribute Attribute;
typedef struct Type Type;
typedef struct DefaultValue DefaultValue;
typedef struct Expression Expression;
typedef struct ConstraintValue ConstraintValue;
typedef struct Constraint Constraint;
typedef struct Constraints Constraints;
typedef struct BooleaExpression BooleanExpression;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

 
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

struct Type {
	char * name;
};

struct DefaultValue {
	char * value;
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
    char *name;              
    Type *type;            
    DefaultValue *default_value; 
    Constraint *constraint;  // puede ser NULL (ej: NOT NULL o UNIQUE en línea)
    struct Attribute *next;         
};

struct Attributes {
    Attribute * head;
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
};

struct Program {
	Tables * tables;
};

/**
 * Node recursive destructors.
 */

/*void releaseConstant(Constant * constant);
void releaseFactor(Factor * factor);*/

void releaseExpression(Expression * expression);
void releaseProgram(Program * program);
void releaseAttributes(Attributes *attributes);
void releaseConstraints(Constraints *constraints);
void releaseTables(Tables *tables);

#endif
