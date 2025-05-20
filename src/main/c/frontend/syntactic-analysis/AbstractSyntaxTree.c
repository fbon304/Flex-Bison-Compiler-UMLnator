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

void releaseFunction(Function * function) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (function != NULL) {
		free(function);
	}
}

void releaseNullCondition(NullCondition * nullCondition) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (nullCondition != NULL) {
		free(nullCondition);
	}
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(factor != NULL) {
		switch(factor->type) {
			case ID_FACTOR_TYPE:
			case STRING_FACTOR_TYPE:
				free(factor->string);
				break;
		}
		free(factor);
	}
}

void releaseProperties(Properties * properties) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (properties != NULL) {
		switch(properties->type) {
			case DEFAULT_VALUE:
				releaseDefaultValue(properties->defaultValue);
				break;
			case CONSTRAINT_CONDITION:
				releaseLocalConstraint(properties->constraint);
				break;
			case NULL_CONDITION:
				releaseNullCondition(properties->nullCondition);
				break;
			case NULL_CONDITION_DEFAULT_VALUE:
				releaseNullCondition(properties->nullConditionDN);
				releaseDefaultValue(properties->defaultValueDN);
				break;
			case NULL_CONDITION_CONSTRAINT:
				releaseNullCondition(properties->nullConditionCN);
				releaseLocalConstraint(properties->constraintCN);
				break;
			case DEFAULT_VALUE_CONSTRAINT:
				releaseDefaultValue(properties->defaultValueDC);
				releaseLocalConstraint(properties->constraintDC);
				break;
			case COMPLETE:
				releaseDefaultValue(properties->defaultValueCDN);
				releaseLocalConstraint(properties->constraintCDN);
				releaseNullCondition(properties->nullConditionCDN);
				break;
			}
			free(properties);
	}
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch(expression->type) {
			case SIMPLE_EXPRESSION:
			free(expression->id);
			break;
			
			case COMPLEX_EXPRESSION:
			free(expression->id);
			free(expression->expression);
			break;
		}

		free(expression);
	}
}

void releaseIsCondition(IsCondition * isCondition) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(isCondition != NULL) {
		free(isCondition);
	}

	IS_CONDITION,
	IS_NOT_CONDITION
}

void releaseBooleanExpression(BooleanExpression * booleanExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (booleanExpression != NULL) {
		switch(booleanExpression->type) {
			case BETWEEN_PARENTHESIS:
			case NOTNULL_BOOLEANTYPE:
			case ISNULL_BOOLEANTYPE:
			case NOT_BOOLEANTYPE:
				releaseBooleanExpression(booleanExpression->unique_boolean_expression);
				break;
			case AND_BOOLEANTYPE:
			case OR_BOOLEANTYPE:
			case EQUALS_BOOLEANTYPE:
			case NOT_EQUALS_BOOLEANTYPE:
			case LESS_THAN_BOOLEANTYPE:
			case GREATER_THAN_BOOLEANTYPE:
			case GREATER_THAN_EQUALS_BOOLEANTYPE:
			case LESS_THAN_EQUALS_BOOLEANTYPE:
				releaseBooleanExpression(booleanExpression->boolean_expression_left);
				releaseBooleanExpression(booleanExpression->boolean_expression_right);
				break;
			case THREE_POINTERS_BOOLEANTYPE:
				releaseBooleanExpression(booleanExpression->boolean_expression_three_pointers);
				releaseIsCondition(booleanExpression->is_condition_three_pointers);
				releaseBooleanValue(booleanExpression->boolean_value);
				break;
			case NUL_BOOLEANTYPE:
				releaseBooleanExpression(booleanExpression->boolean_expression_with_is_condition);
				releaseIsCondition(booleanExpression->is_condition_with_boolean_expression);
				break;
			case BOOLEAN_VALUE_BOOLEANTYPE:
				releaseBooleanValue(booleanExpression->unique_boolean_value);
				break;
			case DISTINCT_FROM_BOOLEANTYPE:
			releaseFaxctor(booleanExpression->factor_left);
				releaseIsCondition(booleanExpression->isCondition_with_two_factors);
				releaseFactor(booleanExpression->factor_right);
		}
		free(booleanExpression);
	}
}

