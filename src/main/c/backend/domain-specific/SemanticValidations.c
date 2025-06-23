#include "semanticValidations.h"

static void validateTablesList(TablesList * tablesList);
static void validateTables(Tables * table);
static int validateAttributes(Content * content);
static int validateConstraints(Content * content);
static int validateLocalConstraint(char * attributeName, LocalConstraint * localConstraints);
static DataType validateBooleanExpression(BooleanExpression * expr);
static DataType validateBooleanFactor(BooleanFactor * booleanFactor);
static DataType translateType(DataType type);
static boolean isFunctionCompatible(DataType attrType, Function * function);
static boolean isDefaultValueCompatible(DataType attrType, DefaultValue * defaultValue);

static Logger * _logger = NULL;
static boolean * errors;

void initializeSemanticValidationsModule(CompilerState * compilerState) {
	_logger = createLogger("SemanticValidations");
    errors = &compilerState->errors;
}

void shutdownSemanticValidationsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

static boolean areTypesCompatible(DataType t1, DataType t2) {
    return t1 == t2;
}

static DataType translateType(DataType type) {
    switch (type) {
        case INTEGER_TYPE:
        case SMALLINT_TYPE:
        case BIGINT_TYPE:
        case SERIAL_TYPE_TYPE:
            return INTEGER_TYPE;
        case REAL_TYPE:
        case DOUBLE_TYPE:
        case FLOAT_TYPE:
        case NUMBER_TYPE:
            return DOUBLE_TYPE;
        case UUID_TYPE:
        case BOOLEAN_TYPE:
        case TIME_TYPE:
        case BOTTOM:
        case NONE:
            return type;
        case DATE_TYPE:
        case TIMESTAMP_TYPE:
            return TIMESTAMP_TYPE;
        case TEXT_TYPE:
        case CHAR_TYPE:
        case VARCHAR_TYPE:
            return TEXT_TYPE;
    }
}

static DataType validateBooleanFactor(BooleanFactor * booleanFactor) {
    switch (booleanFactor->type) {
        case FACTOR_TYPE:
            if (booleanFactor->factor) {
                if (booleanFactor->factor->type == ID_FACTOR_TYPE) {
                    if (!variableExistsInScope(booleanFactor->factor->string)) {
                        *errors = true;
                        logError(_logger, "Column '%s' in CHECK constraint does not exist.", booleanFactor->factor->string);
                        return BOTTOM;
                    }
                    return translateType(type(booleanFactor->factor->string));
                } else if (booleanFactor->factor->type == STRING_FACTOR_TYPE) {
                    if (isTime(booleanFactor->factor->string)) {
                        return TIME_TYPE;
                    } else if (isTimestamp(booleanFactor->factor->string)) {
                        return TIMESTAMP_TYPE;
                    } else if (isDate(booleanFactor->factor->string)) {
                        return DATE_TYPE;
                    } else {
                        return TEXT_TYPE;
                    }
                } else if (booleanFactor->factor->type == INTEGER_FACTOR_TYPE) {
                    return INTEGER_TYPE;
                } else if (booleanFactor->factor->type == DOUBLE_FACTOR_TYPE) {
                    return DOUBLE_TYPE;
                } else if (booleanFactor->factor->type == BOOLEAN_FALSE || booleanFactor->factor->type == BOOLEAN_TRUE) {
                    return BOOLEAN_TYPE;
                }
            }
            break;
        case FACTOR_CONDITION_FACTOR:
        case DISTINCT_FROM_FACTORS:
            // Validate left factor
            if (booleanFactor->factor_left->type == ID_FACTOR_TYPE) {
                if (!variableExistsInScope(booleanFactor->factor_left->string)) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint does not exist.", booleanFactor->factor_left->string);
                } else if (type(booleanFactor->factor_left->string) != BOOLEAN_TYPE) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint must be of type BOOLEAN.", booleanFactor->factor_left->string);
                }
            } else if (booleanFactor->factor_left->type != BOOLEAN_TRUE && booleanFactor->factor_left->type != BOOLEAN_FALSE) {
                *errors = true;
                logError(_logger, "Left factor in CHECK constraint must be a boolean value or a boolean column.");
            }

            // Validate right factor
            if (booleanFactor->factor_right->type == ID_FACTOR_TYPE) {
                if (!variableExistsInScope(booleanFactor->factor_right->string)) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint does not exist.", booleanFactor->factor_right->string);
                } else if (type(booleanFactor->factor_right->string) != BOOLEAN_TYPE) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint must be of type BOOLEAN.", booleanFactor->factor_right->string);
                }
            } else if (booleanFactor->factor_right->type != BOOLEAN_TRUE && booleanFactor->factor_right->type != BOOLEAN_FALSE) {
                *errors = true;
                logError(_logger, "Right factor in CHECK constraint must be a boolean value or a boolean column.");
            }
            return BOOLEAN_TYPE;
        case FACTOR_CONDITION_NUL:
            // factor_with_is_condition IS_CONDITION/IS_NOT_CONDITION NUL
            if (booleanFactor->factor_with_is_condition->type == ID_FACTOR_TYPE) {
                if (!variableExistsInScope(booleanFactor->factor_with_is_condition->string)) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint does not exist.", booleanFactor->factor_with_is_condition->string);
                }
            }
            return BOOLEAN_TYPE;            
        case BOOLEAN_EXPRESSION_PARENTHESIS_TYPE:
        case NOT_BOOLEAN_EXPRESSION:
            return validateBooleanExpression(booleanFactor->booleanExpression);
    }
}

