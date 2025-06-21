#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Logger.h"
#include "Type.h"
#include "CompilerState.h"
#include "DataStructures/HashMap.h"
#include "DataStructures/Stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union DataValue {
	int64_t intValue; // For integer, smallint, bigint, serial, timestamp.
	double doubleValue; // For real, double, float, number.
	char * stringValue; // For text, varchar, char, date, interval, time.
	boolean booleanValue; // For boolean
} DataValue;

typedef struct Symbol {
    DataType type;
	DataValue value;
    boolean isPrimaryKey;
} Symbol;

void initializeSymbolTableModule(CompilerState * compilerState);
void shutdownSymbolTableModule();
void addTableToSymbolTable(HashMap * symbolTable, const char *tableName);
int putVariableInScope(HashMap * symbolTable, const char * scope, const char * variableName, DataValue dataValue, DataType dataType);
void destroySymbolTable(HashMap * symbolTable);
int variableExistsInScope(HashMap * symbolTable, const char * scope, const char * variableName);
boolean tableExistsInSymbolTable(HashMap * symbolTable, const char * tableName);
DataType type(HashMap * symbolTable, const char * scope, const char * variableName);
DataValue * getValue(HashMap * symbolTable, const char * scope, const char * variableName);
Symbol * symbol getEntry(HashMap * symbolTable, const char * scope, const char * variableName);
boolean pushScope(Stack *scopeStack, const char *scopeName);
char *popScope(Stack *scopeStack);
#endif