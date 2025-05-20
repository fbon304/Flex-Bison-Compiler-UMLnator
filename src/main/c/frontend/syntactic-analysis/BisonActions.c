#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */


IsCondition * IsConditionSemanticAction(IsConditionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IsCondition * isCondition = calloc(1, sizeof(IsCondition));
	isCondition->type = type;
	return isCondition;
}

BooleanValue * BooleanValueSemanticAction(BooleanType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BooleanValue * booleanValue = calloc(1, sizeof(BooleanValue));
	booleanValue->type = type;
	return booleanValue;
}

Factor * StringFactorSemanticAction(char * value, FactorType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->string = value;
	factor->type = type;
	return factor;
}

Factor * DoubleFactorSemanticAction(double value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->double_factor = value;
	factor->type = DOUBLE_FACTOR_TYPE;
	return factor;
}

Factor * IntegerFactorSemanticAction(int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->integer_factor = value;
	factor->type = ID_FACTOR_TYPE;
	return factor;
}

BooleanExpression * FactorsBooleanExpressionSemanticAction(Factor * leftFactor, IsCondition * isCondition, Factor * rightFactor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
	booleanExpression->factor_left = leftFactor;
	booleanExpression->isCondition_with_two_factors = isCondition;
	booleanExpression->factor_right = rightFactor;
	booleanExpression->type = DISTINCT_FROM_BOOLEANTYPE;
	return booleanExpression;
}

BooleanExpression * BooleanValueBooleanExpressionSemanticAction(BooleanValue * booleanValue) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
	booleanExpression->unique_boolean_value = booleanValue;
	booleanExpression->type = DISTINCT_FROM_BOOLEANTYPE;
	return booleanExpression;
}

BooleanExpression * SimpleBooleanExpressionSemanticAction(BooleanExpression * booleanExpression, BooleanExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
	booleanExpression->unique_boolean_expression = booleanExpression;
	booleanExpression->type = type;
	return booleanExpression;
}

BooleanExpression * IsConditionBooleanExpressionSemanticAction(BooleanExpression * booleanExpression, IsCondition * isCondition) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BooleanExpression * newBooleanExpression = calloc(1, sizeof(BooleanExpression));
	booleanExpression->boolean_expression_with_is_condition = booleanExpression;
	booleanExpression->is_condition_with_boolean_expression = isCondition;
	booleanExpression->type = NUL_BOOLEANTYPE;
	return booleanExpression;
}

BooleanExpression * TriplePointerBooleanExpressionSemanticAction(BooleanExpression * booleanExpression, IsCondition * isCondition, BooleanValue * booleanValue) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BooleanExpression * newBooleanExpression = calloc(1, sizeof(BooleanExpression));
	newBooleanExpression->boolean_expression_three_pointers = booleanExpression;
	newBooleanExpression->is_condition_three_pointers = isCondition;
	newBooleanExpression->boolean_value = booleanValue;
	newBooleanExpression->type = THREE_POINTERS_BOOLEANTYPE;
	return booleanExpression;
}

BooleanExpression * DoubleBooleanExpressionSemanticAction(BooleanExpression * leftBooleanExpression, BooleanExpression * rightBooleanExpression, BooleanExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BooleanExpression * booleanExpression = calloc(1, sizeof(BooleanExpression));
	booleanExpression->boolean_expression_left = leftBooleanExpression;
	booleanExpression->boolean_expression_right = rightBooleanExpression;
	booleanExpression->type = type;
	return booleanExpression;
}

CheckConstraint * BooleanExpressionCheckConstraint(BooleanExpression * booleanExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	CheckConstraint * checkConstraint = calloc(1, sizeof(CheckConstraint));
	checkConstraint->booleanExpression = booleanExpression;
	return checkConstraint;
}

