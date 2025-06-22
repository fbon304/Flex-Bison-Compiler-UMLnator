#include "semanticValidations.h"

static Logger * _logger = NULL;
static boolean * errors;
static HashMap * symbolTable;

void initializeSemanticValidationsModule(CompilerState * compilerState) {
	_logger = createLogger("SemanticValidations");
    errors = &compilerState->errors;
    symbolTable = compilerState->symbolTable;
}

void shutdownSemanticValidationsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

// Verifica si un atributo es primary key en la tabla de símbolos
// Inecesaria
/*
static boolean isPrimaryKey(HashMap * symbolTable, const char * tableName, const char * attrName) {
    Symbol * symbol = getEntry(symbolTable, tableName, attrName);
    if (symbol && symbol->isPrimaryKey) {
        return true;
    }
    return false;
}*/

// Compara tipos
static boolean areTypesCompatible(DataType t1, DataType t2) {
    return t1 == t2;
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

// Valida atributos duplicados y valores por defecto
static void validateAttributes(const char *tableName, Content * content) {
    while (content) {
        if (content->content_element->contentElementType == ATTRIBUTE_TYPE) {
            Attribute *attr = content->content_element->attribute;
            const char * attrName = (attr->type == COLUMN) ? attr->id : attr->p_id;
            DataType attrType = (attr->type == COLUMN) ? attr->datatype->type : attr->p_type->type;
            if (variableExistsInScope(symbolTable, tableName, attrName)) {
                *errors = true;
                logError(_logger, "Attribute '%s' already exists.", attrName);
            } else {
                putVariableInScope(symbolTable, tableName, attrName, attrType);
            }

            // Validar valor por defecto compatible con tipo
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
                        putValueInVariableInScope(symbolTable, tableName, attrName, dataValue);
                        break;

                    case DOUBLE_DEFAULT:
                        dataValue.doubleValue = defVal->double_value;
                        putValueInVariableInScope(symbolTable, tableName, attrName, dataValue);
                        break;
                    case BOOLEAN_DEFAULT:
                        dataValue.booleanValue = defVal->boolean_value;
                        putValueInVariableInScope(symbolTable, tableName, attrName, dataValue);
                        break;
                
                    case STRING_DEFAULT:
                        dataValue.stringValue = defVal->string_value;
                        putValueInVariableInScope(symbolTable, tableName, attrName, dataValue);
                        break;

                    default:
                        break;
                    }
                } else if (attr->properties->type == CONSTRAINT_CONDITION || attr->properties->type == DEFAULT_VALUE_CONSTRAINT  || attr->properties->type == NULL_CONDITION_CONSTRAINT 
                    || attr->properties->type == COMPLETE) {
                    // TODO validate for local contraints
                }
            }
        }
        content = content->content;
    }
}

static void validateBooleanFactor(const char *tableName, BooleanFactor *factor) {
    if (!factor) return;

    switch (factor->type) {
        case FACTOR_TYPE:
            if (factor->factor && factor->factor->type == ID_FACTOR_TYPE) {
                if (!variableExistsInScope(symbolTable, tableName, factor->factor->string)) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint does not exist.", factor->factor->string);
                }
            }
            break;
        case FACTOR_CONDITION_FACTOR:
            // factor_left IS_CONDITION/IS_NOT_CONDITION factor_right
            if (factor->factor_left && factor->factor_left->type == ID_FACTOR_TYPE) {
                if (!variableExistsInScope(symbolTable, tableName, factor->factor_left->string)) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint does not exist.", factor->factor_left->string);
                }
            }
            if (factor->isCondition_with_two_factors) {
                //validateIsCondition(tableName, factor->isCondition_with_two_factors);
                // TODO CHECK SOMETHING ELSE
            }
            if (factor->factor_right && factor->factor_right->type == ID_FACTOR_TYPE) {
                if (!variableExistsInScope(symbolTable, tableName, factor->factor_right->string)) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint does not exist.", factor->factor_right->string);
                }
            }
            break;
        case FACTOR_CONDITION_NUL:
            // factor_with_is_condition IS_CONDITION/IS_NOT_CONDITION NUL
            if (factor->factor_with_is_condition && factor->factor_with_is_condition->type == ID_FACTOR_TYPE) {
                if (!variableExistsInScope(symbolTable, tableName, factor->factor_with_is_condition->string)) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint does not exist.", factor->factor_with_is_condition->string);
                }
            }
            if (factor->is_condition_with_factor) {
                //validateIsCondition(tableName, factor->is_condition_with_factor);
                // TODO CHECK SOMETHING ELSE
            }
            break;
        case DISTINCT_FROM_FACTORS:
            if (factor->factor_left && factor->factor_left->type == ID_FACTOR_TYPE) {
                if (!variableExistsInScope(symbolTable, tableName, factor->factor_left->string)) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint does not exist.", factor->factor_left->string);
                }
            }
            if (factor->factor_right && factor->factor_right->type == ID_FACTOR_TYPE) {
                if (!variableExistsInScope(symbolTable, tableName, factor->factor_right->string)) {
                    *errors = true;
                    logError(_logger, "Column '%s' in CHECK constraint does not exist.", factor->factor_right->string);
                }
            }
            break;
        case BOOLEAN_EXPRESSION_PARENTHESIS_TYPE:
        case NOT_BOOLEAN_EXPRESSION:
            if (factor->booleanExpression) {
                validateBooleanExpression(tableName, factor->booleanExpression);
            }
            break;
        default:
            break;
    }
}


