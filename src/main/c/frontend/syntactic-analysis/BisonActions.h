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

BooleanExpression * FactorsBooleanExpressionSemanticAction(Factor * leftFactor, IsCondition * isCondition, Factor * rightFactor);
BooleanExpression * BooleanValueBooleanExpressionSemanticAction(BooleanValue * booleanValue);
BooleanExpression * SimpleBooleanExpressionSemanticAction(BooleanExpression * booleanExpression, BooleanExpressionType type);
BooleanExpression * IsConditionBooleanExpressionSemanticAction(BooleanExpression * booleanExpression, IsCondition * isCondition);
BooleanExpression * TriplePointerBooleanExpressionSemanticAction(BooleanExpression * booleanExpression, IsCondition * isCondition, BooleanValue * booleanValue);
BooleanExpression * DoubleBooleanExpressionSemanticAction(BooleanExpression * leftBooleanExpression, BooleanExpression * rightBooleanExpression, BooleanExpressionType type);

CheckConstraint * BooleanExpressionCheckConstraint(BooleanExpression * booleanExpression);

Expression * ComplexExpressionSemanticAction(char * id, Expression * expression)
Expression * IdExpressionSemanticAction(char * id);

Action * ActionSemanticAction(ActionType actiontype);

OnAction * OnActionSemanticAction(Action * action, ActionType actionType);
OnAction * EmptyActionSemanticAction();

ConstraintValue * CheckConstraintValueSemanticAction(CheckConstraint * checkConstraint);
ConstraintValue * SingleExpressionConstraintValueSemanticAction(Expression * expression, ConstraintValueType type);
ConstraintValue * SingleForeignConstraintValueSemanticAction(Expression * expression, char * name, OnAction * onAction);
ConstraintValue * DoubleForeignConstraintValueSemanticAction(Expression * expression1, char * name, Expression * expression2, OnAction * onAction);

Constraint * NamedConstraintValueSemanticAction(char * id, ConstraintValue * constraintValue);
Constraint * UnnamedConstraintValueSemanticAction(ConstraintType type, ConstraintValue * constraintValue);

Constraints * SimpleConstraintsSemanticAction(LocalConstraint * localConstraint);
Constraints * DoubleConstraintsSemanticAction(LocalConstraint * localConstraint1, LocalConstraint * localConstraint2);

LocalConstraint * DoubleNameOnActionLocalConstraintSemanticAction(char * id1, char * id2, OnAction * OnAction);
LocalConstraint * CheckLocalConstraintSemanticAction(CheckConstraint * checkConstraint)

Function * FunctionSemanticAction(FunctionType functionType);

NullCondition * NullConditionSemanticAction(NullConditionType type);

Type * SimpleTypeSemanticAction(DataTypeType type);
Type * ComplexTypeSemanticAction(int param, DataTypeType type);
Type * DoubleComplexTypeSemanticAction(int param1, int param2, DataTypeType type);

Attribute * AttributeTypeSemanticAction(char * id, Type * type);
Attribute * AttributeTypePropertiesSemanticAction(char * id, Type * type, Properties * properties);

Attributes * MultipleAttributesSemanticAction(Attribute * attribute1, Attribute * attribute2);
Attributes * AttributesAttributeSemanticAction(Attribute * attribute);

Content * AttributesContentSemanticAction(Attributes * attributes);
Content * ConstraintsContentSemanticAction(Constraints * constraints);
Content * AttributesAndConstraintsContentSemanticAction(Attributes * attributes, Constraints * constraints);
Content * EmptyContentSemanticAction();

Tables * ContentTablesSemanticAction(char * id, Content * content);
Tables * TableGenerateSemanticAction(Tables * tables1, Tables *tables2);

Program * TablesProgramSemanticAction(CompilerState * compilerState, Tables * tables);

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