Expression * ComplexExpressionSemanticAction(char * id, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * newExpression = calloc(1, sizeof(Expression));
	newExpression->id = id;
	newExpression->expression = expression;
	newExpression->type = COMPLEX_EXPRESSION;
	return expression;
}

Expression * IdExpressionSemanticAction(char * id) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->id = id;
	expression->type = SIMPLE_EXPRESSION;
	return expression;
}

OnAction * OnActionSemanticAction(Action * action, ActionType actionType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	OnAction * onAction = calloc(1, sizeof(OnAction));
	onAction->type = actionType;
	onAction->action = action;
	return onAction;
}

OnAction * DoubleActionSemanticAction(Action * deleteAction, Action * updateAction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	OnAction * onAction = calloc(1, sizeof(OnAction));
	onAction->type = ON_DELETE_ON_UPDATE_ON_ACTION;
	onAction->deleteAction = deleteAction;
	onAction->updateAction = updateAction;
	return onAction;
}

OnAction * EmptyActionSemanticAction() {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	OnAction * onAction = calloc(1, sizeof(OnAction));
	onAction->type = LAMBDA_ON_ACTION;
	onAction->action = NULL;
	return onAction;
}

Action * ActionSemanticAction(ActionType actiontype) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Action * action = calloc(1, sizeof(Action));
	action->type = actiontype;
	return action;
}

ConstraintValue * CheckConstraintValueSemanticAction(CheckConstraint * checkConstraint) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstraintValue * constraintValue = calloc(1, sizeof(ConstraintValue));
	constraintValue->type = CHECK_CONSTRAINT_TYPE;
	constraintValue->checkConstraint = checkConstraint;
	return constraintValue;
}

ConstraintValue * SingleExpressionConstraintValueSemanticAction(Expression * expression, ConstraintValueType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstraintValue * constraintValue = calloc(1, sizeof(ConstraintValue));
	constraintValue->type = type;
	constraintValue->expression = expression;
	return constraintValue;
}

ConstraintValue * SingleForeignConstraintValueSemanticAction(Expression * expression, char * name, OnAction * onAction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstraintValue * constraintValue = calloc(1, sizeof(ConstraintValue));
	constraintValue->type = FOREIGN_KEY_CONSTRAINT_TYPE;
	constraintValue->singleExpression = expression;
	constraintValue->onActionSingle = onAction;
	constraintValue->id = name;
	return constraintValue;
}

ConstraintValue * DoubleForeignConstraintValueSemanticAction(Expression * expression1, char * name, Expression * expression2, OnAction * onAction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstraintValue * constraintValue = calloc(1, sizeof(ConstraintValue));
	constraintValue->type = FOREIGN_KEY_DOUBLE_EXPRESSION_CONSTRAINT_TYPE;
	constraintValue->mainExpression = expression1;
	constraintValue->secondExpression = expression2;
	constraintValue->onActionDouble = onAction;
	constraintValue->idComplex = name;
	return constraintValue;
}

Constraint * NamedConstraintValueSemanticAction(char * id, ConstraintValue * constraintValue) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constraint * constraint = calloc(1, sizeof(Constraint));
	constraint->type = NAMED_CONSTRAINT;
	constraint->id = id;
	constraint->constraintValue = constraintValue;
	return constraint;
}

Constraint * UnnamedConstraintValueSemanticAction(ConstraintType type, ConstraintValue * constraintValue) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constraint * constraint = calloc(1, sizeof(Constraint));
	constraint->type = UNNAMED_CONSTRAINT;
	constraint->constraintValue = constraintValue;
	return constraint;
}

Constraints * SimpleConstraintsSemanticAction(LocalConstraint * localConstraint) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constraints * constraints = calloc(1, sizeof(Constraints));
	constraints->type = SINGLE_CONSTRAINT;
	constraints->constraint = localConstraint;
	return constraints;
}

