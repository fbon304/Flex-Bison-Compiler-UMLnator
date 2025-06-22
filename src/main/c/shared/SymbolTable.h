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
	char * stringValue; // For text, varchar, char, date, time.
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
void addTableToSymbolTable(const char *tableName);
int putVariableInScope(const char * variableName, DataType dataType);
int putValueInVariableInScope(const char * variableName, DataValue dataValue);
void setIsPrimaryKey(const char * variableName);
void setIsUnique(const char * variableName);
void destroySymbolTable();
int variableExistsInScope(const char * variableName);
boolean tableExistsInSymbolTable(const char * tableName);
DataType type(const char * variableName);
DataValue * getValue(const char * variableName);
boolean getIsPrimaryKey(const char * variableName);
boolean getIsUnique(const char * variableName);
Symbol * getEntry(const char * variableName);
boolean pushScope(char * scopeName);
char * popScope();
char * getScope();
#endif