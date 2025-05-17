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

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"[Cc][Rr][Ee][Aa][Tt][Ee]" 					{ return TextLexemeAction(createLexicalAnalyzerContext(), CREATE); }
"[Tt][Aa][Bb][Ll][Ee]" 						{ return TextLexemeAction(createLexicalAnalyzerContext(), TABLE); }

"[Cc][Hh][EEe][Cc][Kk]" 					{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), CHECK); }
"[Pp][Rr][Ii][Mm][Aa][Rr][Yy]" 				{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), PRIMARY); }
"[Ff][Oo][Rr][Ee][Ii][Nn][Gg]" 				{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), FOREIGN); }
"[Kk][Ee][Yy]" 								{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), KEY); }
"[Uu][Nn][Ii][Qq][Uu][Ee]" 					{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), UNIQUE); }
"[Cc][Oo][Nn][Ss][Tt][Rr][Aa][Ii][Nn][Tt]" 	{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), CONSTRAINT); }

"("											{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"											{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
","											{ return ComaLexemeAction(createLexicalAnalyzerContext(), COMA); }
";"					 						{ return SemicolonLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }




<<EOF>>								{
										printf("%s", "[FlexPatterns] EOF, poping buffer.\n");
										yypop_buffer_state();
										if (!YY_CURRENT_BUFFER) {
											// Signals Bison to stop the parsing if there is no more input buffers in the stack.
											yyterminate();
										}
									}

[[:alnum:]"_"]+						{ return IdLexemeAction(createLexicalAnalyzerContext()); }



[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"

/**
*
* "[Nn][Oo][Tt]" 								{ return BooleanLexemeAction(createLexicalAnalyzerContext(), NOT); }
*
*"[Nn][Uu][Ll][Ll]"							{ return ConstraintLexemeAction(createLexicalAnalyzerContext(), NUL); }
*
*[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }
*
* "-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
* "*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
* "/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
* "+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }
*
* <IMPORT_EXPRESSION>[^{}]+			{
* 										LexicalAnalyzerContext * lexicalAnalyzerContext = createLexicalAnalyzerContext();
* 										printf("[FlexPatterns] Creating next buffer to consume from file '%s'.\n", lexicalAnalyzerContext->lexeme);
* 										yyin = fopen(lexicalAnalyzerContext->lexeme, "r");
* 										importedExpressionBufferState = yy_create_buffer(yyin, YY_BUF_SIZE);
*										destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
*									}
*
* <IMPORT_EXPRESSION>"}"				{
*										printf("%s", "[FlexPatterns] Loading next buffer from imported expression.\n");
*										yypush_buffer_state(importedExpressionBufferState);
*										BEGIN(INITIAL);
*									}
*
* <IMPORT_EXPRESSION>.				{
*										printf("%s", "[FlexPatterns] Syntax error inside import expression.\n");
*										return UnknownLexemeAction(createLexicalAnalyzerContext());
*									}
*"{"									{
*										printf("%s", "[FlexPatterns] Context-switch to IMPORT_EXPRESSION.\n");
*										BEGIN(IMPORT_EXPRESSION);
*									}
**/
