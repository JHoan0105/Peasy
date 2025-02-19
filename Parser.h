/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa ; John Hoang Modified for Peasy
* Course: CST 8152 – Compilers, Lab Section: [013]
* Assignment: A12, A22, A32.
* Date: April 13 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef BUFFER_H_
#include "Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
PZY_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern PZY_int line;
extern Token tokenizer();
extern PZY_char* keywordTable[];
#define RELAOP(x) x==OP_EQ? "==": x==OP_LT?"<": x==OP_GT ? ">": "!="
#define ARTHOP(x) x==OP_ADD? '+': x==OP_SUB?'-': x==OP_DIV ? '/': '*'
#define LOGIOP(x) x==OP_AND? '&': x==OP_OR?'|': '^'
#define BRACENEOS(x) x==LPR_T? '(':x==RPR_T?')':x==LBR_T?'{':x==RBR_T?'}':';'

#define STR_LANGNAME "Peasy"


/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	START,		/*#0 */
	END,		/*#1 */
	PZY_INT,	/*#2 */	
	PZY_UINT,	/*#3 */
	PZY_LONG,	/*#4 */
	PZY_ULONG,	/*#5 */
	PZY_SHORT,	/*#6 */
	PZY_FLOAT,	/*#7 */
	PZY_DOUBLE,	/*#8 */
	CHARZ,		/*#9 */
	PZY_CHAR,	/*#10 */
	PZY_BOL,	/*#11 */
	PZY_VOID,	/*#12 */
	IF,			/*#13 */
	ELSE,		/*#14 */
	ELSE_IF,	/*#15 */
	WHILE,		/*#16 */
	FOR,		/*#17 */
	RIN,		/*#18 */
	POUT,		/*#19 */
	RETURN,		/*#20 */
	VOID,		/*#21 */
	FILEW,		/*#22 */
	KEYB,		/*#23 */
	CONSOLE		/*#24 */
};

/* Function definitions */
PZY_void startParser();
PZY_void matchToken(PZY_int, PZY_int);
PZY_void syncErrorHandler(PZY_int);
PZY_void printError();

/* TODO: Place ALL non-terminal function declarations */
PZY_void optAssignmentExpression();
PZY_void assignmentStatement();
PZY_void codeSession();
PZY_void dataSession();
PZY_void optVarListDecAss();
PZY_void varListDeclarations();
PZY_void datatypeList();
PZY_void datatypeListPrime();

PZY_void ifStatement();
PZY_void conditionalStatement();
PZY_void iterativeStatement();
PZY_void relationalExpression();
PZY_void logicalExpression();
PZY_void arithmeticExpression();
PZY_void functionDeclaration();
PZY_void functionCallStatement();
PZY_void optionalStatements();
PZY_void outputStatement();
PZY_void outputVariableList();
PZY_void inputStatement();
PZY_void inputVariableList();
PZY_void formatSpecifierList();
PZY_void formatSpecifier();
PZY_void program();
PZY_void session();
PZY_void statement();

#endif
