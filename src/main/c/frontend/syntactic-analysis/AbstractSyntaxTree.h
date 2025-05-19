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
typedef struct BooleaExpression BooleanExpression;
typedef struct Function Function;
typedef struct BooleanValue BooleanValue;
typedef struct IsCondition IsCondition;

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

enum BooleanType {
	BOOLEAN_TRUE,
	BOOLEAN_FALSE
}

enum IsConditionType {
	IS_CONDITION,
	IS_NOT_CONDITION
}

enum DefaultValueType {
	INTEGER_DEFAULT,
	DOUBLE_DEFAULT,
	STRING_DEFAULT,
	FUNCTION
}

enum ExpressionType {
	AND_OPERATOR, 
	OR_OPERATOR,
	NOT_OPERATOR
};

enum ConstraintValueType {
	EXPRESSION,
    BOOLEAN_EXPRESSION
};

enum FunctionType {
	CURRENT_TIMESTAMP_FUNCTION,
	NOW_OPEN_PARENTHESIS_CLOSE_PARENTHESIS_FUNCTION, 
	AUTO_INCREMENT_FUNCTION,
	CURRENT_DATE_FUNCTION,
	CURRENT_TIME_FUNCTION,
	LOCALTIME_FUNCTION,
	LOCAL_TIMESTAMP_FUNCTION,
	CURRENT_TIMESTAMP_FUNCTION,
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
	CHECK_CONSTRAINT_LCT
};

enum BooleanExpressionType {
	AND_BOOLEANTYPE,
	OR_BOOLEANTYPE,
	EQUALS_BOOLEANTYPE,
	NOT_EQUALS_BOOLEANTYPE,
	EQUALS_BOOLEANTYPE,
	LESS_THAN_BOOLEANTYPE,
	GREATER_THAN_BOOLEANTYPE,
	GREATER_THAN_EQUALS_BOOLEANTYPE,
	DISTINCT_FROM_BOOLEANTYPE,
	THREE_POINTERS_BOOLEANTYPE,
	NUL_BOOLEANTYPE,
	NOTNULL_BOOLEANTYPE,
	ISNULL_BOOLEANTYPE,
	NOT_BOOLEANTYPE,
	ID_BOOLEANTYPE,
	INTEGER_VALUE_BOOLEANTYPE,
	DOUBLE_VALUE_BOOLEANTYPE,
	STRING_VALUE_BOOLEANTYPE,
	BOOLEAN_VALUE_BOOLEANTYPE
}

struct BooleanExpression {
	union {
		struct {
			BooleanExpression * boolean_expression_left;
			BooleanExpression * boolean_expression_rigth;
		};

		struct {
			BooleanExpression * boolean_expression_distinct_from;
			IsCondition * is_condition_distinct_from;
			Type * data_type;
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
		char * string;
		double double_value;
		int integer_value;
	}
}

struct CheckConstraint {
	BooleanExpression * booleanExpression;
}

struct LocalConstraint {
	union {
		OnAction * onAction;
		CheckConstraint * checkConstraint;
		struct {}
	};
	LocalConstraintType type;
}

struct Expression {
	ExpressionType type;
}

struct IsCondition {
	IsConditionType type;
}

struct BooleanValue {
	BooleanType booleanType;
}

struct Function {
	FunctionType functionType;
};

struct ConstraintValue {
	union {
		CheckConstraint * checkConstraint;
		Expression * expression;
		struct {
			Expression * singleExpression;
			OnAction * onAction;
		}
		struct {
			Expression * mainExpression;
			Expression * secondExpression;
			OnAction * onAction;
		}
	};
	ConstraintValueType type;
};

struct Constraint {
	union {
		char * name;
		struct {}
	}   
   struct ConstraintValue * value;
};

struct Constraints {
    Constraint *first;
};

struct DefaultValue {
	union {
		int integer_value;
		double double_value;
		char * string_value;
		Function * function; 
	}
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

/*void releaseConstant(Constant * constant);
void releaseFactor(Factor * factor);*/

void releaseProperties(Properties * properties);
void releaseExpression(Expression * expression);
void releaseBooleanExpression(BooleanExpression * booleanExpression);
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
void releaseNullCondition(NullCondition * nullCondition);

#endif
