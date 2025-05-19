#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseNullCondition(NullCondition * nullCondition) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (nullCondition != NULL) {
		free(nullCondition);
	}
}

void releaseProperties(Properties * properties) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (properties != NULL) {
		switch(properties->type) {
			case DEFAULT_VALUE:
				// releaseDefaultValue(properties->defaultValue);
				break;
			case CONSTRAINT_CONDITION:
				// releaseLocalConstraint(properties->constraint);
				break;
			case NULL_CONDITION:
				releaseNullCondition(properties->nullCondition);
				break;
			case NULL_CONDITION_DEFAULT_VALUE:
				releaseNullCondition(properties->nullConditionDN);
				// releaseDefaultValue(properties->defaultValueDN);
				break;
			case NULL_CONDITION_CONSTRAINT:
				releaseNullCondition(properties->nullConditionCN);
				// releaseLocalConstraint(properties->constraintCN);
				break;
			case DEFAULT_VALUE_CONSTRAINT:
				// releaseDefaultValue(properties->defaultValueDC);
				// releaseLocalConstraint(properties->constraintDC);
				break;
			case COMPLETE:
				// releaseDefaultValue(properties->defaultValueCDN);
				// releaseLocalConstraint(properties->constraintCDN);
				releaseNullCondition(properties->nullConditionCDN);
				break;
			}
			free(properties);
	}
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {

	}
}

void releaseBooleanExpression(BooleanExpression * booleanExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (booleanExpression != NULL) {

	}
}

void releaseType(Type * type) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (type != NULL) {
		free(type);
	}
}

void releaseDefaultValue(DefaultValue * defaultValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (defaultValue != NULL) {
		free(defaultValue);
	}
}

void releaseConstraintValue(ConstraintValue * constraintValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constraintValue != NULL) {
		switch (constraintValue->type) {
			case EXPRESSION:
				releaseExpression(constraintValue->expression);
				;
			case BOOLEAN_EXPRESSION:
				releaseBooleanExpression(constraintValue->booleanExpression);
				;
		}
	}
}

void releaseConstraint(Constraint * constraint) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constraint != NULL) {
		free(constraint->name);
		releaseConstraintValue(constraint->value);
		releaseConstraint(constraint->next);
		free(constraint);
	}
}

void releaseConstraints(Constraints * constraints) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constraints != NULL) {
		releaseConstraint(constraints->first);
		free(constraints);
	}
}

void releaseAttribute(Attribute * attribute) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(attribute != NULL) {
		switch (attribute->type) {
			case COLUMN:
				free(attribute->id);
				releaseType(attribute->datatype);
				break;
			case COLUMN_WITH_PROPERTIES:
				free(attribute->p_id);
				releaseType(attribute->p_type);
				releaseProperties(attribute->properties);
				break;
		}
		free(attribute);
	}
}

void releaseAttributes(Attributes * attributes) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(attributes != NULL) {
		switch(attributes->type) {
			case MULTIPLE_ATTRIBUTE:
				releaseAttribute(attributes->attribute1);
				releaseAttribute(attributes->attribute2);
				break;
			case ATTRIBUTE:
				releaseAttribute(attributes->head);
				break;
		}
		free(attributes);
	}
}

void releaseContent(Content * content) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(content != NULL) {
		switch (content->type) {
			case ATTRIBUTES:
				releaseAttributes(content->attributes);
				break;
			case CONSTRAINTS:
				releaseConstraints(content->constraints);
				break;
			case ATTRIBUTES_CONSTRAINTS:
				releaseAttributes(content->attributes);
				releaseConstraints(content->constraints);
				break;
		}
		free(content);
	}
}

void releaseTables(Tables *tables) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (tables != NULL) {
		switch (tables->type) {
			case MULTIPLE_TABLES:
				releaseTables(tables->tables1);
				releaseTables(tables->tables2);
				break;
			case CONTENT:
				releaseContent(tables->content);
				free(tables->id);
				break;
		}
		free(tables);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseTables(program->tables);
		free(program);
	}
}


/*void releaseConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ADDITION:
			case DIVISION:
			case MULTIPLICATION:
			case SUBTRACTION:
				releaseExpression(expression->leftExpression);
				releaseExpression(expression->rightExpression);
				break;
			case FACTOR:
				releaseFactor(expression->factor);
				break;
		}
		free(expression);
	}
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				releaseConstant(factor->constant);
				break;
			case EXPRESSION:
				releaseExpression(factor->expression);
				break;
		}
		free(factor);
	}
} 
*/




