%{

#include "FlexActions.h"

static YY_BUFFER_STATE importedExpressionBufferState = NULL;

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack
%option case-insensitive

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 * %x IMPORT_EXPRESSION
 */
%x MULTILINE_COMMENT
%x DEFAULT_VALUE_STRING

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"											{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"							{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+					{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.							{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }


"'"												{ BEGIN(DEFAULT_VALUE_STRING); BeginDefaultValueStringLexemeAction(createLexicalAnalyzerContext()); }
<DEFAULT_VALUE_STRING>"'"						{ EndDefaultValueStringLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<DEFAULT_VALUE_STRING>[^']*						{ DefaultValueStringLexemeAction(createLexicalAnalyzerContext()); }

"CREATE" 										{ return TextLexemeAction(createLexicalAnalyzerContext(), CREATE); }
"TABLE" 										{ return TextLexemeAction(createLexicalAnalyzerContext(), TABLE); }

"NULL"											{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), NUL); }
"CHECK" 										{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), CHECK); }
"PRIMARY" 										{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), PRIMARY); }
"FOREIGN" 										{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), FOREIGN); }
"KEY" 											{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), KEY); }
"UNIQUE"				 						{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), UNIQUE); }
"CONSTRAINT"									{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), CONSTRAINT); }
"DEFAULT"										{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), DEFAULT); }
"REFERENCES"    								{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), REFERENCES); }

"INTEGER"|"INT"	  								{ return TypeLexemeAction(createLexicalAnalyzerContext(), INTEGER); }
"SMALLINT"										{ return TypeLexemeAction(createLexicalAnalyzerContext(), SMALLINT); }
"BIGINT"										{ return TypeLexemeAction(createLexicalAnalyzerContext(), BIGINT); }
"REAL"											{ return TypeLexemeAction(createLexicalAnalyzerContext(), REAL); }
"DOUBLE"										{ return TypeLexemeAction(createLexicalAnalyzerContext(), DOUBLE); }
"DATE"											{ return TypeLexemeAction(createLexicalAnalyzerContext(), DATE); }
"TIMESTAMP"										{ return TypeLexemeAction(createLexicalAnalyzerContext(), TIMESTAMP); }		
"INTERVAL"										{ return TypeLexemeAction(createLexicalAnalyzerContext(), INTERVAL); }
"TEXT"											{ return TypeLexemeAction(createLexicalAnalyzerContext(), TEXT); }
"SERIAL"										{ return TypeLexemeAction(createLexicalAnalyzerContext(), SERIAL); }
"CHAR"|"CHARACTER"								{ return TypeLexemeAction(createLexicalAnalyzerContext(), CHAR); }		
"VARCHAR"|"CHAR VARYING"|"CHARACTER VARYING"	{ return TypeLexemeAction(createLexicalAnalyzerContext(), VARCHAR); }
"NUMBER"|"DECIMAL"|"DEC"						{ return TypeLexemeAction(createLexicalAnalyzerContext(), NUMBER); }
"FLOAT"											{ return TypeLexemeAction(createLexicalAnalyzerContext(), FLOAT); }
"TIME"											{ return TypeLexemeAction(createLexicalAnalyzerContext(), TIME); }

"CURRENT_TIMESTAMP"|"NOW()"						{ return FunctionLexemeAction(createLexicalAnalyzerContext(), CURRENT_TIMESTAMP); }
"AUTO_INCREMENT"								{ return FunctionLexemeAction(createLexicalAnalyzerContext(), AUTO_INCREMENT); }
"CURRENT_DATE"									{ return FunctionLexemeAction(createLexicalAnalyzerContext(), CURRENT_DATE); }	
"CURRENT_TIME"									{ return FunctionLexemeAction(createLexicalAnalyzerContext(), CURRENT_TIME); }
"LOCALTIME"										{ return FunctionLexemeAction(createLexicalAnalyzerContext(), LOCALTIME); }
"LOCALTIMESTAMP"								{ return FunctionLexemeAction(createLexicalAnalyzerContext(), LOCALTIMESTAMP); }
"GEN_RANDOM_UUID"								{ return FunctionLexemeAction(createLexicalAnalyzerContext(), GEN_RANDOM_UUID); }			
"UUID_GENERATE_V4"								{ return FunctionLexemeAction(createLexicalAnalyzerContext(), UUID_GENERATE_V4); }

"NOT" 											{ return BooleanLexemeAction(createLexicalAnalyzerContext(), NOT); }
"OR"											{ return BooleanLexemeAction(createLexicalAnalyzerContext(), OR); }
"AND"											{ return BooleanLexemeAction(createLexicalAnalyzerContext(), AND); }
"="												{ return BooleanLexemeAction(createLexicalAnalyzerContext(), EQUALS); }
"!="|"<>"										{ return BooleanLexemeAction(createLexicalAnalyzerContext(), NOT_EQUALS); }	
">"												{ return BooleanLexemeAction(createLexicalAnalyzerContext(), GREATER_THAN); }
">="											{ return BooleanLexemeAction(createLexicalAnalyzerContext(), GREATER_THAN_EQUALS); }
"<"												{ return BooleanLexemeAction(createLexicalAnalyzerContext(), LESS_THAN); }
"<="											{ return BooleanLexemeAction(createLexicalAnalyzerContext(), LESS_THAN_EQUALS); }
"IS"											{ return BooleanLexemeAction(createLexicalAnalyzerContext(), IS); }
"IS NOT"										{ return BooleanLexemeAction(createLexicalAnalyzerContext(), ISNOT); }
"ISNULL"										{ return BooleanLexemeAction(createLexicalAnalyzerContext(), ISNULL); }
"NOTNULL"										{ return BooleanLexemeAction(createLexicalAnalyzerContext(), NOTNULL); }
"DISTINCT FROM"									{ return BooleanLexemeAction(createLexicalAnalyzerContext(), DISTINCT_FROM); }
"TRUE"											{ return BooleanLexemeAction(createLexicalAnalyzerContext(), TRUE); }
"FALSE"											{ return BooleanLexemeAction(createLexicalAnalyzerContext(), FALSE); }

"ON"											{ return ActionLexemeAction(createLexicalAnalyzerContext(), ON); }
"DELETE"										{ return ActionLexemeAction(createLexicalAnalyzerContext(), DELETE); }
"SET"											{ return ActionLexemeAction(createLexicalAnalyzerContext(), SET); }
"CASCADE"										{ return ActionLexemeAction(createLexicalAnalyzerContext(), CASCADE); }
"UPDATE"										{ return ActionLexemeAction(createLexicalAnalyzerContext(), UPDATE); }
"NO ACTION"										{ return ActionLexemeAction(createLexicalAnalyzerContext(), NO_ACTION); }
"RESTRICT"										{ return ActionLexemeAction(createLexicalAnalyzerContext(), RESTRICT); }

"("												{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"												{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
","												{ return ComaLexemeAction(createLexicalAnalyzerContext(), COMA); }
";"					 							{ return SemicolonLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }




<<EOF>>										{
												printf("%s", "[FlexPatterns] EOF, poping buffer.\n");
												yypop_buffer_state();
												if (!YY_CURRENT_BUFFER) {
													// Signals Bison to stop the parsing if there is no more input buffers in the stack.
													yyterminate();
												}
											}

[[:digit:]]+									{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }
									
[[:digit:]]+\,[[:digit:]]+ 						{ return DoubleLexemeAction(createLexicalAnalyzerContext()); }

[[:alnum:]"_"]+									{ return IdLexemeAction(createLexicalAnalyzerContext()); }


[[:space:]]+									{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

.												{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"