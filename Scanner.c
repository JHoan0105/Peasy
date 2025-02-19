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
* Assignment: A22
* Date: March 8 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

  /* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
PZY_int line;								/* Current line number of the source code */
extern PZY_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern PZY_int stateType[];
extern PZY_char* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern PZY_int transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

PZY_int startScanner(BufferPointer psc_buf) {
	/* in case the buffer has been read previously  */
	bRecover(psc_buf);
	bClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(void) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	PZY_char c;	/* input symbol */
	PZY_int state = 0;		/* initial state of the FSM */
	PZY_int lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	PZY_int lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	PZY_int lexLength;		/* token length */
	PZY_int i;				/* counter */
	PZY_char newc;			/* new char */
	
	while (1) { /* endless loop broken by token returns it will generate a warning */
		
		c = bGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {

			/* Cases for spaces */
		case ' ':
		case '\t':
		case '\f':
			break;
		case '\n':
			line++;
			break;

			/* Cases for symbols */
		case ';':
			currentToken.code = EOS_T;
			currentToken.attribute.pzy_eofType = EOS_T;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			currentToken.attribute.codeType = -1;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			currentToken.attribute.codeType = -1;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			currentToken.attribute.codeType = -1;
			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			currentToken.attribute.codeType = -1;
			return currentToken;
		case ',':
			currentToken.code = COMA_T;
			currentToken.attribute.assSepOperator = OP_COMA;
			return currentToken;

			/* relational operators and assignment operator */
		case '>':
			currentToken.code = RO_T;
			currentToken.attribute.relationalOperator = OP_GT;
			return currentToken;
		case '<':
			currentToken.code = RO_T;
			currentToken.attribute.relationalOperator = OP_LT;
			return currentToken;
		case '!':
			newc = bGetChar(sourceBuffer);
			if (newc == '=') {
				currentToken.code = RO_T;
				currentToken.attribute.relationalOperator = OP_NE;
				return currentToken;
			}
			else {
				bRetract(sourceBuffer);
			}
		case '=':
			
			newc = bGetChar(sourceBuffer);
			if (newc == '=') {
				currentToken.code = RO_T;
				currentToken.attribute.relationalOperator = OP_EQ;
				return currentToken;
			}
			else {
				bRetract(sourceBuffer);
				currentToken.code = ASS_T;
				currentToken.attribute.assSepOperator = OP_ASS;
				return currentToken;
			}

			/* logical operators */
		case '&':
			currentToken.code = LO_T;
			currentToken.attribute.logicalOperator = OP_AND;
			return currentToken;
		case '|':
			currentToken.code = LO_T;
			currentToken.attribute.logicalOperator = OP_OR;
			return currentToken;
		case '^':
			currentToken.code = LO_T;
			currentToken.attribute.logicalOperator = OP_NOT;
			return currentToken;

			/* Comments */
		case '#':
			
			newc = bGetChar(sourceBuffer);
			do {
				c = bGetChar(sourceBuffer);
				if (c == CHARSEOF0 || c == CHARSEOF255) {
					bRetract(sourceBuffer);
					//return currentToken;
				}
				else if (c == '\n') {
					line++;
				}
			} while (c != '#' && c != CHARSEOF0 && c != CHARSEOF255);
			
			break;


			/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.pzy_eofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.pzy_eofType = SEOF_255;
			return currentToken;
		case BUFFER_EOF:
			currentToken.code = SEOF_T;
			currentToken.attribute.pzy_eofType = SEOF_N1;
			return currentToken;

			/* Arithmetic operators */

		case '/':
			currentToken.code = AO_T;
			currentToken.attribute.arithmeticOperator = OP_DIV;
			return currentToken;
		case '*':
			currentToken.code = AO_T;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			return currentToken;
		case '+':
			newc = bGetChar(sourceBuffer);
			if (newc == '+') {
				currentToken.code = CCAT_T;
				currentToken.attribute.concat = CON_CAT;
				return currentToken;
			}
			if (!isdigit(newc)) {
				currentToken.code = AO_T;
				currentToken.attribute.arithmeticOperator = OP_ADD;
				bRetract(sourceBuffer);
				return currentToken;
			}
			bRetract(sourceBuffer);
		case '-':
			if (c != '+') {
				newc = bGetChar(sourceBuffer);
				if (!isdigit(newc)) {
					currentToken.code = AO_T;
					currentToken.attribute.arithmeticOperator = OP_SUB;
					bRetract(sourceBuffer);
					return currentToken;
				}
			bRetract(sourceBuffer);
			}
			
			/* ------------------------------------------------------------------------
				Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
				Note: Part 2 must follow Part 1 to catch the illegal symbols
				-----------------------------------------------------------------------
			*/

			/* TO_DO: Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = bGetReadPos(sourceBuffer) - 1;
			bSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOAS) {
				c = bGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == ASWR) bRetract(sourceBuffer);
			//retract if ER error retract
			//if (stateType[state] == ER) bRetract(sourceBuffer);

			lexEnd = bGetReadPos(sourceBuffer);
			
			lexLength = lexEnd - lexStart;
			
			//printf("lex length %d", lexLength);
			
			lexemeBuffer = bCreate((PZY_short)lexLength + 2, 0, MODE_FIXED);
			
			//printf("Lexemebuffer %s", lexemeBuffer->string);
	
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			bRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				bAddChar(lexemeBuffer, bGetChar(sourceBuffer));
			bAddChar(lexemeBuffer, CHARSEOF0);

			//printf("Lexemebuffer %s", lexemeBuffer->string);
			currentToken = (*finalStateTable[state])(bGetContent(lexemeBuffer, 0));
			printf("");
	
			bDestroy(lexemeBuffer);

			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

PZY_int nextState(PZY_int state, PZY_char c) {
	PZY_int col;
	PZY_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != IS);
	if (DEBUG)
		if (next == IS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
 /* TO_DO: Use your column configuration */

 /* Adjust the logic to return next column in TT */
 /*	[A-z](0), [0-9](1),	_(2), &(3), "(4), SEOF(5), other(6) */
/* CLASS */
PZY_int nextClass(PZY_char c) {
	PZY_int val = -1;
	switch (c) {
	case CHRCOL2:
		val = 2;
		break;
	case CHRCOL3_4:
		val = 3;
		break;
	case CHRCOL5:
		val = 5;
		break;
	case CHRCOL6:
		val = 6;
		break;
	case CHRCOL7:
		val = 7;
		break;
	case CHRCOL8:
		val = 8;
		break;
	case CHRCOL9:
		val = 9;
		break;
	case CHRCOL10:
		val = 10;
		break;
	case CHRCOL11:
		val = 11;
		break;
	case CHRCOL12:
		val = 12;
		break;
	case CHRCOL13:
		val = 13;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = ES;
		break;
	default:
		if (isalpha(c)) {
			val = 0;
		}		
		else if (isdigit(c)) {
			val = 1;
		}	
		else
			val = 14;
	}
	return val;
}


/*
 ************************************************************
 * Acceptance State Function VNID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcVID(PZY_char lexeme[]) {
	Token currentToken = { 0 };
	char firstch = lexeme[0];
	int isID = PZY_FALSE;
	//printf("");
	switch (firstch) {
	case VNIDPREFIX:
		currentToken.code = VNID_T;
		isID = PZY_TRUE;
		break;
	default:
		// Test Keyword
		currentToken = funcKEY(lexeme);
		break;
	}
	if (isID == PZY_TRUE) {
		strcpy(currentToken.attribute.idLexeme, "");
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		//currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}
	return currentToken;
}
/************************************************************
*Acceptance State Function MNID
* ******************FIXXXXXXXXXXXXXXXXXXX
* In this function, the pattern for IDs must be recognized.
* Since keywords obey the same pattern, is required to test if
* the current lexeme matches with KW from language.
* -Remember to respect the limit defined for lexemes(VID_LEN) and
*set the lexeme to the corresponding attribute(vidLexeme).
* Remember to end each token with the \0.
* -Suggestion: Use "strncpy" function.
* **********************************************************
*/
/* TO_DO: Adjust the function for ID */

Token funcMID(PZY_char lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	char lastch = lexeme[length-1];
	printf("");
	//printf("last char %c ", lastch);
	int isID = PZY_FALSE;
	switch (lastch) {
	case MNIDPOSTFIX:
		currentToken.code = MNID_T;
		isID = PZY_TRUE;
		break;
	default:
		// Test Keyword
		currentToken = funcKEY(lexeme);
		break;
	}
	if (isID == PZY_TRUE) {
		strcpy(currentToken.attribute.midLexeme, "");
		strcpy(currentToken.attribute.midLexeme, lexeme);
		//currentToken.attribute.midLexeme[VID_LEN] = CHARSEOF0;
	}
	//printf("%s midlexeme ", currentToken.attribute.midLexeme);
	
	return currentToken;
}

/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for SL */

Token funcSL(PZY_char lexeme[]) {
	Token currentToken = { 0 };
	PZY_int i = 0, len = (PZY_int)strlen(lexeme);
	currentToken.attribute.contentString = bGetWritePos(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!bAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!bAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = SL_T;
	return currentToken;
}

/************************************************************
 * Acceptance State Function CHARL
 * Function responsible to identify CHARL (CHAR literals).
 *
 ***********************************************************
 */
 /* TO_DO: Adjust the function for SL */

Token funcCHARL(PZY_char lexeme[]) {
	Token currentToken = { 0 };
	int len = (PZY_int)strlen(lexeme);
	int isID = PZY_FALSE;
	//printf("");
	if (len > 3){
		isID = PZY_TRUE;
	}else{
	// check with KEY
	        currentToken = funcKEY(lexeme);
	}
	if (isID == PZY_TRUE) {
		currentToken.code = CHARL_T;
		currentToken.attribute.charValue = lexeme[1];
	}
	return currentToken;
}


/************************************************************
*Acceptance State Function FID
* **************FIX
* Function responsible to identify SL(string literals).
* -The lexeme must be stored in the String Literal Table
* (stringLiteralTable).You need to include the literals in
* this structure, using offsets.Remember to include \0 to
* separate the lexemes.Remember also to incremente the line.
* **********************************************************
*/
/* TO_DO: Adjust the function for SL */

Token funcFID(PZY_char lexeme[]) {
	Token currentToken = { 0 };
	char firstch = lexeme[0];
	printf("");
	//printf("last char %c ", lastch);
	int isID = PZY_FALSE;
	switch (firstch) {
	case FIDPOSTFIX:
		currentToken.code = FID_T;
		isID = PZY_TRUE;
		break;
	default:
		// Test Keyword
		currentToken = funcKEY(lexeme);
		break;
	}
	if (isID == PZY_TRUE) {
		strcpy(currentToken.attribute.formatSpecifier, "");
		strcpy(currentToken.attribute.formatSpecifier, lexeme);
		//currentToken.attribute.midLexeme[VID_LEN] = CHARSEOF0;
	}
	//printf("%s lexeme ", currentToken.attribute.formatSpecifier);
	currentToken.code = FID_T;
	return currentToken;
}

/************************************************************
*Acceptance State Function NUML
* **************FIX
* Function responsible to identify SL(string literals).
* -The lexeme must be stored in the String Literal Table
* (stringLiteralTable).You need to include the literals in
* this structure, using offsets.Remember to include \0 to
* separate the lexemes.Remember also to incremente the line.
* **********************************************************
*/
/* TO_DO: Adjust the function for SL */

Token funcNUML(PZY_char lexeme[]) {
	Token currentToken = { 0 };
	PZY_long tlong;
	PZY_double tdoub;
	PZY_int i;
	size_t length = strlen(lexeme);
	BOL fltNum = PZY_FALSE;
	BOL expNum = PZY_FALSE;
	
	lexeme[length - 1] = '\0';
	for (i = 0; i < length; i++) if (lexeme[i] == '.') { fltNum = PZY_TRUE; break; } 
	for (i = (PZY_int)length; i > 0 ; i--) if (lexeme[i] == 'e' || lexeme[i] == 'E') { expNum = PZY_TRUE; break; }

	if (fltNum) {
		tdoub = atof(lexeme);
		if (tdoub <= SHRT_MAX) {
			currentToken.code = NUMF_T;
			currentToken.attribute.floatValue = (PZY_float)tdoub;
			//printf("%d  %s", tdoub, lexeme);
		}
		else {
			currentToken = (*finalStateTable[ES])(lexeme);
		}
	}
	else {
		if (strlen(lexeme) > NUM_LEN) currentToken = (*finalStateTable[ES])(lexeme);
		tlong = atol(lexeme);
		if ( tlong <= SHRT_MAX) {
			currentToken.code = NUML_T;
			currentToken.attribute.intValue = (PZY_int)tlong;
			//printf("%d  %s", tlong, lexeme);
		}
		else {
			currentToken = (*finalStateTable[ES])(lexeme);
		}
	}
	return currentToken;
}

/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(PZY_char lexeme[]) {
	
	Token currentToken = { 0 };
	PZY_int kwindex = -1, j = 0;
	//size_t length = strlen(lexeme);
	//if (!isspace(lexeme[length])) lexeme[length] = '\0';

	for (j = 0; j < KWT_SIZE; j++) {
		if (!strcmp(lexeme, &keywordTable[j][0])) { kwindex = j; break; }
		//printf("%s ", &keywordTable[j][0]);
	}
	if (kwindex != -1) {

		currentToken.code = KW_T;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(PZY_char lexeme[]) {
	Token currentToken = { 0 };
	PZY_int i = 0, len = (PZY_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

PZY_void printToken(Token t) {
	extern PZY_char* keywordTable[]; /* link to keyword table in */

	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.pzy_eofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.midLexeme);
		break;
	case VNID_T:
		printf("VNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case FID_T:
		printf("FID_T\t\t%s\n", t.attribute.formatSpecifier);
		break;
	case NUML_T:
		printf("NUML_T\t\t%d\n", t.attribute.intValue);
		break;
	case NUMF_T:
		printf("NUMF_T\t\t%f\n", t.attribute.floatValue);
		break;
	case SL_T:
		printf("STR_T\t\t%d\t ", (PZY_short)t.attribute.codeType);
		printf("%s\n", bGetContent(stringLiteralTable,
			(PZY_short)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\t\t%c\n", '(');
		break;
	case RPR_T:
		printf("RPR_T\t\t%c\n", ')');
		break;
	case LBR_T:
		printf("LBR_T\t\t%c\n", '{');
		break;
	case RBR_T:
		printf("RBR_T\t\t%c\n", '}');
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case AO_T:
		
		switch (t.attribute.arithmeticOperator) {
		case OP_ADD:
			printf("AO_T\t\t%c\n",'+');
			break;
		case OP_SUB:
			printf("AO_T\t\t%c\n", '-');
			break;
		case OP_MUL:
			printf("AO_T\t\t%c\n", '*');
			break;
		case OP_DIV:
			printf("AO_T\t\t%c\n", '/');
			break;
		}
		break;
	case RO_T:
		switch (t.attribute.relationalOperator) {
		case OP_EQ:
			printf("RO_T\t\t%c%c\n", '=','=');
			break;
		case OP_NE:
			printf("RO_T\t\t%c%c\n", '!','=');
			break;
		case OP_GT:
			printf("RO_T\t\t%c\n", '>');
			break;
		case OP_LT:
			printf("RO_T\t\t%c\n", '<');
			break;
		}
		break;
	case LO_T:
		switch (t.attribute.logicalOperator) {
		case OP_AND:
			printf("LO_T\t\t%c\n", '&');
			break;
		case OP_OR:
			printf("LO_T\t\t%c\n", '|');
			break;
		case OP_NOT:
			printf("LO_T\t\t%c\n", '&');
			break;
		}
		break;
	case ASS_T:
		printf("ASS_T\t\t%c\n",'=');
		break;
	case COMA_T:
		printf("COMA_T\t\t%c\n", ',');
		break;
	case CCAT_T:
		printf("CCAT_T\t\t%c%c\n", '+','+');
		break;
	case CHARL_T:
		printf("CHARL_T\t\t%c\n", t.attribute.charValue);
		break;
	case EOS_T:
		printf("EOS_T\t\t%c\n", ';');
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
