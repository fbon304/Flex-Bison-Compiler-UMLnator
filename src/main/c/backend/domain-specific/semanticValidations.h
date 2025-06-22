#ifndef SEMANTIC_VALIDATIONS
#define SEMANTIC_VALIDATIONS

#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../../shared/CompilerState.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdio.h>
#include "../../shared/SymbolTable.h"
#include <string.h>

void initializeSemanticValidationsModule(CompilerState * compilerState);
void shutdownSemanticValidationsModule();
boolean semanticValidation(Program * program);

#endif