Constraints * DoubleConstraintsSemanticAction(LocalConstraint * localConstraint1, LocalConstraint * localConstraint2) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constraints * constraints = calloc(1, sizeof(Constraints));
	constraints->type = DOUBLE_CONSTRAINT;
	constraints->constraint1 = localConstraint1;
	constraints->constraint2 = localConstraint2;
	return constraints;
}

LocalConstraint * DoubleNameOnActionLocalConstraintSemanticAction(char * id1, char * id2, OnAction * onAction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LocalConstraint * localConstraint = calloc(1, sizeof(LocalConstraint));
	localConstraint->type = FOREIGN_KEY_LCT;
	localConstraint->id1 = id1;
	localConstraint->id2 = id2;
	localConstraint->onAction = onAction;
	return localConstraint;
}

LocalConstraint * CheckLocalConstraintSemanticAction(CheckConstraint * checkConstraint) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LocalConstraint * localConstraint = calloc(1, sizeof(LocalConstraint));
	localConstraint->type = CHECK_LCT;
	localConstraint->checkConstraint = checkConstraint;
	return localConstraint;
}

Function * FunctionSemanticAction(FunctionType functionType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Function * function = calloc(1, sizeof(Function));
	function->functionType = functionType;
	return function;
}

DefaultValue * DefaultValueIntegerTerminalSemanticAction(int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DefaultValue * defaultValue = calloc(1, sizeof(DefaultValue));
	defaultValue->type = INTEGER_DEFAULT;
	defaultValue->integer_value = value;
	return defaultValue;
}

DefaultValue * DefaultValueDoubleTerminalSemanticAction(double value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DefaultValue * defaultValue = calloc(1, sizeof(DefaultValue));
	defaultValue->type = DOUBLE_DEFAULT;
	defaultValue->double_value = value;
	return defaultValue;
}

DefaultValue * DefaultValueStringTerminalSemanticAction(char * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DefaultValue * defaultValue = calloc(1, sizeof(DefaultValue));
	defaultValue->type = STRING_DEFAULT;
	defaultValue->string_value = value;
	return defaultValue;
}


DefaultValue * DefaultValueNonTerminalSemanticAction(Function * function) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DefaultValue * defaultValue = calloc(1, sizeof(DefaultValue));
	defaultValue->type = FUNCTION;
	defaultValue->function = function;
	return defaultValue;
}

Properties * SimpleDefaultPropertySemanticAction(DefaultValue * defaultValue, PropertiesType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Properties * property = calloc(1, sizeof(Properties));
	property->defaultValue = defaultValue;
	property->type = type;
	return property;
}

Properties * SimpleConstraintPropertySemanticAction(LocalConstraint * constraint, PropertiesType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Properties * property = calloc(1, sizeof(Properties));
	property->constraint = constraint;
	property->type = type;
	return property;
}

Properties * SimpleNullPropertySemanticAction(NullCondition * nullCondition, PropertiesType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Properties * property = calloc(1, sizeof(Properties));
	property->nullCondition = nullCondition;
	property->type = type;
	return property;
}

Properties * DoubleDefaultConstraintPropertySemanticAction(DefaultValue * defaultValue, LocalConstraint * constraint, PropertiesType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Properties * property = calloc(1, sizeof(Properties));
	property->defaultValueDC = defaultValue;
	property->constraintDC = constraint;
	property->type = type;
	return property;
}

Properties * DoubleDefaultNullPropertySemanticAction(DefaultValue * defaultValue, NullCondition * nullCondition, PropertiesType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Properties * property = calloc(1, sizeof(Properties));
	property->defaultValueDN = defaultValue;
	property->nullConditionDN = nullCondition;
	property->type = type;
	return property;
}

Properties * DoubleConstraintNullPropertySemanticAction(LocalConstraint * constraint, NullCondition * nullCondition, PropertiesType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Properties * property = calloc(1, sizeof(Properties));
	property->constraintCN = constraint;
	property->nullConditionCN = nullCondition;
	property->type = type;
	return property;
}

