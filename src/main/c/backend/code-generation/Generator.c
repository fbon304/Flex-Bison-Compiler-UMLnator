// #include "Generator.h"

// /* MODULE INTERNAL STATE */

// const char _indentationCharacter = ' ';
// const char _indentationSize = 4;
// static Logger * _logger = NULL;

// void initializeGeneratorModule() {
// 	_logger = createLogger("Generator");
// }

// void shutdownGeneratorModule() {
// 	if (_logger != NULL) {
// 		destroyLogger(_logger);
// 	}
// }

// /** PRIVATE FUNCTIONS */

// static const char _expressionTypeToCharacter(const ExpressionType type);
// static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
// static void _generateEpilogue(const int value);
// static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
// static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
// static void _generateProgram(Program * program);
// static void _generatePrologue(void);
// static char * _indentation(const unsigned int indentationLevel);
// static void _output(const unsigned int indentationLevel, const char * const format, ...);

// /**
//  * Converts and expression type to the proper character of the operation
//  * involved, or returns '\0' if that's not possible.
//  */
// static const char _expressionTypeToCharacter(const ExpressionType type) {
// 	switch (type) {
// 		case ADDITION: return '+';
// 		case DIVISION: return '/';
// 		case MULTIPLICATION: return '*';
// 		case SUBTRACTION: return '-';
// 		default:
// 			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
// 			return '\0';
// 	}
// }

// /**
//  * Generates the output of a constant.
//  */
// static void _generateConstant(const unsigned int indentationLevel, Constant * constant) {
// 	_output(indentationLevel, "%s", "[ $C$, circle, draw, black!20\n");
// 	_output(1 + indentationLevel, "%s%d%s", "[ $", constant->value, "$, circle, draw ]\n");
// 	_output(indentationLevel, "%s", "]\n");
// }

// /**
//  * Creates the epilogue of the generated output, that is, the final lines that
//  * completes a valid Latex document.
//  */
// static void _generateEpilogue() {
// 	_output(0, "%s%d%s",
// 		"@enduml\n"
// 	);
// }

// /**
//  * Generates the output of an expression.
//  */
// static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
// 	_output(indentationLevel, "%s", "[ $E$, circle, draw, black!20\n");
// 	switch (expression->type) {
// 		case ADDITION:
// 		case DIVISION:
// 		case MULTIPLICATION:
// 		case SUBTRACTION:
// 			_generateExpression(1 + indentationLevel, expression->leftExpression);
// 			_output(1 + indentationLevel, "%s%c%s", "[ $", _expressionTypeToCharacter(expression->type), "$, circle, draw, purple ]\n");
// 			_generateExpression(1 + indentationLevel, expression->rightExpression);
// 			break;
// 		case FACTOR:
// 			_generateFactor(1 + indentationLevel, expression->factor);
// 			break;
// 		default:
// 			logError(_logger, "The specified expression type is unknown: %d", expression->type);
// 			break;
// 	}
// 	_output(indentationLevel, "%s", "]\n");
// }

// /**
//  * Generates the output of a factor.
//  */
// static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
// 	_output(indentationLevel, "%s", "[ $F$, circle, draw, black!20\n");
// 	switch (factor->type) {
// 		case CONSTANT:
// 			_generateConstant(1 + indentationLevel, factor->constant);
// 			break;
// 		case EXPRESSION:
// 			_output(1 + indentationLevel, "%s", "[ $($, circle, draw, purple ]\n");
// 			_generateExpression(1 + indentationLevel, factor->expression);
// 			_output(1 + indentationLevel, "%s", "[ $)$, circle, draw, purple ]\n");
// 			break;
// 		default:
// 			logError(_logger, "The specified factor type is unknown: %d", factor->type);
// 			break;
// 	}
// 	_output(indentationLevel, "%s", "]\n");
// }

// /**
//  * Generates the output of the program.
//  */
// static void _generateProgram(Program * program) {
// 	//_generateExpression(3, program->expression);
//     _generateTablesList(2, program->tablesList);
// }

// static void _generateTablesList(const unsigned int indentationLevel, TablesList * tablesList) {
// 	if (tablesList != NULL) {
// 		_generateTable(indentationLevel, tablesList->tables);
// 		_generateTablesList(indentationLevel, tablesList->tablesList);
// 	}
// }