// Valida que los identificadores usados en la expresión booleana existan y sean del tipo adecuado
static void validateBooleanExpression(const char *tableName, BooleanExpression *expr) {
    if (!expr) return;

    switch (expr->type) {
        case AND_BOOLEANTYPE:
        case OR_BOOLEANTYPE:
        case EQUALS_BOOLEANTYPE:
        case NOT_EQUALS_BOOLEANTYPE:
        case LESS_THAN_BOOLEANTYPE:
        case GREATER_THAN_BOOLEANTYPE:
        case GREATER_THAN_EQUALS_BOOLEANTYPE:
        case LESS_THAN_EQUALS_BOOLEANTYPE:
            validateBooleanExpression(tableName, expr->boolean_expression_left);
            validateBooleanExpression(tableName, expr->boolean_expression_right);
            break;
        case NOTNULL_BOOLEANTYPE:
        case ISNULL_BOOLEANTYPE:
            validateBooleanExpression(tableName, expr->unique_boolean_expression);
            break;
        case BOOLEAN_FACTOR_BOOLEANTYPE:
            if (expr->boolean_factor) {
                validateBooleanFactor(tableName, expr->boolean_factor);
            }
            break;
        default:
            break;
    }
}
                        

// Valida constraints y retorna la cantidad de primary keys encontrados
static int validateConstraints(const char *tableName, Content *content) {
    int primaryKeyCount = 0;
    while (content) {
        if (content->content_element->contentElementType == CONSTRAINT_TYPE) {
            Constraint *constraint = content->content_element->constraint;
            ConstraintValue *cval = (constraint->type == NAMED_CONSTRAINT) ? constraint->constraintValue : constraint->singleConstraintValue;

            if (cval) {
                switch (cval->type) {
                    case PRIMARY_KEY_CONSTRAINT_TYPE: {
                        Expression *expr = cval->expression;
                        while (expr) {
                            if (!variableExistsInScope(symbolTable, tableName, expr->id)) {
                                *errors = true;
                                logError(_logger, "Cannot add constraint to non-existent variable: '%s'.", expr->id);
                            } else {
                                Symbol *symbol = getEntry(symbolTable, tableName, expr->id);
                                if (symbol) symbol->isPrimaryKey = true;
                            }
                            expr = expr->expression;
                        }
                        primaryKeyCount++;
                        break;
                    }
                    case FOREIGN_KEY_CONSTRAINT_TYPE: {
                        if (!tableExistsInSymbolTable(symbolTable, cval->id)) {
                            *errors = true;
                            logError(_logger, "Table '%s' not found.", cval->id);
                        } else {
                            Expression *expr = cval->singleExpression;
                            while (expr) {
                                if (!variableExistsInScope(symbolTable, tableName, expr->id)) {
                                    *errors = true;
                                    logError(_logger, "Cannot add constraint to non-existent variable: '%s'.", expr->id);
                                }
                                // Validar columna referenciada
                                if (!variableExistsInScope(symbolTable, cval->id, expr->id)) {
                                    *errors = true;
                                    logError(_logger, "Reference not found: '%s'.", expr->id);
                                } else if (!getIsPrimaryKey(symbolTable, cval->id, expr->id)) {
                                    *errors = true;
                                    logError(_logger, "Cannot reference '%s' as it is not a primary key.", expr->id);
                                }
                                expr = expr->expression;
                            }
                        }
                        break;
                    }
                    case CHECK_CONSTRAINT_TYPE:
                        // Aquí podrías validar tipos en la expresión booleana
                        // Por ejemplo podríamos tener: (((attr != 4) and (age > 7)) or (isTall = true))
                        // Validar que las columnas usadas en la expresión existan
                        if (cval->checkConstraint && cval->checkConstraint->booleanExpression) {
                            validateBooleanExpression(tableName, cval->checkConstraint->booleanExpression);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        content = content->content;
    }
    return primaryKeyCount;
}

boolean semanticValidation(Program * program) {
    if (program == NULL) {
        *errors = true;
        logError(_logger, "Program is NULL");
        return false;
    }

    logInfo(_logger, "Starting semantic validation for the program");

    TablesList *tablesList = program->tablesList;
    while (tablesList) {
        Tables *table = tablesList->tables;
        const char *tableName = table->id;

        addTableToSymbolTable(symbolTable, tableName);
        if (*errors) {
            break;
        }

        if (table->content) {
            validateAttributes(tableName, table->content);
        }

        int primaryKeyCount = 0;
        if (table->content) {
            primaryKeyCount = validateConstraints(tableName, table->content);
        }

        if (primaryKeyCount > 1) {
            *errors = true;
            logError(_logger, "Table '%s' cannot have more than one primary key.", tableName);
        }

        tablesList = tablesList->tablesList;
    }

    destroyHashMap(symbolTable);

    logInfo(_logger, "Semantic validation finished");
    return true;
}



