#include "semanticValidations.h"

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

// Verifica si un atributo es primary key en la tabla de símbolos
static boolean isPrimaryKey(HashMap * symbolTable, const char * tableName, const char * attrName) {
    Symbol * symbol = getEntry(symbolTable, tableName, attrName);
    if (symbol && symbol->isPrimaryKey) {
        return true;
    }
    return false;
}

// Compara tipos
static boolean areTypesCompatible(DataType t1, DataType t2) {
    return t1 == t2;
}


// Valida si el valor por defecto es compatible con el tipo del atributo
static boolean isDefaultValueCompatible(DataType attrType, DefaultValue * defaultValue) {
    if (!defaultValue) return true;
    switch (defaultValue->type) {
        case INTEGER_DEFAULT:
            return attrType == INTEGER_TYPE || attrType == SMALLINT_TYPE || attrType == BIGINT_TYPE;
        case DOUBLE_DEFAULT:
            return attrType == REAL_TYPE || attrType == DOUBLE_TYPE || attrType == FLOAT_TYPE;
        case STRING_DEFAULT:
            return attrType == TEXT_TYPE || attrType == CHAR_TYPE || attrType == VARCHAR_TYPE;
        case FUNCTION:
            // Aquí podrías validar funciones especiales según el tipo
            return true;
        default:
            return false;
    }
}


boolean semanticValidation(Program * program) {
    if (program == NULL) {
        *errors = true;
        logError(_logger, "Program is NULL");
        return false;
    }

    logInfo(_logger, "Starting semantic validation for the program");

    // Crear tabla de símbolos global
    HashMap * symbolTable = createHashMap();

    // Recorrer todas las tablas del programa
    TablesList * tablesList = program->tablesList;
    while (tablesList) {
        Tables * table = tablesList->tables;
        const char * tableName = table->id;

        // Validar tabla duplicada
        if (tableExistsInSymbolTable(symbolTable, tableName)) {
            *errors = true;
            logError(_logger, "Table '%s' already exists.", tableName);
        } else {
            addTableToSymbolTable(symbolTable, tableName);
        }

        // Validar atributos duplicados y agregar atributos a la tabla de símbolos
        if (table->content) {
            Content * content = table->content;
            while (content) {
                if (content->content_element->contentElementType == ATTRIBUTE_TYPE) {
                    Attribute * attr = content->content_element->attribute;
                    const char * attrName = (attr->type == COLUMN) ? attr->id : attr->p_id;
                    DataType attrType = (attr->type == COLUMN) ? attr->datatype->type : attr->p_type->type;
                    if (variableExistsInScope(symbolTable, tableName, attrName)) {
                        *errors = true;
                        logError(_logger, "Attribute '%s' already exists.", attrName);
                    } else {
                        putVariableInScope(symbolTable, tableName, attrName, attrType);
                    }

                    // Validar valor por defecto compatible con tipo
                    if (attr->type == COLUMN_WITH_PROPERTIES && attr->properties && attr->properties->type == DEFAULT_VALUE) {
                        DefaultValue * defVal = attr->properties->defaultValue;
                        if (!isDefaultValueCompatible(attrType, defVal)) {
                           *errors = true;
                            logError(_logger, "Cannot assign default value '%s' to type '%s'.", attrName, "type");
                        }
                    }
                }
                content = content->content;
            }
        }

        // Validar constraints (primary key, foreign key, etc.)
        int primaryKeyCount = 0;
        if (table->content) {
            Content * content = table->content;
            while (content) {
                if (content->content_element->contentElementType == CONSTRAINT_TYPE) {
                    Constraint * constraint = content->content_element->constraint;
                    ConstraintValue * cval = (constraint->type == NAMED_CONSTRAINT) ? constraint->constraintValue : constraint->singleConstraintValue;

                    if (cval) {
                        switch (cval->type) {
                            case PRIMARY_KEY_CONSTRAINT_TYPE:
                                // Validar que los atributos existan y marcar como PK
                                {
                                    Expression * expr = cval->expression;
                                    while (expr) {
                                        if (!variableExistsInScope(symbolTable, tableName, expr->id)) {
                                            *errors = true;
                                            logError(_logger, "Cannot add constraint to non-existent variable: '%s'.", expr->id);
                                        } else {
                                            Symbol * symbol = getEntry(symbolTable, tableName, expr->id);
                                            if (symbol) symbol->isPrimaryKey = true;
                                        }
                                        expr = expr->expression;
                                    }
                                    primaryKeyCount++;
                                }
                                break;
                            case FOREIGN_KEY_CONSTRAINT_TYPE:
                                // Validar tabla y columna referenciada
                                {
                                    if (!tableExistsInSymbolTable(symbolTable, cval->id)) {
                                        *errors = true;
                                        logError(_logger, "Table '%s' not found.", cval->id);
                                    } else {
                                        Expression * expr = cval->singleExpression;
                                        while (expr) {
                                            if (!variableExistsInScope(symbolTable, tableName, expr->id)) {
                                               *errors = true;
                                                logError(_logger, "Cannot add constraint to non-existent variable: '%s'.", expr->id);
                                            }
                                            // Validar columna referenciada
                                            if (!variableExistsInScope(symbolTable, cval->id, expr->id)) {
                                                *errors = true;
                                                logError(_logger, "Reference not found: '%s'.", expr->id);
                                            } else if (!isPrimaryKey(symbolTable, cval->id, expr->id)) {
                                                *errors = true;
                                                logError(_logger, "Cannot reference '%s' as it is not a primary key.", expr->id);
                                            }
                                            expr = expr->expression;
                                        }
                                    }
                                }
                                break;
                            case CHECK_CONSTRAINT_TYPE:
                                // Aquí podrías validar tipos en la expresión booleana
                                break;
                            default:
                                break;
                        }
                    }
                }
                content = content->content;
            }
        }


        if (primaryKeyCount > 1) {
            *errors = true;
            logError(_logger, "Table '%s' cannot have more than one primary key.", tableName);
        }

        tablesList = tablesList->tablesList;
    }

    // Liberar tabla de símbolos
    destroyHashMap(symbolTable);

    logInfo(_logger, "Semantic validation finished");
    return true;
}



