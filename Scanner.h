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
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: John Hoang
* Course: CST 8152 – Compilers, Lab Section: [013]
* Assignment: A22.
* Date: March 8 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20   /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1   /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: @ end: @) */
	VNID_T,		/*  2: Variable name identifier token (start: @ */
	SL_T,		/*  3: String literal token */
	FID_T,		/*  4: Format specifier token */
	NUML_T,		/*  5: Number literal token */
	NUMF_T,		/*  6: floating-point literal token */
	LPR_T,		/*  7: Left parenthesis token */
	RPR_T,		/*  8: Right parenthesis token */
	LBR_T,		/*  9: Left brace token */
	RBR_T,		/*  10: Right brace token */
	COMA_T,		/*  11: multi assignment / seperator */
	ASS_T,		/*  12: Assignment operator */
	KW_T,		/*  13: Keyword token */
	RO_T,		/*  14: Relational operator */
	AO_T,		/*  15: Arithmetic operator */
	LO_T,		/*  16: Logical Operator */
	CCAT_T,		/*  17: Concatenation operator */
	EOS_T,		/*  18: End of statement (semicolon) */
	CHARL_T,	/*  19: Char literal */
	RTE_T,		/*  20: Run-time error token */
	SEOF_T		/*  21: Source end-of-file token */
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum AssignmentSeperatorOperators { OP_ASS, OP_COMA } AssSepOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum StringConcate { CON_CAT } stringConcat;
typedef enum SourceEndOfFile { SEOF_N1=BUFFER_EOF,SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	PZY_int codeType;      /* integer attributes accessor */

	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	stringConcat concat;				/* string concatenation ++ */
	AssSepOperator assSepOperator;		/* assignment and separator comma */
	EofOperator pzy_eofType;				/* source-end-of-file attribute code */
	PZY_int keywordIndex;					/* keyword index in the keyword table */
	PZY_int contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */

	PZY_char charValue;				/* char value */
	PZY_uchar ucharValue;				/* char value */
	PZY_short shortValue;			/* short Integer value */
	PZY_int intValue;				/* Integer value */
	PZY_uint uintValue;				/* Unsigned Integer value */
	PZY_long longValue;				/* long value */
	PZY_ulong ulongValue;			/* unsigned long value */
	PZY_float floatValue;					/* floating-point literal attribute (value) */
	PZY_double doubleValue;			/* Float value */

	PZY_char errChar;				/* unexpected character error */
	PZY_char formatSpecifier[VID_LEN +1];		/* format Specifier */
	PZY_char idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	PZY_char midLexeme[VID_LEN + 1];		/* method identifier token attribute */
	PZY_char errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	FLG flags;			/* Flags information */
	union {
		PZY_char charValue;				/* char value */
		PZY_uchar ucharValue;				/* char value */
		PZY_short shortValue;			/* short Integer value */
		PZY_int intValue;				/* Integer value */
		PZY_uint uintValue;				/* Unsigned Integer value */
		PZY_long longValue;				/* long value */
		PZY_ulong ulongValue;			/* unsigned long value */
		PZY_float floatValue;			/* Float value */
		PZY_double doubleValue;			/* Float value */
		PZY_void* formatSpecifier;		/* format Specifier */
		PZY_void* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	PZY_int code;				/* token code */

	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space, #comment# , ',' , ';' , '-' , '+' , '*' , '/', ## ,
 *  .&., .|. , .!. , SEOF.
 */

 /* TO_DO: Error states and illegal state */
#define ER  13		/* Error state with retract */
#define ES  14		/* Error state with no retract */
#define IS -1		/* Illegal state */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 15

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3_4 '@'
#define CHRCOL5 '\"'
#define CHRCOL6 '&'
#define CHRCOL7 '\\'
#define CHRCOL8 '#'
#define CHRCOL9 '%'
#define CHRCOL10 '.'
#define CHRCOL11 '+'
#define CHRCOL12 '-'
#define CHRCOL13 '\''


/* This constant will be used on VNID function */
#define VNIDPREFIX '@'
/* This constant will be used on MNID function */
#define MNIDPOSTFIX '@'

/* This constant will be used for string literals */
#define SLPREFIX '\"'
/* This constant will be used on format specification*/
#define FIDPOSTFIX '%'

/* This constant will be used for comments */ 
#define CIDPREFIX '#'
/* This constant will be used for number literals */
#define NUMLPOSTFIX '&'
#define POSTFIXEOS ';'


/* TO_DO: Transition table - type of states defined in separate table */
static PZY_int transitionTable[][TABLE_COLUMNS] = {
	/*   [A-z] , [0-9],    _,    @,   @@,    ",		&,    \,     #,    %,     .,   		  ^[LDUVQECFP]
		   L(0),  D(1), U(2), V(3), M(4), Q(5),  N(6), E(7),  C(8),	F(9), P(10), Po+, Ne-, CH'  O(14) */
		{    12,     8,   ES,    1,    1,    4,   ES,    ES,    10,    6,   ES,	  8,	 8,	14,		ES}, // S0: NOAS (VNID/MNID -)
		{     1,     1,    1,    3,    3,    2,   ES,    ES,     2,   ES,   ES,	 ES,	ES,	 ER,	 2}, // S1: NOAS 
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,	IS,	 IS,	IS}, // S2: ASWR (VNID)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,	IS,  IS,	IS}, // S3: ASNR (MNID)
		{     4,     4,    4,    4,    4,    5,    4,     4,     4,    4,    4,	  4,	 4,	  4,	4}, // S4: NOAS (SL - )
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,	IS,  IS,	IS}, // S5: ASNR (SL)
		{     6,     6,    6,    7,   ES,    7,   ES,    ES,     7,   7,    6,	 ES,    ES,	 ES,	7}, // S6: NOAS (F - )
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,    IS,  IS,	IS}, // S7: ASWR (FID)
		{    ER,     8,   ES,   ER,   ER,   ES,    9,    ES,    ER,   ES,    8,	 ES,	ES,  ES,	ES},  // S8: NOAS (NUML --)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,	IS,  IS,	IS},  // S9: ASNR (NUML)
		{    10,    10,   10,   10,   10,   10,   10,    10,    11,   10,   10,	 10,	10,  10,	10},  // S10: NOAS (cid --)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,	IS,	 IS,	IS},  // S11: ASNR (CID)
		{    12,    12,   12,   13,   ER,   13,   ES,    ES,    13,   13,   ES,	 ES,	ES,  ER,	13},  // S12: NOAS (key --)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,	IS,	 IS,	IS}, // S13: ASWR (KEY)
		{    15,    15,	  15,	15,   15,   15,   15,	 15, 	15,   15,   15,	 15,	15,  15,	15}, // S14: NOAS (CHARL)
		{    ER,    ES,	  ES,	ER,   ER,   ER,	  ES,	 ES,	ER,   ES,   ES,	 ES,	ES,	 16,	ES}, // S15 NOAS (CHARL)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,	IS,  IS,	IS}, // S16 ASWR (CHARL)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,	IS,	 IS,	IS}, // S17: ASWR (ER)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,    IS,   IS,   IS,	 IS,	IS,	 IS,	IS}  // S18: ASNR (ES)
};

/* Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static PZY_int stateType[] = {
	NOAS, /* 00 */
	NOAS, /* 01 */
	ASWR, /* 02 (VID) - variable */
	ASNR, /* 03 (MID) - method */
	NOAS, /* 04  recursive SL*/
	ASNR, /* 05 (SL) - string literal */
	NOAS, /* 06  recursive FID */
	ASWR, /* 07 (FID) - format specifier */
	NOAS, /* 08  recursive NUML*/
	ASNR, /* 09 (NUML) - number literal */
	NOAS, /* 10  recursive comments */
	ASNR, /* 11 (CID) - comments */
	NOAS, /* 12  recursive KEY */
	ASWR, /* 13 (KEY) - reserved KEY */
	NOAS, /* 14 (CHARL) - single quote */
	NOAS, /* 15 (CHARL) - Single Char */
	ASWR, /* 16 (CHARL) - character */
	ASWR, /* 17 (Err1 - no retract) */
	ASNR  /* 18 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
PZY_int startScanner(BufferPointer psc_buf);
static PZY_int nextClass(PZY_char c);			/* character class function */
static PZY_int nextState(PZY_int, PZY_char);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(PZY_char* lexeme);

/* Declare accepting states functions */
Token funcSL(PZY_char lexeme[]);
Token funcNUML(PZY_char lexeme[]);
Token funcVID(PZY_char lexeme[]);
Token funcMID(PZY_char lexeme[]);
Token funcFID(PZY_char lexeme[]);
Token funcKEY(PZY_char lexeme[]);
Token funcCHARL(PZY_char lexeme[]);

Token funcErr(PZY_char lexeme[]);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcVID,	/* VNID	[02] */
	funcMID,	/* MNID	[03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcFID,	/* -    [07] */
	NULL,		/* NUML [08] */
	funcNUML,	/* -    [09] */
	NULL,		/* -    [10] */
	NULL,		/* CID  [11] */
	NULL,		/* -    [12] */
	funcKEY,	/* KEY	[13] */
	NULL,		/* CHARL[14] */
	NULL,		/* CHARL[15] */
	funcCHARL,	/* CHARL[16] */
	funcErr,	/* ERR1 [17] */
	funcErr		/* ERR2 [18] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 25

/* TO_DO: Define the list of keywords */
static PZY_char* keywordTable[KWT_SIZE] = {
	"START",		/*0*/
	"END",			/*1*/
	"PZY_int",		/*2*/
	"PZY_uint",		/*3*/
	"PZY_long",		/*4*/
	"PZY_ulong",	/*5*/
	"PZY_short",	/*6*/
	"PZY_float",	/*7*/
	"PZY_double",	/*8*/
	"charz",		/*9*/
	"PZY_char",		/*10*/
	"PZY_BOL",		/*11*/
	"PZY_void",		/*12*/
	"if",			/*13*/
	"else",			/*14*/
	"else_if",		/*15*/
	"while",		/*16*/
	"for",			/*17*/
	"rin",			/*18*/
	"pout",			/*19*/
	"RETURN",		/*20*/
	"VOID",			/*21*/
	"FILEW",		/*22*/
	"KEYB",			/*23*/
	"CONSOLE"		/*24*/
};

#endif