static DataType validateBooleanExpression(BooleanExpression * expr) {
    switch (expr->type) {
        case AND_BOOLEANTYPE:
        case OR_BOOLEANTYPE:
        case EQUALS_BOOLEANTYPE:
        case NOT_EQUALS_BOOLEANTYPE:
        case LESS_THAN_BOOLEANTYPE:
        case GREATER_THAN_BOOLEANTYPE:
        case GREATER_THAN_EQUALS_BOOLEANTYPE:
        case LESS_THAN_EQUALS_BOOLEANTYPE:
            DataType left = validateBooleanExpression(expr->boolean_expression_left);
            DataType right = validateBooleanExpression(expr->boolean_expression_right);
            if (left != right) {
                return BOTTOM;
            } else {
                return BOOLEAN_TYPE;
            }
        case NOTNULL_BOOLEANTYPE:
        case ISNULL_BOOLEANTYPE:
            return validateBooleanExpression(expr->unique_boolean_expression);
        case BOOLEAN_FACTOR_BOOLEANTYPE:
            return validateBooleanFactor(expr->boolean_factor);
    }
}

// Returns 1 if the constraint was a Primary Key constraint
static int validateLocalConstraint(char * attributeName, LocalConstraint * localConstraints) {
    DataType referrerDataType = type(attributeName);
    switch (localConstraints->type) {
    case PRIMARY_KEY_LCT:
        setIsPrimaryKey(attributeName);
        return 1;
    case UNIQUE_LCT:
        setIsUnique(attributeName);
        return 0;
    case FOREIGN_KEY_LCT:
        pushScope(localConstraints->id);
        char * primaryKeyName;
        if(!variableExistsInScope(attributeName)) {
            *errors = true;
            logError(_logger, "Referenced attribute '%s' doesn't exist in table '%s'.", attributeName, localConstraints->id);
        } else if ((primaryKeyName = getScopeTablePrimaryKey()) == NULL) {
            *errors = true;
            logError(_logger, "Table '%s' can't be referenced because it doesn't have a primary key.", localConstraints->id);
        } else if (!areTypesCompatible(referrerDataType, type(primaryKeyName))) {
            *errors = true;
            logError(_logger, "Referenced attribute '%s' in table '%s' has incompatible type with '%s'.", attributeName, localConstraints->id, attributeName);
        }
        popScope();
        return 0;
    case FOREING_KEY_DOUBLE_NAME_LCT:
        pushScope(localConstraints->id1);
        if(!variableExistsInScope(localConstraints->id2)) {
            *errors = true;
            logError(_logger, "Referenced attribute '%s' doesn't exist in table '%s'.", localConstraints->id2, localConstraints->id1);
        };
        if (!areTypesCompatible(referrerDataType, type(localConstraints->id2))) {
            *errors = true;
            logError(_logger, "Referenced attribute '%s' in table '%s' has incompatible type with '%s'.", localConstraints->id2, localConstraints->id1, attributeName);
        }
        popScope();
        return 0;
    case CHECK_LCT:
        validateBooleanExpression(localConstraints->checkConstraint->booleanExpression);
        return 0;
    }
}

static boolean isFunctionCompatible(DataType attrType, Function * function) {
    switch (function->functionType) {
    case GEN_RANDOM_UUID_OPEN_AND_CLOSE_PARENTHESIS_FUNCTION:
        return attrType == UUID_TYPE;
    case CURRENT_DATE_FUNCTION:
        return attrType == DATE_TYPE || attrType == TIMESTAMP_TYPE;
    case CURRENT_TIME_FUNCTION:
    case LOCALTIME_FUNCTION:
        return attrType = TIME_TYPE;
    case LOCALTIMESTAMP_FUNCTION:
    case CURRENT_TIMESTAMP_FUNCTION:
        return attrType == DATE_TYPE || attrType == TIME_TYPE || attrType == TIMESTAMP_TYPE;
    default:
        break;
    }
}


