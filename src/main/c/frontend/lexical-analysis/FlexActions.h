#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

void BeginDefaultValueStringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndDefaultValueStringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DefaultValueStringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DoubleLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IdLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token TypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token ActionLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token FunctionLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token DateTimeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

Token TextLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token ComaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token SemicolonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token ConstraintLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token BooleanLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

//Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

#endif
