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

IsCondition * IsConditionSemanticAction(IsConditionType type);

BooleanValue * BooleanValueSemanticAction(BooleanType type);

Factor * StringFactorSemanticAction(char * value, FactorType type);
Factor * DoubleFactorSemanticAction(double value);
Factor * IntegerFactorSemanticAction(int value);

BooleanExpression * SimpleBooleanExpressionSemanticAction(BooleanExpression * booleanExpression, BooleanExpressionType type);
BooleanExpression * DoubleBooleanExpressionSemanticAction(BooleanExpression * leftBooleanExpression, BooleanExpression * rightBooleanExpression, BooleanExpressionType type);
BooleanExpression * BooleanFactorBooleanExpressionSemanticAction(BooleanFactor * booleanFactor);

BooleanFactor * IsConditionBooleanFactorSemanticAction(BooleanFactor * booleanExpression, IsCondition * isCondition);
BooleanFactor * TriplePointerBooleanFactorSemanticAction(BooleanFactor * booleanExpression, IsCondition * isCondition, BooleanValue * booleanValue);
BooleanFactor * FactorsBooleanFactorSemanticAction(Factor * leftFactor, IsCondition * isCondition, Factor * rightFactor);
BooleanFactor * FactorBooleanFactorSemanticAction(Factor * factor);
BooleanFactor * BooleanValueBooleanFactorSemanticAction(BooleanValue * booleanValue);
BooleanFactor * BooleanExpressionFactorSemanticAction(BooleanExpression * booleanExpression, BooleanFactorType type);

CheckConstraint * BooleanExpressionCheckConstraint(BooleanExpression * booleanExpression);

Expression * ComplexExpressionSemanticAction(char * id, Expression * expression);
Expression * IdExpressionSemanticAction(char * id);

Action * ActionSemanticAction(ActionType actiontype);

OnAction * OnActionSemanticAction(Action * action, ActionType actionType);
OnAction * EmptyActionSemanticAction();
OnAction * DoubleActionSemanticAction(Action * deleteAction, Action * updateAction);

ConstraintValue * CheckConstraintValueSemanticAction(CheckConstraint * checkConstraint);
ConstraintValue * SingleExpressionConstraintValueSemanticAction(Expression * expression, ConstraintValueType type);
ConstraintValue * SingleForeignConstraintValueSemanticAction(Expression * expression, char * name, OnAction * onAction);
ConstraintValue * DoubleForeignConstraintValueSemanticAction(Expression * expression1, char * name, Expression * expression2, OnAction * onAction);

Constraint * NamedConstraintValueSemanticAction(char * id, ConstraintValue * constraintValue);
Constraint * UnnamedConstraintValueSemanticAction(ConstraintValue * constraintValue);

LocalConstraint * NameOnActionLocalConstraintSemanticAction(char * name, OnAction * onAction);
LocalConstraint *  SimpleLocalConstraintSemanticAction(LocalConstraintType type);
LocalConstraint * DoubleNameOnActionLocalConstraintSemanticAction(char * id1, char * id2, OnAction * OnAction);
LocalConstraint * CheckLocalConstraintSemanticAction(CheckConstraint * checkConstraint);

Function * FunctionSemanticAction(FunctionType functionType);

NullCondition * NullConditionSemanticAction(NullConditionType type);

Type * SimpleTypeSemanticAction(DataTypeType type);
Type * ComplexTypeSemanticAction(int param, DataTypeType type);
Type * DoubleComplexTypeSemanticAction(int param1, int param2, DataTypeType type);

Attribute * AttributeTypeSemanticAction(char * id, Type * type);
Attribute * AttributeTypePropertiesSemanticAction(char * id, Type * type, Properties * properties);

Content * SimpleContentSemanticAction(ContentElement * contentElement);
Content * MultiContentSemanticAction(Content * content, ContentElement * contentElement);

ContentElement * ContentElementAttributeSemanticAction(Attribute * attribute);
ContentElement * ContentElementConstraintSemanticAction(Constraint * constraint);

Tables * ContentTablesSemanticAction(char * id, Content * content);

TablesList * TableListSingleTableSemanticAction(Tables * tables);
TablesList * TableListMultiTableSemanticAction(TablesList * tablesList, Tables * tables);

Program * TablesListProgramSemanticAction(CompilerState * compilerState, TablesList * tablesList);

Properties * SimpleDefaultPropertySemanticAction(DefaultValue * defaultValue, PropertiesType type);
Properties * SimpleConstraintPropertySemanticAction(LocalConstraint * constraint, PropertiesType type);
Properties * SimpleNullPropertySemanticAction(NullCondition * nullCondition, PropertiesType type);

Properties * DoubleDefaultConstraintPropertySemanticAction(DefaultValue * defaultValue, LocalConstraint * constraint, PropertiesType type);
Properties * DoubleDefaultNullPropertySemanticAction(DefaultValue * defaultValue, NullCondition * nullCondition, PropertiesType type);
Properties * DoubleConstraintNullPropertySemanticAction(LocalConstraint * constraint, NullCondition * nullCondition, PropertiesType type);

Properties * TriplePropertySemanticAction(DefaultValue * defaultValue, LocalConstraint * constraint, NullCondition * nullCondition);

DefaultValue * DefaultValueIntegerTerminalSemanticAction(int value);
DefaultValue * DefaultValueDoubleTerminalSemanticAction(double value);
DefaultValue * DefaultValueStringTerminalSemanticAction(char * value);
DefaultValue * DefaultValueNonTerminalSemanticAction(Function * function);

#endif
