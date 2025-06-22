#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static void _generateEpilogue();
static void _generateProgram(Program * program);
static void _generatePrologue();
static void _generateTablesList(const unsigned int indentationLevel, TablesList * tablesList);
static void _generateTable(const unsigned int indentationLevel, Tables * table);
static void _generateContent(const unsigned int indentationLevel, Content * content);
static void _generateContentElement(const unsigned int indentationLevel, ContentElement * contentElement);
static void _generateAttribute(const unsigned int indentationLevel, Attribute * attribute);
static void _generateProperties(const unsigned int indentationLevel, Properties * properties);
static void _generateDefaultValue(const unsigned int indentationLevel, DefaultValue * defaultValue);
static void _generateFunction(const unsigned int indentationLevel, Function * function);
static void _generateLocalConstraint(const unsigned int indentationLevel, LocalConstraint * localConstraint);
static void _generateNullCondition(const unsigned int indentationLevel, NullCondition * nullCondition);
static void _generateConstraint(const unsigned int indentationLevel, Constraint * constraint);
static void _generateConstraintValue(const unsigned int indentationLevel, ConstraintValue * constraintValue);
static void _generateType(const unsigned int indentationLevel, Type * type);
static void _generateOnAction(const unsigned int indentationLevel, OnAction * onAction);
static void _generateAction(const unsigned int indentationLevel, Action * action);
static void _generateCheckConstraint(const unsigned int indentationLevel, CheckConstraint * checkConstraint);
static void _generateBooleanExpression(const unsigned int indentationLevel, BooleanExpression * booleanExpression);
static void _generateBooleanFactor(const unsigned int indentationLevel, BooleanFactor * booleanFactor);
static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generateIsCondition(const unsigned int indentationLevel, IsCondition * isCondition);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

static char ** constraintsBuff;
static int fKCount = 0;

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
    _generateTablesList(2, program->tablesList);
}

static void _generateEpilogue() {
	_output(0, "%s%d%s",
		"@enduml\n"
	);
}

static void _generateTablesList(const unsigned int indentationLevel, TablesList * tablesList) {
	if (tablesList != NULL) {
		_generateTable(indentationLevel, tablesList->tables);
		_generateTablesList(indentationLevel, tablesList->tablesList);
	}
}

static void _generateTable(const unsigned int indentationLevel, Tables * table) {
	if (table != NULL) { 
		_output(indentationLevel, "object %s {\n", table->id);
		_generateContent(indentationLevel + 1, table->content);
		for (int i = 0; i < fKCount; i++) {
			printf("%s\n", constraintsBuff[i]);
		}
		_output(indentationLevel + 1, "}");
	}
}

static void _generateContent(const unsigned int indentationLevel, Content * content) {
	if (content != NULL) {
		if (content->type == ELEMENT) {
			_generateContentElement(indentationLevel, content->content_element);
		} else if (content->type == CONTENT_LIST) {
			_generateContent(indentationLevel, content->content);
			_generateContentElement(indentationLevel, content->content_element);
		}
	}
}

static void _generateContentElement(const unsigned int indentationLevel, ContentElement * contentElement) {
	if (contentElement != NULL) {
		if (contentElement->contentElementType == ATTRIBUTE_TYPE) {
			_generateAttribute(indentationLevel, contentElement->attribute);
		} else if (contentElement->contentElementType == CONSTRAINT_TYPE) {
			// TODO capaz se puede hacer que retorne una funcion que genere al final de todo todos los constraints
			_generateConstraint(indentationLevel, contentElement->constraint);
		}
	}
}

