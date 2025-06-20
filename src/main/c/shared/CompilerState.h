#ifndef COMPILER_STATE_HEADER
#define COMPILER_STATE_HEADER

#include "Type.h"
#include "stdint.h"
#include "DataStructures/Stack.h"
#include "DataStructures/HashMap.h"

/**
 * The general status of a compilation.
*/
typedef enum {
	SUCCEED = 0,
	FAILED = 1
} CompilationStatus;

typedef union DataValue {
	int64_t intValue; // For integer, smallint, bigint, serial, timestamp.
	double doubleValue; // For real, double, float, number.
	char *stringValue; // For text, varchar, char, date, interval, time.
	boolean booleanValue; // For boolean
} DataValue;

typedef struct Symbol {
    DataType type;
	DataValue value;
} Symbol;

/**
 * The global state of the compiler. Should transport every data structure
 * needed across the different phases of a compilation.
 */
typedef struct {
	// The root node of the AST.
	void * abstractSyntaxtTree;

	// A flag that indicates the current state of the compilation so far.
	boolean succeed;

	HashMap * symbolTable; 			// Global symbol table, with the key as the SQL table name and with value as another HashMap
	Stack * scopeStack; 			// A stack of the SQL table names for scopes

	// TODO: Add a stack to handle nested scopes.
	// TODO: Add a symbol table.
	// TODO: Add configuration.
	// TODO: ...

	// The computed value of the entire program (only for the calculator).
	 int value;
} CompilerState;

#endif
