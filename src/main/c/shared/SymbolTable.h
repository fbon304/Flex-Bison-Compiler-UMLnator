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

void initializeSymbolTableModule();
void shutdownSymbolTableModule();
int addTableToSymbolTable(HashMap * symbolTable, const char *tableName);
int putVariableInScope(HashMap * symbolTable, const char * scope, const char * variableName, DataValue dataValue, DataType dataType, ConstarintDataType constraint);
void destroySymbolTable(HashMap * symbolTable);
int variableExistsInScope(HashMap * symbolTable, const char * scope, const char * variableName);
boolean tableExistsInSymbolTable(HashMap * symbolTable, const char * tableName);
DataType type(HashMap * symbolTable, const char * scope, const char * variableName);
DataValue* getValue(HashMap * symbolTable, const char * scope, const char * variableName);
boolean pushScope(Stack *scopeStack, const char *scopeName);
char *popScope(Stack *scopeStack);
#endif