static void _generateAttribute(const unsigned int indentationLevel, Attribute * attribute) {
	if (attribute != NULL) {
		CompilerState * ccs = currentCompilerState(); 
        if (attribute->type == COLUMN) {
			Symbol * entry = getEntry(ccs->symbolTable, getScope(ccs->scopeStack), attribute->id);
            _output(indentationLevel, "%s%s: <size:12>", entry->isPrimaryKey ? "-" : "", attribute->id);
            _generateType(indentationLevel, attribute->datatype);
        } else if (attribute->type == COLUMN_WITH_PROPERTIES) {
			Symbol * entry = getEntry(ccs->symbolTable, getScope(ccs->scopeStack), attribute->id);
            _output(indentationLevel, "%s%s: <size:12>", entry->isPrimaryKey ? "-" : "", attribute->p_id);
            _generateType(indentationLevel, attribute->p_type);
            _generateProperties(indentationLevel, attribute->properties);
			_output(indentationLevel, ";\n");
        }
	}
}

static void _generateType(const unsigned int indentationLevel, Type * type) {
    if(type != NULL) {
        switch(type->type) {
            case INTEGER_DATATYPE:
                _output(indentationLevel, "INT ");
                break;
            case SMALLINT_DATATYPE:
                _output(indentationLevel, "SMALLINT ");
                break;
            case BIGINT_DATATYPE:
                _output(indentationLevel, "BIGINT ");
                break;
            case REAL_DATATYPE:
                _output(indentationLevel, "REAL ");
                break;
            case DOUBLE_DATATYPE:
                _output(indentationLevel, "DOUBLE ");
                break;
            case BOOLEAN_DATATYPE:
                _output(indentationLevel, "BOOLEAN ");
                break;
            case DATE_DATATYPE:
                _output(indentationLevel, "DATE ");
                break;
            case TIMESTAMP_DATATYPE:
                _output(indentationLevel, "TIMESTAMP ");
                break;
            case INTERVAL_DATATYPE:
                _output(indentationLevel, "INTERVAL ");
                break;
            case TEXT_DATATYPE:
                _output(indentationLevel, "TEXT ");
                break;
            case SERIAL_DATATYPE:
                _output(indentationLevel, "SERIAL ");
                break;
            case CHAR_DATATYPE:
                _output(indentationLevel, "CHAR(%d) ", type->param1);
                break;
            case VARCHAR_DATATYPE:
                _output(indentationLevel, "VARCHAR(%d) ", type->param1);
                break;
            case FLOAT_DATATYPE:
                _output(indentationLevel, "FLOAT(%d) ", type->param1);
                break;
            case TIME_DATATYPE:
                _output(indentationLevel, "TIME(%d) ", type->param1);
                break;
            case NUMBER_DATATYPE:
                _output(indentationLevel, "NUMBER(%d, %d) ", type->param1, type->param2);
                break;
            case UNKNOWN_SQL_DATATYPE:
                break;
        }
    }
}

static void _generateProperties(const unsigned int indentationLevel, Properties * properties) {
    if (properties != NULL) {
        switch (properties->type){
            case DEFAULT_VALUE:
                _generateDefaultValue(indentationLevel, properties->defaultValue);
                break;
            case CONSTRAINT_CONDITION:
                _generateLocalConstraint(indentationLevel, properties->constraint);
                break;
            case NULL_CONDITION:
                _generateNullCondition(indentationLevel, properties->nullCondition);
                break;
            case NULL_CONDITION_DEFAULT_VALUE:
                _generateNullCondition(indentationLevel, properties->nullConditionDN);
                _generateDefaultValue(indentationLevel, properties->defaultValueDN);
                break;
            case DEFAULT_VALUE_CONSTRAINT:
                _generateDefaultValue(indentationLevel, properties->defaultValueDC);
                _generateLocalConstraint(indentationLevel, properties->constraintDC);
                break;
            case NULL_CONDITION_CONSTRAINT:
                _generateNullCondition(indentationLevel, properties->nullConditionCN);
                _generateLocalConstraint(indentationLevel, properties->constraintCN);
                break;
            case COMPLETE:
                _generateLocalConstraint(indentationLevel, properties->constraintCDN);
                _generateDefaultValue(indentationLevel, properties->defaultValueCDN);
                _generateNullCondition(indentationLevel, properties->nullConditionCDN);
                break;
        }
    }
}