// static void _generateTable(const unsigned int indentationLevel, Tables * table) {
// 	if (table != NULL) {
// 		_output(indentationLevel, "object %s {\n", table->id);
// 		_generateContent(indentationLevel + 1, table->content);
// 		_output(identationLevel + 1, "}");
// 	}
// }

// static void _generateContent(const unsigned int indentationLevel, Content * content) {
// 	if (content != NULL) {
// 		if (content->type == ELEMENT) {
// 			_generateContentElement(indentationLevel, content->content_element);
// 		} else if (content->type == CONTENT_LIST) {
// 			_generateContent(indentationLevel, content->content);
// 			_generateContentElement(indentationLevel, content->content_element);
// 		}
// 	}
// }

// static void _generateContentElement(const unsigned int indentationLevel, ContentElement * contentElement) {
// 	if (contentElement != NULL) {
// 		if (contentElement->contentElementType == ATTRIBUTE_TYPE) {
// 			_generateAttribute(indentationLevel, contentElement->attribute);
// 		} else if (contentElement->contentElementType == CONSTRAINT_TYPE) {
// 			// TODO capaz se puede hacer que retorne una funcion que genere al final de todo todos los constraints
// 			_generateConstraint(indentationLevel, contentElement->constraint);
// 		}
// 	}
// }

// static void _generateAttribute(const unsigned int indentationLevel, Attribute * attribute) {
// 	if (attribute != NULL) {
//         if (attribute->type == COLUMN) {
//             _output(indentationLevel, "%s: <size:12>%s;", attribute->id, attribute->datatype);
//         } else if (attribute->type == COLUMN_WITH_PROPERTIES) {
//             _output(indentationLevel, "%s: <size:12>%s ", attribute->p_id, attribute->p_type);
//             _generateProperties(indentationLevel, attribute->properties);
//         }
// 	}
// }

// static void _generateProperties(const unsigned int indentationLevel, Properties * properties) {
//     if (properties != NULL) {
//         switch (properties->type)
//         {
//         case DEFAULT_VALUE:
            
//             break;
//         case CONSTRAINT_CONDITION:
            
//             break;
//         case NULL_CONDITION:
            
//             break;
//         case NULL_CONDITION_DEFAULT_VALUE:
            
//             break;
//         case DEFAULT_VALUE_CONSTRAINT:
            
//             break;
//         case NULL_CONDITION_CONSTRAINT:
            
//             break;
//         case COMPLETE:
            
//             break;
//         }
//     }
// }

// static void _generateConstraint(const unsigned int indentationLevel, Constraint * constraint) {
// 	// TODO
// }



// /**
//  * Creates the prologue of the generated output, a Latex document that renders
//  * a tree thanks to the Forest package.
//  *
//  * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
//  */
// static void _generatePrologue(void) {
// 	_output(0, "%s",
// 		"@startuml\n"
// 	);
// }

// /**
//  * Generates an indentation string for the specified level.
//  */
// static char * _indentation(const unsigned int level) {
// 	return indentation(_indentationCharacter, level, _indentationSize);
// }

// /**
//  * Outputs a formatted string to standard output. The "fflush" instruction
//  * allows to see the output even close to a failure, because it drops the
//  * buffering.
//  */
// static void _output(const unsigned int indentationLevel, const char * const format, ...) {
// 	va_list arguments;
// 	va_start(arguments, format);
// 	char * indentation = _indentation(indentationLevel);
// 	char * effectiveFormat = concatenate(2, indentation, format);
// 	vfprintf(stdout, effectiveFormat, arguments);
// 	fflush(stdout);
// 	free(effectiveFormat);
// 	free(indentation);
// 	va_end(arguments);
// }

// /** PUBLIC FUNCTIONS */

// void generate(CompilerState * compilerState) {
// 	logDebugging(_logger, "Generating final output...");
// 	//_generatePrologue();
// 	//_generateProgram(compilerState->abstractSyntaxtTree);
// 	//_generateEpilogue();
// 	logDebugging(_logger, "Generation is done.");
// }