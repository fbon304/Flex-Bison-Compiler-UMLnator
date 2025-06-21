
#include "SymbolTable.h"

static Logger * _logger = NULL;
static boolean * errors;

void initializeSymbolTableModule(CompilerState * compilerState) {
	_logger = createLogger("SymbolTable");
	errors = &compilerState->errors;
}

void shutdownSymbolTableModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/**
Adds a new table to the global symbolTable.
 */
void addTableToSymbolTable(HashMap * symbolTable, const char *tableName) {
	if(tableExistsInSymbolTable(symbolTable, tableName)) {
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

int putVariableInScope(HashMap * symbolTable, const char * scope, const char * variableName, DataValue dataValue, DataType dataType) {
	if(!tableExistsInSymbolTable(symbolTable, scope)) {
		logError(_logger, "Table '%s' doesn't exists in the symbol table, couldn't put .", symbolTable); 
		return -1;
	}
	
	if(variableExistsInScope(symbolTable, scope, variableName)) {
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
	
	put(get(symbolTable, scope), variableName, symbol);
	return 0;
}

int putValueInVariableInScope(HashMap * symbolTable, const char * scope, const char * variableName, DataValue dataValue, DataType dataType) {
	if(!tableExistsInSymbolTable(symbolTable, scope)) {
		logError(_logger, "Table '%s' doesn't exists in the symbol table, couldn't put .", symbolTable); 
		return -1;
	}
	
	if(variableExistsInScope(symbolTable, scope, variableName)) {
		logError(_logger, "Variable '%s' already exists in the scope '%s'.", variableName, scope);
		return -1; // Variable already exists in scope
	}

	Symbol * symbol = malloc(sizeof(Symbol));

	if(!symbol) {
		logError(_logger, "Failed to create a the new symbol.");
		return -1; // Failed to create new table
	}

	symbol->type = dataType;
	symbol->value = dataValue;
	
	put(get(symbolTable, scope), variableName, symbol);
	return 0;
}

int variableExistsInScope(HashMap * symbolTable, const char * scope, const char * variableName) {
    if (symbolTable == NULL) {
        logError(_logger, "Symbol table is NULL.");
        return -1;
    }
    HashMap * scopeTable = get(symbolTable, scope);
    if (scopeTable == NULL) {
        logError(_logger, "Scope '%s' does not exist in the symbol table.", scope);
        return 0;
    }
    return get(scopeTable, variableName) != NULL;
}

void destroySymbolTable(HashMap * symbolTable) {
	for(int i = 0; i < TABLE_SIZE; i++) {
		Entry * entry = symbolTable->buckets[i];
		while(entry) {
			destroyHashMap((HashMap *)entry->value);
			entry = entry->next;
		}
	}
	destroyHashMap(symbolTable);
}

char * getScope(Stack * scopeStack) {
	if (isEmpty(scopeStack)) {
		return NULL;
	}
	return peek(scopeStack);
}


boolean tableExistsInSymbolTable(HashMap * symbolTable, const char * tableName) {
	if(symbolTable == NULL || tableName == NULL) {
		return false;
	}

	return get(symbolTable, tableName) != NULL;
}

/**
 * Devuelve el tipo de dato de una variable en un scope dado.
 * Si no existe, devuelve BOTTOM.
 */
DataType type(HashMap * symbolTable, const char * scope, const char * variableName) {
    if (!tableExistsInSymbolTable(symbolTable, scope)) {
        logError(_logger, "Scope '%s' does not exist in the symbol table.", scope);
        return BOTTOM;
    }
    HashMap * scopeTable = get(symbolTable, scope);
    if (!scopeTable) {
        return BOTTOM;
    }
    Symbol * symbol = get(scopeTable, variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
        return BOTTOM;
    }
    return symbol->type;
}


/**
 * Devuelve un puntero al valor de una variable en un scope dado.
 * Si no existe, devuelve NULL.
 */
DataValue * getValue(HashMap * symbolTable, const char * scope, const char * variableName) {
    if (!tableExistsInSymbolTable(symbolTable, scope)) {
        logError(_logger, "Scope '%s' does not exist in the symbol table.", scope);
        return NULL;
    }
    HashMap * scopeTable = get(symbolTable, scope);
    if (!scopeTable) {
        return NULL;
    }
    Symbol * symbol = get(scopeTable, variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
        return NULL;
    }
    return &(symbol->value);
}

Symbol * getEntry(HashMap * symbolTable, const char * scope, const char * variableName) {
    if (!tableExistsInSymbolTable(symbolTable, scope)) {
        logError(_logger, "Scope '%s' does not exist in the symbol table.", scope);
        return NULL;
    }
    HashMap * scopeTable = get(symbolTable, scope);
    if (!scopeTable) {
        return NULL;
    }
    Symbol * symbol = get(scopeTable, variableName);
    if (!symbol) {
        logError(_logger, "Variable '%s' does not exist in scope '%s'.", variableName, scope);
        return NULL;
    }
    return &symbol;
}


/**
 * Agrega un nuevo scope (nombre de tabla) al stack de scopes.
 * Devuelve true si tuvo éxito, false si el stack está lleno.
 */
boolean pushScope(Stack * scopeStack, const char * scopeName) {
    if (isFull(scopeStack)) {
        logError(_logger, "Scope stack is full. Cannot push '%s'.", scopeName);
        return false;
    }
    // Hacemos una copia del nombre para evitar problemas de memoria
    char *copy = strdup(scopeName);
    if (!copy) {
        logError(_logger, "Failed to allocate memory for scope name.");
        return false;
    }
    return push(scopeStack, copy);
}

/**
 * Saca el scope actual del stack de scopes.
 * Devuelve el nombre del scope que se sacó, o NULL si el stack está vacío.
 * El caller debe liberar la memoria del nombre retornado.
 */
char * popScope(Stack * scopeStack) {
    if (isEmpty(scopeStack)) {
        logError(_logger, "Scope stack is empty. Cannot pop.");
        return NULL;
    }
    return pop(scopeStack);
}