static void _generateDefaultValue(const unsigned int indentationLevel, DefaultValue * defaultValue) {
	if (defaultValue != NULL) {
		if (defaultValue->type == INTEGER_DEFAULT) {
			_output(indentationLevel, "| DEFAULT %u", defaultValue->integer_value);
		} else if (defaultValue->type == DOUBLE_DEFAULT) {
			_output(indentationLevel, "| DEFAULT %d", defaultValue->double_value);
		} else if (defaultValue->type == STRING_DEFAULT) {
			_output(indentationLevel, "| DEFAULT %s", defaultValue->string_value);
		} else if (defaultValue->type == FUNCTION) {
			_output(indentationLevel, "| DEFAULT");
			_generateFunction(indentationLevel, defaultValue->function);
		}
	}
}

static void _generateFunction(const unsigned int indentationLevel, Function * function) {
	if (function != NULL) {
		switch (function->functionType) {
		case CURRENT_TIMESTAMP_FUNCTION:
			_output(indentationLevel, "CURRENT_TIMESTAMP");
			break;
		case AUTO_INCREMENT_FUNCTION:
			_output(indentationLevel, "AUTO_INCREMENT");
			break;
		case CURRENT_DATE_FUNCTION:
			_output(indentationLevel, "CURRENT_DATE");
			break;
		case CURRENT_TIME_FUNCTION:
			_output(indentationLevel, "CURRENT_TIME");
			break;
		case LOCALTIME_FUNCTION:
			_output(indentationLevel, "LOCALTIME");
			break;
		case LOCALTIMESTAMP_FUNCTION:
			_output(indentationLevel, "LOCALTIMESTAMP");
			break;
		case GEN_RANDOM_UUID_OPEN_AND_CLOSE_PARENTHESIS_FUNCTION:
			_output(indentationLevel, "gen_random_uuid()");
			break;
		case UUID_GENERATE_V4_OPEN_AND_CLOSE_PARENTHESIS_FUNCTION:
			_output(indentationLevel, "uuid_generate_v4()");
			break;
		}
	}
}

static void _generateLocalConstraint(const unsigned int indentationLevel, LocalConstraint * localConstraint) {
	if (localConstraint != NULL) {
		switch(localConstraint->type) {
            case PRIMARY_KEY_LCT:
                _output(indentationLevel, "| PRIMARY KEY");
                break;
            case UNIQUE_LCT:
                _output(indentationLevel, "| UNIQUE");
                break;
            case FOREIGN_KEY_LCT:
                _output(indentationLevel, "| REFERENCES %s ", localConstraint->id);
                _generateOnAction(indentationLevel, localConstraint->onAction);
                break; 
            case FOREING_KEY_DOUBLE_NAME_LCT:
                _output(indentationLevel, "| REFERENCES %s(%s) ", localConstraint->id1, localConstraint->id2);
                _generateOnAction(indentationLevel, localConstraint->onActionComplex);
                break;
            case CHECK_LCT:
                _generateCheckConstraint(indentationLevel, localConstraint->checkConstraint);
                break;
        }
	}
}

static void _generateOnAction(const unsigned int indentationLevel, OnAction * onAction) {
    if(onAction != NULL) {
        switch(onAction->type) {
            case DELETE_ON_ACTION:
                _output(indentationLevel, "| ON DELETE ");
                _generateAction(indentationLevel, onAction->action);                
                break;
            case UPDATE_ON_ACTION:
                _output(indentationLevel, "| ON UPDATE ");
                _generateAction(indentationLevel, onAction->action);
                break;
            case ON_DELETE_ON_UPDATE_ON_ACTION:
                _output(indentationLevel, "| ON DELETE ");
                _generateAction(indentationLevel, onAction->deleteAction);
                _output(indentationLevel, "ON UPDATE ");
                _generateAction(indentationLevel, onAction->updateAction);
                break;
            case LAMBDA_ON_ACTION:
                break;
        }
    }
}

