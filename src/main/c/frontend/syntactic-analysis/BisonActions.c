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

Properties * SimpleDefaultPropertySemanticAction(DefaultValue * defaultValue, PropertiesType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Properties * property = calloc(1, sizeof(Properties));
	property->defaultValue = defaultValue;
	property->type = type;
	return property;
}

Properties * SimpleConstraintPropertySemanticAction(Constraint * constraint, PropertiesType type) {
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

Properties * DoubleDefaultConstraintPropertySemanticAction(DefaultValue * defaultValue, Constraint * constraint, PropertiesType type) {
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

Properties * DoubleConstraintNullPropertySemanticAction(Constraint * constraint, NullCondition * nullCondition, PropertiesType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Properties * property = calloc(1, sizeof(Properties));
	property->constraintCN = defaultValue;
	property->nullConditionCN = nullCondition;
	property->type = type;
	return property;
}

Properties * TriplePropertySemanticAction(DefaultValue * defaultValue, Constraint * constraint, NullCondition * nullCondition) {
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
	Type * type = calloc(1, sizeof(Type));
	type->type = type;
	return type;
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

Attribute * AttributeTypeSemanticAction(const char * id, Type * type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Attribute * attribute = calloc(1, sizeof(Attribute));
	attribute->id = id;
	attribute->datatype = type;
	attribute->type = COLUMN;
	return attribute;
}

Attribute * AttributeTypePropertiesSemanticAction(const char * id, Type * type, Properties * properties) {
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