Properties * TriplePropertySemanticAction(DefaultValue * defaultValue, LocalConstraint * constraint, NullCondition * nullCondition) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Properties * property = calloc(1, sizeof(Properties));
	property->defaultValueCDN = defaultValue;
	property->constraintCDN = constraint;
	property->nullConditionCDN = nullCondition;
	property->type = COMPLETE;
	return property;
}

Type * SimpleTypeSemanticAction(DataTypeType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type * typeNode = calloc(1, sizeof(Type));
	typeNode->type = type;
	return typeNode;
}

Type * ComplexTypeSemanticAction(int param, DataTypeType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type * complexType = calloc(1, sizeof(Type));
	complexType->type = type;
	complexType->param1 = param;
	return complexType;
}

Type * DoubleComplexTypeSemanticAction(int param1, int param2, DataTypeType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type * complexType = calloc(1, sizeof(Type));
	complexType->type = type;
	complexType->param1 = param1;
	complexType->param2 = param2;
	return complexType;
}

NullCondition * NullConditionSemanticAction(NullConditionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NullCondition * nullCondition = calloc(1, sizeof(NullCondition));
	nullCondition->type = type;
	return nullCondition;
}

Attribute * AttributeTypeSemanticAction(char * id, Type * type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Attribute * attribute = calloc(1, sizeof(Attribute));
	attribute->id = id;
	attribute->datatype = type;
	attribute->type = COLUMN;
	return attribute;
}

Attribute * AttributeTypePropertiesSemanticAction(char * id, Type * type, Properties * properties) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Attribute * attribute = calloc(1, sizeof(Attribute));
	attribute->p_id = id;
	attribute->p_type = type;
	attribute->properties = properties;
	attribute->type = COLUMN_WITH_PROPERTIES;
	return attribute;
}

Attributes * MultipleAttributesSemanticAction(Attribute * attribute1, Attribute * attribute2) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Attributes * attributes = calloc(1, sizeof(Attributes));
	attributes->attribute1 = attribute1;
	attributes->attribute2 = attribute2;
	attributes->type = MULTIPLE_ATTRIBUTE;
	return attributes;
}

Attributes * AttributesAttributeSemanticAction(Attribute * attribute) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Attributes * attributes = calloc(1, sizeof(Attributes));
	attributes->head = attribute;
	attributes->type = ATTRIBUTE;
	return attributes;
}

Content * AttributesContentSemanticAction(Attributes * attributes) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Content * content = calloc(1, sizeof(Content));
	content->onlyAttributes = attributes;
	content->type = ATTRIBUTES;
	return content;
}

Content * ConstraintsContentSemanticAction(Constraints * constraints) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Content * content = calloc(1, sizeof(Content));
	content->onlyConstraints = constraints;
	content->type = CONSTRAINTS;
	return content;
}

Content * AttributesAndConstraintsContentSemanticAction(Attributes * attributes, Constraints * constraints) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Content * content = calloc(1, sizeof(Content));
	content->attributes = attributes;
	content->constraints = constraints;
	content->type = ATTRIBUTES_CONSTRAINTS;
	return content;
}

Content * EmptyContentSemanticAction() {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return NULL;
}

Tables * ContentTablesSemanticAction(char * id, Content * content) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Tables * newTable = calloc(1, sizeof(Tables));
	newTable->content = content;
	newTable->id = id;
	newTable->type = CONTENT;
	return newTable;
}

Tables * TableGenerateSemanticAction(Tables * tables1, Tables *tables2) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Tables * tables = calloc(1, sizeof(Tables));
	tables->tables1 = tables1;
	tables->tables2 = tables2;
	tables->type = MULTIPLE_TABLES;
	return tables;
};

Program * TablesProgramSemanticAction(CompilerState * compilerState, Tables * tables) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->tables = tables;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}


/*Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Factor * ConstantFactorSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->constant = constant;
	factor->type = CONSTANT;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
} 

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}
*/