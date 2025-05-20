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
}

void releaseBooleanExpression(BooleanExpression * booleanExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (booleanExpression != NULL) {
		switch(booleanExpression->type) {
			case NOTNULL_BOOLEANTYPE:
			case ISNULL_BOOLEANTYPE:
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
			case BOOLEAN_FACTOR_BOOLEANTYPE:
				releaseBooleanFactor(booleanExpression->boolean_factor);
				break;
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

			case FOREING_KEY_DOUBLE_NAME_LCT:
				free(localConstraint->id1);
				free(localConstraint->id2);
				releaseOnAction(localConstraint->onActionComplex);
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

void releaseContentElement(ContentElement * contentElement) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(contentElement != NULL) {
		switch (contentElement->contentElementType) {
			case ATTRIBUTE_TYPE:
				releaseAttribute(contentElement->attribute);
				break;
			case CONSTRAINT_TYPE:
				releaseConstraint(contentElement->constraint);
				break;
		}
		free(contentElement);
	}
}

void releaseContent(Content * content) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(content != NULL) {
		switch (content->type) {
			case ELEMENT:
				releaseContentElement(content->content_element);
				break;
			case CONTENT_LIST:
				releaseContent(content->content);
				releaseContentElement(content->content_element);
				break;
		}
		free(content);
	}
}

void releaseTablesList(TablesList *tablesList) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (tablesList != NULL) {
		switch (tablesList->type) {
			case LIST:
				releaseTablesList(tablesList->tablesList);
				releaseTables(tablesList->tables);
				break;
			case SINGULAR:
				releaseTables(tablesList->tables);
				break;
		}
		free(tablesList);
	}
}

void releaseTables(Tables * tables) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(tables != NULL) {
		free(tables->id);
		releaseContent(tables->content);
		free(tables);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseTablesList(program->tablesList);
		free(program);
	}
}

void releaseBooleanFactor(BooleanFactor * booleanFactor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(booleanFactor != NULL) {
		switch(booleanFactor->type) {
			case BOOLEAN_EXPRESSION_PARENTHESIS_TYPE:
			case NOT_BOOLEAN_EXPRESSION:
			releaseBooleanExpression(booleanFactor->booleanExpression);
			break;

			case BOOLEAN_VALUE_TYPE:
			releaseBooleanValue(booleanFactor->booleanValue);
			break;

			case FACTOR_TYPE:
			releaseFactor(booleanFactor->factor);
			break;

			case DISTINCT_FROM_BOOLEANTYPE:
				releaseFactor(booleanFactor->factor_left);
				releaseIsCondition(booleanFactor->isCondition_with_two_factors);
				releaseFactor(booleanFactor->factor_right);

			case NUL_BOOLEANTYPE:
				releaseFactorExpression(booleanFactor->boolean_factor_with_is_condition);
				releaseIsCondition(booleanFactor->is_condition_with_boolean_factor);
				break;

			case THREE_POINTERS_BOOLEANTYPE:
				releaseFactorExpression(booleanFactor->boolean_factor_three_pointers);
				releaseIsCondition(booleanFactor->is_condition_three_pointers);
				releaseBooleanValue(booleanFactor->boolean_value);
				break;
		}

		free(booleanFactor);
	}
}



