#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

/*
Constant * IntegerConstantSemanticAction(const int value);
Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression); 
*/

Content * AttributesContentSemanticAction(Attributes * attributes);
Content * ConstraintsContentSemanticAction(Constraints * constraints);
Content * AttributesAndConstraintsContentSemanticAction(Attributes * attributes, Constraints * constraints);
Content * EmptyContentSemanticAction();
Tables * ContentTablesSemanticAction(char * id, Content * content);
Tables * TableGenerateSemanticAction(Tables * tables1, Tables *tables2);
Program * TablesProgramSemanticAction(CompilerState * compilerState, Tables * tables);

#endif