static void _generateAction(const unsigned int indentationLevel, Action * action) {
    if(action != NULL) {
        switch(action->type) {
            case CASCADE_ACTION:
                _output(indentationLevel, "CASCADE");
                break;
            case SET_NUL_ACTION:
                _output(indentationLevel, "SET NULL");
                break;
            case SET_DEFAULT_ACTION:
                _output(indentationLevel, "SET DEFAULT");
                break;
            case NO_ACTION_ACTION:
                _output(indentationLevel, "NO ACTION");
                break;
            case RESTRICT_ACTION:
                _output(indentationLevel, "RESTRICT");
                break;
        }
    }
}

static void _generateCheckConstraint(const unsigned int indentationLevel, CheckConstraint * checkConstraint) {
	if (checkConstraint != NULL) {
		_generateBooleanExpression(indentationLevel, checkConstraint->booleanExpression);
	}
}

static void _generateBooleanExpression(const unsigned int indentationLevel, BooleanExpression * booleanExpression) {
	if (booleanExpression != NULL) {
		switch (booleanExpression->type) {
			case AND_BOOLEANTYPE:
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_left);
				_output(indentationLevel, " AND ");
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_right);
				break;
			case OR_BOOLEANTYPE:
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_left);
				_output(indentationLevel, " OR ");
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_right);
				break;
			case EQUALS_BOOLEANTYPE:
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_left);
				_output(indentationLevel, " == ");
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_right);
				break;
			case NOT_EQUALS_BOOLEANTYPE:
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_left);
				_output(indentationLevel, " != ");
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_right);
				break;
			case LESS_THAN_BOOLEANTYPE:
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_left);
				_output(indentationLevel, " < ");
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_right);
				break;
			case GREATER_THAN_BOOLEANTYPE:
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_left);
				_output(indentationLevel, " > ");
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_right);
				break;
			case GREATER_THAN_EQUALS_BOOLEANTYPE:
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_left);
				_output(indentationLevel, " >= ");
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_right);
				break;
			case LESS_THAN_EQUALS_BOOLEANTYPE:
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_left);
				_output(indentationLevel, " <= ");
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_right);
				break;
			case NOTNULL_BOOLEANTYPE:
				_output(indentationLevel, "NOT NULL ");
				_generateBooleanExpression(indentationLevel, booleanExpression->boolean_expression_left);
				break;
			case ISNULL_BOOLEANTYPE:
				_output(indentationLevel, "IS NULL ");
				_generateBooleanExpression(indentationLevel, booleanExpression->unique_boolean_expression);
				break;
			case BOOLEAN_FACTOR_BOOLEANTYPE:
				_generateBooleanFactor(indentationLevel, booleanExpression->boolean_factor);
				break;
		}
	}
}

static void _generateBooleanFactor(const unsigned int indentationLevel, BooleanFactor * booleanFactor) {
	if (booleanFactor != NULL) {
		switch (booleanFactor->type) {
		case BOOLEAN_EXPRESSION_PARENTHESIS_TYPE:
			_output(indentationLevel, "(");
			_generateBooleanExpression(indentationLevel, booleanFactor->booleanExpression);
			_output(indentationLevel, ")");
			break;
		case NOT_BOOLEAN_EXPRESSION:
			_output(indentationLevel, "NOT ");
			_generateBooleanExpression(indentationLevel, booleanFactor->booleanExpression);
			break;
		case FACTOR_CONDITION_FACTOR:
			_generateFactor(indentationLevel, booleanFactor->factor_left);
			_generateIsCondition(indentationLevel, booleanFactor->isCondition_with_two_factors);
			_generateFactor(indentationLevel, booleanFactor->factor_right);
			break;
		case FACTOR_CONDITION_NUL:
			_generateFactor(indentationLevel, booleanFactor->factor_with_is_condition);
			_generateIsCondition(indentationLevel, booleanFactor->is_condition_with_factor);
			_output(indentationLevel, "NULL");
			break;
		case DISTINCT_FROM_FACTORS:
			_generateFactor(indentationLevel, booleanFactor->factor_left);
			_generateIsCondition(indentationLevel, booleanFactor->isCondition_with_two_factors);
			_output(indentationLevel, "DISTINCT FROM ");
			_generateFactor(indentationLevel, booleanFactor->factor_right);
			break;
		case FACTOR_TYPE:
			_generateFactor(indentationLevel, booleanFactor->factor);
			break;
		}
	}
}

