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
typedef enum ConstraintType ConstraintType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef struct Attribute Attribute;
typedef struct Attributes Attributes;
typedef struct Constraint Constraint;
typedef struct Constraints Constraints;

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
*/

enum ConstraintType {
    CHECK_CONSTRAINT,
    PRIMARY_KEY_CONSTRAINT,
    UNIQUE_CONSTRAINT,
    FOREIGN_KEY_CONSTRAINT
};

/*
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

struct Program {
	Content * content;
};

struct Attribute {
    char *name;              /**< Column identifier */
    Type *type;              /**< Data type node */
    bool not_null;           /**< NOT NULL flag */
    DefaultValue *default_value; /**< Optional DEFAULT clause */
    Constraint *constraint;  /**< Optional inline constraint */
    Attribute *next;         /**< Next attribute in list */
};

struct Attributes {
    Attribute *head;
};

struct Constraint {
    ConstraintType type;     /**< Kind of constraint */
    char *name;              /**< Optional constraint name */
    /* For CHECK constraints */
    Expression *condition;
    /* For key constraints (PRIMARY, UNIQUE, FOREIGN) */
    char **columns;          /**< Column names involved */
    int column_count;
    Constraint *next;        /**< Next constraint in list */
};

struct Constraints {
    Constraint *head;
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseProgram(Program * program);
void releaseAttributes(Attributes *attributes);
void releaseConstraints(Constraints *constraints);

#endif