// Valida si el valor por defecto es compatible con el tipo del atributo
static boolean isDefaultValueCompatible(DataType attrType, DefaultValue * defaultValue) {
    switch (defaultValue->type) {
        case INTEGER_DEFAULT:
            return attrType == INTEGER_TYPE || attrType == SMALLINT_TYPE || attrType == BIGINT_TYPE;
        case DOUBLE_DEFAULT:
            return attrType == REAL_TYPE || attrType == DOUBLE_TYPE || attrType == FLOAT_TYPE;
        case STRING_DEFAULT:
            return attrType == TEXT_TYPE || attrType == CHAR_TYPE || attrType == VARCHAR_TYPE;
        case BOOLEAN_DEFAULT:
            return attrType == BOOLEAN_TYPE;
        case FUNCTION:
            return isFunctionCompatible(attrType, defaultValue->function);
        default:
            return false;
    }
}

static int validateAttributes(Content * content) {
    int primaryKeyCount = 0;
    while (content) {
        if (content->content_element->contentElementType == ATTRIBUTE_TYPE) {
            Attribute *attr = content->content_element->attribute;
            char * attrName = (attr->type == COLUMN) ? attr->id : attr->p_id;
            DataType attrType = (attr->type == COLUMN) ? attr->datatype->type : attr->p_type->type;
            if (variableExistsInScope(attrName)) {
                *errors = true;
                logError(_logger, "Attribute '%s' already exists.", attrName);
            } else {
                putVariableInScope(attrName, attrType);
            }

            if (attr->type == COLUMN_WITH_PROPERTIES) {
                if (attr->properties->type == DEFAULT_VALUE || attr->properties->type == NULL_CONDITION_DEFAULT_VALUE  || attr->properties->type == DEFAULT_VALUE_CONSTRAINT 
                    || attr->properties->type == COMPLETE ) {
                    DefaultValue * defVal = attr->properties->defaultValue;
                    if (!isDefaultValueCompatible(attrType, defVal)) {
                        *errors = true;
                        logError(_logger, "Cannot assign default value '%s' to type '%s'.", attrName, "type");
                        break;
                    }
                    DataValue dataValue;
                    switch (defVal->type) {
                        case INTEGER_DEFAULT:
                            dataValue.intValue = defVal->integer_value;
                            putValueInVariableInScope(attrName, dataValue);
                            break;
                        case DOUBLE_DEFAULT:
                            dataValue.doubleValue = defVal->double_value;
                            putValueInVariableInScope(attrName, dataValue);
                            break;
                        case BOOLEAN_DEFAULT:
                            dataValue.booleanValue = defVal->boolean_value;
                            putValueInVariableInScope(attrName, dataValue);
                            break;
                        case STRING_DEFAULT:
                            switch (attrType) {
                                case TIME_TYPE:
                                    if (isTime(defVal->string_value)) {
                                        dataValue.stringValue = defVal->string_value;
                                        putValueInVariableInScope(attrName, dataValue);
                                    }
                                    break;
                                case TIMESTAMP_TYPE:
                                    if (isTimestamp(defVal->string_value)) {
                                        dataValue.stringValue = defVal->string_value;
                                        putValueInVariableInScope(attrName, dataValue);
                                    }
                                    break;
                                case DATE_TYPE:
                                    if (isDate(defVal->string_value)) {
                                        dataValue.stringValue = defVal->string_value;
                                        putValueInVariableInScope(attrName, dataValue);
                                    }
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
                } else if (attr->properties->type == CONSTRAINT_CONDITION || attr->properties->type == DEFAULT_VALUE_CONSTRAINT  || attr->properties->type == NULL_CONDITION_CONSTRAINT 
                    || attr->properties->type == COMPLETE) {
                    primaryKeyCount += validateLocalConstraint(attrName, attr->properties->constraint);
                }
            }
        }
        content = content->content;
    }
    return primaryKeyCount;
}
                        
static int validateConstraints(Content * content) {
    int primaryKeyCount = 0;
    while (content) {
        if (content->content_element->contentElementType == CONSTRAINT_TYPE) {
            Constraint * constraint = content->content_element->constraint;
            ConstraintValue * cval = constraint->constraintValue;
            switch (cval->type) {
                case PRIMARY_KEY_CONSTRAINT_TYPE: {
                    Expression *expr = cval->expression;
                    while (expr) {
                        if (!variableExistsInScope(expr->id)) {
                            *errors = true;
                            logError(_logger, "Cannot add constraint to non-existent variable: '%s'.", expr->id);
                        } else {
                            setIsPrimaryKey(expr->id);
                        }
                        expr = expr->expression; 
                    }
                    primaryKeyCount++;
                    break;
                }
                case FOREIGN_KEY_CONSTRAINT_TYPE:
                    if (!tableExistsInSymbolTable(cval->id)) {
                        *errors = true;
                        logError(_logger, "Table '%s' not found.", cval->id);
                    } else {
                        Expression * expr = cval->singleExpression;
                        while (expr) {
                            if (!variableExistsInScope(expr->id)) {
                                *errors = true;
                                logError(_logger, "Cannot add constraint to non-existent variable: '%s'.", expr->id);
                            }
                            // Validate referenced column
                            pushScope(cval->id);
                            if (!variableExistsInScope(expr->id)) {
                                *errors = true;
                                logError(_logger, "Reference not found: '%s'.", expr->id);
                            } else if (!getIsPrimaryKey(expr->id) && !getIsUnique(expr->id)) {
                                *errors = true;
                                logError(_logger, "Cannot reference '%s' as it is not a primary key or unique.", expr->id);
                            }
                            popScope();
                            expr = expr->expression;
                        }
                    }
                    break;
                case FOREIGN_KEY_DOUBLE_EXPRESSION_CONSTRAINT_TYPE:
                    if (!tableExistsInSymbolTable(cval->idComplex)) {
                        *errors = true;
                        logError(_logger, "Table '%s' not found.", cval->idComplex);
                    } else {
                        Expression * expr1 = cval->mainExpression;
                        Expression * expr2 = cval->secondExpression;
                        while (expr1) {
                            if (expr2 == NULL) {
                                *errors = true;
                                logError(_logger, "The amount of columns in a foreign key on table '%s' doesn't match.", cval->idComplex);
                            }
                            if (!variableExistsInScope(expr1->id)) {
                                *errors = true;
                                logError(_logger, "Cannot add constraint to non-existent variable: '%s'.", expr1->id);
                            }
                            // Validate referenced column
                            pushScope(cval->idComplex);
                            if (!variableExistsInScope(expr2->id)) {
                                *errors = true;
                                logError(_logger, "Reference not found: '%s'.", expr2->id);
                            } else if (!getIsPrimaryKey(expr2->id) && !getIsUnique(expr2->id)) {
                                *errors = true;
                                logError(_logger, "Cannot reference '%s' as it is not a primary key or unique.", expr2->id);
                            }
                            popScope();
                            expr1 = expr1->expression;
                            expr2 = expr2->expression;
                        }
                    }
                    break;
                case CHECK_CONSTRAINT_TYPE:
                    validateBooleanExpression(cval->checkConstraint->booleanExpression);
                    break;
                case UNIQUE_CONSTRAINT_TYPE: 
                    Expression *expr = cval->expression;
                    while (expr) {
                        if (!variableExistsInScope(expr->id)) {
                            *errors = true;
                            logError(_logger, "Cannot add UNIQUE constraint to non-existent variable: '%s'.", expr->id);
                        } else {
                            setIsUnique(expr->id);
                        }
                        expr = expr->expression;
                    }
                    break;
                default:
                    break;
            }
        }
        content = content->content;
    }
    return primaryKeyCount;
}

static void validateTables(Tables * table) {
    char * tableName = table->id;

        addTableToSymbolTable(tableName);
        if (*errors) {
            return;
        }

        pushScope(tableName);
        int primaryKeyCount = 0;

        if (table->content) {
            primaryKeyCount += validateAttributes(table->content);
        }

        if (table->content) {
            primaryKeyCount += validateConstraints(table->content);
        }

        if (primaryKeyCount > 1) {
            *errors = true;
            logError(_logger, "Table '%s' cannot have more than one primary key.", tableName);
        }

        popScope();
}

static void validateTablesList(TablesList * tablesList) {
    if (tablesList->type == SINGULAR) {
        validateTables(tablesList->tables);
    } else {
        validateTablesList(tablesList->tablesList);
        validateTables(tablesList->tables);
    }
}

boolean semanticValidation(Program * program) {
    if (program == NULL) {
        *errors = true;
        logError(_logger, "Program is NULL");
        return false;
    }

    logInformation(_logger, "Starting semantic validation for the program");

    TablesList * tablesList = program->tablesList;
    validateTablesList(tablesList);

    logInformation(_logger, "Semantic validation finished");
    return true;
}