static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
	if (factor != NULL) {
		switch (factor->type) {
		case INTEGER_FACTOR_TYPE:
			_output(indentationLevel, "%u", factor->integer_factor);
			break;
		case DOUBLE_FACTOR_TYPE:
			_output(indentationLevel, "%f", factor->double_factor);
			break;
		case ID_FACTOR_TYPE:
		case STRING_FACTOR_TYPE:
			_output(indentationLevel, "%s", factor->string);
			break;
		case BOOLEAN_TRUE:
			_output(indentationLevel, "TRUE");
			break;
		case BOOLEAN_FALSE:
			_output(indentationLevel, "FALSE");
			break;
		}
	}	
}

static void _generateIsCondition(const unsigned int indentationLevel, IsCondition * isCondition) {
	if (isCondition != NULL) {
		if (isCondition->type == IS_CONDITION) {
			_output(indentationLevel, " IS ");
		} else if (isCondition->type == IS_NOT_CONDITION) {
			_output(indentationLevel, " IS NOT ");
		}
	}
}

static void _generateNullCondition(const unsigned int indentationLevel, NullCondition * nullCondition) {
	if (nullCondition != NULL) {
		if (nullCondition->type == NOT_NULL_CONDITION) {
			_output(indentationLevel, "| NOT NULL");
		} else if (nullCondition->type == NULL_CONDITION) {
			_output(indentationLevel, "| NULL");
		}
	}
}

static void _generateConstraint(const unsigned int indentationLevel, Constraint * constraint) {
	if (constraint != NULL) {
		if (constraint->type == NAMED_CONSTRAINT) {
			_output(indentationLevel, "| CONSTRAINT %s ");
		}
		_generateConstraintValue(indentationLevel, constraint->constraintValue);
	}
}

static void _generateConstraintValue(const unsigned int indentationLevel, ConstraintValue * constraintValue) {
	if (constraintValue != NULL) {
		switch (constraintValue->type) {
		case CHECK_CONSTRAINT_TYPE:
			_generateCheckConstraint(indentationLevel, constraintValue->checkConstraint);
			break;
		case PRIMARY_KEY_CONSTRAINT_TYPE:
			_output(indentationLevel, "PRIMARY KEY (");
			_generateExpression(indentationLevel, constraintValue->expression);
			_output(indentationLevel, ")");
			break;
		case UNIQUE_CONSTRAINT_TYPE:
			output(indentationLevel, "UNIQUE (");
			_generateExpression(indentationLevel, constraintValue->expression);
			_output(indentationLevel, ")");
			break;
		case FOREIGN_KEY_CONSTRAINT_TYPE:
	
	
			Expression * currentExpression = constraintValue->singleExpression;
			do {
				fKCount++;
				realloc(constraintsBuff, fKCount);
				constraintsBuff[fKCount-1] = malloc(MAX_BUFF_SIZE);
				CompilerState * ccs = currentCompilerState();
				char * tableName = peek(getScope(ccs->scopeStack));
				char * action = _getOnAction(constraintValue->onActionSingle);
				snprintf(constraintsBuff[fKCount-1], sizeof(constraintsBuff[fKCount-1]), "%s::%s \"<size:20><color:#FFFFFF>1\" --- \"<size:20><color:#FFFFFF>*\" %s::%s : %s", tableName, currentExpression->id, constraintValue->id, currentExpression->id, action);
				currentExpression = currentExpression->expression;
			} while (currentExpression != NULL);
			break;
		case FOREIGN_KEY_DOUBLE_EXPRESSION_CONSTRAINT_TYPE:
			Expression * localExpression = constraintValue->mainExpression;
			Expression * foreignExpression = constraintValue->secondExpression;
			do {
				do {
					fKCount++;
					realloc(constraintsBuff, fKCount);
					constraintsBuff[fKCount-1] = malloc(MAX_BUFF_SIZE);
					CompilerState * ccs = currentCompilerState();
					char * tableName = peek(getScope(ccs->scopeStack));
					char * action = _getOnAction(constraintValue->onActionSingle);
					snprintf(constraintsBuff[fKCount-1], sizeof(constraintsBuff[fKCount-1]), "%s::%s \"<size:20><color:#FFFFFF>1\" --- \"<size:20><color:#FFFFFF>*\" %s::%s : %s", tableName, localExpression->id, constraintValue->id, foreignExpression->id, action);
					foreignExpression = foreignExpression->expression;
				} while (foreignExpression->expression != NULL);
				localExpression = localExpression->expression;
			} while (currentExpression != NULL);
			break;
		}
	}
}

