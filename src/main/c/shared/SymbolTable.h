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
#endif