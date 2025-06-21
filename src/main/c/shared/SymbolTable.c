
#include "SymbolTable.h"
#include "CompilerState.h"

static Logger * _logger = NULL;

void initializeSymbolTableModule() {
	_logger = createLogger("SymbolTable");
}

void shutdownSymbolTableModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}



/**
Agrega una nueva tabla a la symbolTable global.
Devuelve 0 si hubo éxito y -1 si ya existe la table.
 */
int addTableToSymbolTable(HashMap * symbolTable, const char *tableName) {
	if(get(symbolTable, tableName) != NULL) {
		logError(_logger, "Table '%s' already exists in the symbol table.", tableName);
		return -1; // Table already exists //TODO check
	}
	HashMap * newTable = createHashMap();
	if(!newTable) {
		logError(_logger, "Failed to create a new hash map for table '%s'.", tableName);
		return -1; // Failed to create new table
	}
	put(symbolTable, tableName, newTable);
	return 0;
}

int putVariableInScope(HashMap * symbolTable, const char * scope, const char * variableName, DataValue dataValue, DataType dataType) {
	if(get(symbolTable, scope) == NULL) {
		logError(_logger, "Table '%s' doesn't exists in the symbol table.", symbolTable);
		return -1; // Table already exists //TODO check
	}
	//TODO
	
	if(variableExistsInScope(symbolTable, scope, variableName)) {
		logError(_logger, "Variable '%s' already exists in the scope '%s'.", variableName, scope);
		return -1; // Variable already exists
	}

	Symbol * symbol = malloc(sizeof(Symbol));

	if(!symbol) {
		logError(_logger, "Failed to create a the new symbol.");
		return -1; // Failed to create new table
	}

	symbol->type = dataType;
	symbol->value = dataValue;
	
	put(scope, variableName, symbol);
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


/*
TODO

preguntar si existe una tabla
la funcion de type, devuelve el tipo de dato. Se va a usar para haceer las validaciones de semántica.
funion getValue, devuelve el valor si lo tiene
popScope
pushScope


*/