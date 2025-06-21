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
    boolean isUnique;
} Symbol;

void initializeSymbolTableModule(CompilerState * compilerState);
void shutdownSymbolTableModule();
void addTableToSymbolTable(HashMap * symbolTable, const char *tableName);
int putVariableInScope(HashMap * symbolTable, const char * scope, const char * variableName, DataType dataType);
int putValueInVariableInScope(HashMap * symbolTable, const char * scope, const char * variableName, DataValue dataValue);
void setIsPrimaryKey(HashMap * symbolTable, const char * scope, const char * variableName);
void setIsUnique(HashMap * symbolTable, const char * scope, const char * variableName);
void destroySymbolTable(HashMap * symbolTable);
int variableExistsInScope(HashMap * symbolTable, const char * scope, const char * variableName);
boolean tableExistsInSymbolTable(HashMap * symbolTable, const char * tableName);
DataType type(HashMap * symbolTable, const char * scope, const char * variableName);
DataValue * getValue(HashMap * symbolTable, const char * scope, const char * variableName);
boolean getIsPrimaryKey(HashMap * symbolTable, const char * scope, const char * variableName);
boolean getIsUnique(HashMap * symbolTable, const char * scope, const char * variableName);
Symbol * getEntry(HashMap * symbolTable, const char * scope, const char * variableName);
boolean pushScope(Stack *scopeStack, char *scopeName);
char * popScope(Stack *scopeStack);
char * getScope(Stack * scopeStack);
#endif