void releaseBooleanValue(BooleanValue * booleanValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (booleanValue != NULL) {
		free(booleanValue);
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
		switch(defaultValue->type) {
			case INTEGER_DEFAULT:
				break;
			case DOUBLE_DEFAULT:
				break;
			case STRING_DEFAULT:
				free(defaultValue->string_value);
				break;
			case FUNCTION:
				releaseFunction(defaultValue->function);
			break;
		}
		free(defaultValue);
	}
}

void releaseCheckConstraint(CheckConstraint * checkConstraint) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(checkConstraint != NULL) {
		releaseBooleanExpression(checkConstraint->booleanExpression);
		free(checkConstraint);
	}
}

void releaseAction(Action * action) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(action != NULL) {
		free(action);
	}
}

void releaseOnAction(OnAction * onAction) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(onAction != NULL) {
		switch (onAction->type) {
			case DELETE_ON_ACTION:
			case UPDATE_ON_ACTION:
				releaseAction(onAction->action);
				break;

			case ON_DELETE_ON_UPDATE_ON_ACTION:
				releaseAction(onAction->deleteAction);
				releaseAction(onAction->updateAction);
				break;
		}
		free(onAction);
	}
}

void releaseLocalConstraint(LocalConstraint * localConstraint){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(localConstraint != NULL) {
		switch (localConstraint->type) {
			case FOREIGN_KEY_LCT:
				free(localConstraint->id);
				releaseOnAction(localConstraint->onAction);
				break;

			case CHECK_LCT:
				releaseCheckConstraint(localConstraint->checkConstraint);
				break;

			case FOREING_KEY_ATTRIBUTE_LCT:
				free(localConstraint->id1);
				free(localConstraint->id2);
				releaseOnAction(localConstraint->onAction);
				break;
			break;
		}

		free(localConstraint);
	}
}

void releaseConstraintValue(ConstraintValue * constraintValue) { 
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constraintValue != NULL) {
		switch (constraintValue->type) {
			case CHECK_CONSTRAINT_TYPE:
				releaseCheckConstraint(constraintValue->checkConstraint);
				break;
			case PRIMARY_KEY_CONSTRAINT_TYPE:
				releaseExpression(constraintValue->expression);
				break;
			case UNIQUE_CONSTRAINT_TYPE:
				releaseExpression(constraintValue->expression);
				break;
			case FOREIGN_KEY_CONSTRAINT_TYPE:
				free(constraintValue->id);
				releaseExpression(constraintValue->singleExpression);
				releaseOnAction(constraintValue->onActionSingle);
				break;
			case FOREIGN_KEY_DOUBLE_EXPRESSION_CONSTRAINT_TYPE:
				free(constraintValue->idComplex);
				releaseExpression(constraintValue->mainExpression);
				releaseExpression(constraintValue->secondExpression);
				releaseOnAction(constraintValue->onActionDouble);
				break;
		}
		free(constraintValue);
	}
}

void releaseConstraint(Constraint * constraint) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constraint != NULL) {
		switch(constraint->type) {
			case NAMED_CONSTRAINT:
				free(constraint->id);
				releaseConstraintValue(constraint->constraintValue);
				break;
			case UNNAMED_CONSTRAINT:
				releaseConstraintValue(constraint->singleConstraintValue);
				break;
		}
	}
	free(constraint);
}

void releaseConstraints(Constraints * constraints) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constraints != NULL) {
		switch(constraints->type) {
			case DOUBLE_CONSTRAINT:
				releaseConstraint(constraints->constraint1);
				releaseConstraint(constraints->constraints);
				break;
			case SINGLE_CONSTRAINT:
				releaseConstraint(constraints->constraint);
				break;
		}
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