static char * _getOnAction(OnAction * onAction) {
	if (onAction != NULL) {
		switch (onAction->type) {
			case DELETE_ON_ACTION:
				char * pref = "ON DELETE ";
				char * action = _getAction(onAction->action);
				size_t totalLength = strlen(pref) + strlen(action) + 1;
				char * buf = malloc(totalLength); // ver los frees!!!
				if(buf == NULL) {
					logError(_logger, "Memory allocation failed for buffer in _getOnAction.");
					return NULL;
				}
				strcpy(buf, pref);
				strncat(buf, action, totalLength);
 				return buf;
			case UPDATE_ON_ACTION:
				char * pref = "ON UPDATE ";
				char * action = _getAction(onAction->action);
				size_t totalLength = strlen(pref) + strlen(action) + 1;
				char * buf = malloc(totalLength);
				if(buf == NULL) {
					logError(_logger, "Memory allocation failed for buffer in _getOnAction.");
					return NULL;
				}
				strcpy(buf, pref);
				strncat(buf, action, totalLength);
 				return buf;
			case ON_DELETE_ON_UPDATE_ON_ACTION:
				char * onUpdate = "ON UPDATE ";
				char * updateAction = _getAction(onAction->updateAction);
				char * onDelete = " ON DELETE ";
				char * deleteAction = _getAction(onAction->deleteAction);
				size_t totalLength = strlen(onUpdate) + strlen(onDelete) + strlen(updateAction) + strlen(deleteAction) + 1;
				char * buf = malloc(totalLength);
				if(buf == NULL) {
					logError(_logger, "Memory allocation failed for buffer in _getOnAction.");
					return NULL;
				}
				strcpy(buf, onUpdate);
				strncat(buf, updateAction, totalLength);
				strncat(buf, onDelete, totalLength);
				strncat(buf, deleteAction, totalLength);
 				return buf;
			case LAMBDA_ON_ACTION:
				return "";
		}
	}
}

static char * _getAction(Action * action) {
	if (action != NULL) {
		switch (action->type) {
		case CASCADE_ACTION:
			return "CASCADE";
		case SET_NUL_ACTION:
			return "SET NULL";
		case SET_DEFAULT_ACTION:
			return "SET DEFAULT";
		case NO_ACTION_ACTION:
			return "NO ACTION";
		case RESTRICT_ACTION:
			return "RESTRICT";
		}
	}
	return "";
}

static void _generatePrologue(void) {
	_output(0, "%s",
		"@startuml\n"
	);
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	//logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue();
	//logDebugging(_logger, "Generation is done.");
}