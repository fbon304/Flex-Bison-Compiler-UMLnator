#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

/*void releaseConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ADDITION:
			case DIVISION:
			case MULTIPLICATION:
			case SUBTRACTION:
				releaseExpression(expression->leftExpression);
				releaseExpression(expression->rightExpression);
				break;
			case FACTOR:
				releaseFactor(expression->factor);
				break;
		}
		free(expression);
	}
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				releaseConstant(factor->constant);
				break;
			case EXPRESSION:
				releaseExpression(factor->expression);
				break;
		}
		free(factor);
	}
} 
*/
void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		//releaseTables(program->tables);
		free(program);
	}
}
/*
void releaseTables(Tables *tables) {
    if (tables == NULL) return;

    // caso recursivo: una lista de dos tablas
    if (tables->tables1 != NULL && tables->tables2 != NULL) {
        releaseTables(tables->tables1);
        releaseTables(tables->tables2);

    // caso hoja: free del id y del contenido asociado
    } else {
        // aquí liberamos el malloc/strcpy de IdLexemeAction()
        free(tables->id);

        // luego liberar el contenido (attributes/constraints), si tienes un destructor
        if (tables->content) {
            releaseAttributes(tables->content->attributes);
            releaseConstraints(tables->content->constraints);
            free(tables->content);
        }
    }

    // por último, liberamos el propio struct
    free(tables);
}

*/
