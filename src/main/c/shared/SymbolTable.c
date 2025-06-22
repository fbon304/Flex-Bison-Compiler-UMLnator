
#include "SymbolTable.h"

static Logger * _logger = NULL;
static HashMap * symbolTable;
static Stack * scopeStack;
static boolean * errors;

void initializeSymbolTableModule(CompilerState * compilerState) {
	_logger = createLogger("SymbolTable");
	symbolTable = compilerState->symbolTable;
	scopeStack = compilerState->scopeStack;
	errors = &(compilerState->errors);
}

void shutdownSymbolTableModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/**
Adds a new table to the global symbolTable.
 */
void addTableToSymbolTable(const char * tableName) {
	if(tableExistsInSymbolTable(tableName)) {
		logError(_logger, "Table '%s' already exists in the symbol table.", tableName);
		*errors = true; 
		return;
	}
	HashMap * newTable = createHashMap();
	if(!newTable) {
		logError(_logger, "Failed to create a new hash map for table '%s'.", tableName);
		*errors = true;
		return;
	}
	put(symbolTable, tableName, newTable);
}

int putVariableInScope(const char * variableName, DataType dataType) {
	char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
		return -1;
	}
	
	if(variableExistsInScope(variableName)) {
		logError(_logger, "Variable '%s' already exists in the scope '%s'.", variableName, scope);
		return -1; // Variable already exists in scope
	}

	Symbol * symbol = malloc(sizeof(Symbol));

	if(!symbol) {
		logError(_logger, "Failed to create a the new symbol.");
		return -1; // Failed to create new table
	}

	symbol->type = dataType;
	symbol->value.stringValue = NULL;
	symbol->isPrimaryKey = false;
	
	put(get(symbolTable, scope), variableName, symbol);
	return 0;
}

int putValueInVariableInScope(const char * variableName, DataValue dataValue) {
	char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
		return -1;
	}

	Symbol * symbol = get(get(symbolTable, scope), variableName);
	if(!symbol) {
		logError(_logger, "Variable '%s' doesn't exists in the scope '%s'.", variableName, scope);
		return -1;
	}

	symbol->value = dataValue;
	
	put(get(symbolTable, scope), variableName, symbol);
	return 0;
}

int variableExistsInScope(const char * variableName) {
    char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
        return -1;
    }
    HashMap * scopeTable = get(symbolTable, scope);
    if (scopeTable == NULL) {
        logError(_logger, "Scope '%s' does not exist in the symbol table.", scope);
        return 0;
    }
    return get(scopeTable, variableName) != NULL;
}

void destroySymbolTable() {
	for(int i = 0; i < TABLE_SIZE; i++) {
		Entry * entry = symbolTable->buckets[i];
		while(entry) {
			destroyHashMap((HashMap *)entry->value);
			entry = entry->next;
		}
	}
	destroyHashMap(symbolTable);
}

boolean tableExistsInSymbolTable(const char * tableName) {
	if(tableName == NULL) {
		return false;
	}

	return get(symbolTable, tableName) != NULL;
}

/**
 * Devuelve el tipo de dato de una variable en un scope dado.
 * Si no existe, devuelve BOTTOM.
 */
DataType type(const char * variableName) {
    char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
        return BOTTOM;
    }
    HashMap * scopeTable = get(symbolTable, scope);
    if (scopeTable == NULL) {
		logError(_logger, "Table '%s' doesn't exists in symbolTable", scope);
		*errors = true;
        return BOTTOM;
    }
    Symbol * symbol = get(scopeTable, variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
        return BOTTOM;
    }
    return symbol->type;
}

DataValue * getValue(const char * variableName) {
    char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
		return NULL;
    }
    Symbol * symbol = get(get(symbolTable, scope), variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
        *errors = true;
		return NULL;
    }
    return &(symbol->value);
}

boolean getIsPrimaryKey(const char * variableName) {
    char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
		return false;
    }
    Symbol * symbol = get(get(symbolTable, scope), variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
        *errors = true;
		return false;
    }
    return symbol->isPrimaryKey;
}

boolean getIsUnique(const char * variableName) {
    char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
		return false;
    }
    Symbol * symbol = get(get(symbolTable, scope), variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
		*errors = true;
		return false;
    }
    return symbol->isUnique;
}

void setIsPrimaryKey(const char * variableName) {
    char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
		return;
    }
    Symbol * symbol = get(get(symbolTable, scope), variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
        *errors = true;
		return;
    }
	symbol->isPrimaryKey = true;
}

void setIsUnique(const char * variableName) {
    char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
		return;
    }
    Symbol * symbol = get(get(symbolTable, scope), variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
        *errors = true;
		return;
    }
	symbol->isUnique = true;
}

Symbol * getEntry(const char * variableName) {
	char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack for variable '%s' ", variableName);
		*errors = true;
        return NULL;
    }
    Symbol * symbol = get(get(symbolTable, scope), variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
        return NULL;
    }
    return symbol;
}

char * getScopeTablePrimaryKey() {
	char * scope = getScope();
    if (scope == NULL) {
        logError(_logger, "Couldn't get scope from stack");
		*errors = true;
		return NULL;
	}
	HashMap * map = get(symbolTable, scope);
	if (map == NULL) {
        logError(_logger, "Couldn't get scopeTable '%s' form symbolTable");
		*errors = true;
		return NULL;
	}
	for (int i = 0; i < TABLE_SIZE; i++) {
		Entry * current = map->buckets[i];

    	while (current) {
        	if (((Symbol *)(current->value))->isPrimaryKey) {
				return current->key;
			}
        	current = current->next;
    	}
	}
	return NULL;
}

/**
 * Agrega un nuevo scope (nombre de tabla) al stack de scopes.
 * Devuelve true si tuvo éxito, false si el stack está lleno.
 */
boolean pushScope(char * scopeName) {
    if (isFull(scopeStack)) {
        logError(_logger, "Scope stack is full. Cannot push '%s'.", scopeName);
        return false;
    }
    return push(scopeStack, scopeName);
}

/**
 * Saca el scope actual del stack de scopes.
 * Devuelve el nombre del scope que se sacó, o NULL si el stack está vacío.
 * El caller debe liberar la memoria del nombre retornado.
 */
char * popScope() {
    if (isEmpty(scopeStack)) {
        logError(_logger, "Scope stack is empty. Cannot pop.");
        return NULL;
    }
    return pop(scopeStack);
}

char * getScope() {
	if (isEmpty(scopeStack)) {
		return NULL;
	}
	return peek(scopeStack);
}