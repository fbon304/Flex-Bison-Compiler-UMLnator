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
		free(name);
	}
}

void releaseDefaultValue(DefaultValue * defaultValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (defaultValue != NULL) {
		free(defaultValue->value);
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
		free(attribute->name);
		releaseType(attribute->type);
		releaseDefaultValue(attribute->defaultValue);
		if (attribute->constraint != NULL) {
			releaseConstraint(attribute->constraint);
		}
		releaseAttribute(attribute->next);
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




