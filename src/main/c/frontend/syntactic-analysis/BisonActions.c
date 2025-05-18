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

Type * SimpleTypeSemanticAction(const char * name) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type * type = calloc(1, sizeof(Type));
	type->name = name;
	return type;
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