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
* File name: mainBuffer.c
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa ; John Hoang Modified for Peasy
* Course: CST 8152 – Compilers, Lab Section: [013]
* Assignment: A12, A22, A32.
* Date: April 13 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TODO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif


/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TODO: This is the function to start the parser - check your program definition */

PZY_void startParser() {
	lookahead = tokenizer();
	
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TODO: This is the main code for match - check your definition */
PZY_void matchToken(PZY_int tokenCode, PZY_int tokenAttribute) {
	PZY_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
		break;
	case RO_T:
		if (lookahead.attribute.relationalOperator != tokenAttribute)
			matchFlag = 0;
		break;
	case COMA_T:
		if (lookahead.attribute.assSepOperator != tokenAttribute)
			matchFlag = 0;
		break;
	case ASS_T:
		if (lookahead.attribute.assSepOperator != tokenAttribute)
			matchFlag = 0;
		break;
	case AO_T:
		if (lookahead.attribute.arithmeticOperator != tokenAttribute)
			matchFlag = 0;
		break;
	case LO_T:
		if (lookahead.attribute.logicalOperator != tokenAttribute)
			matchFlag = 0;
		break;
	case CCAT_T:
		if (lookahead.attribute.concat != tokenAttribute)
			matchFlag = 0;
		break;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TODO: This is the function to handler error - adjust basically datatypes */
PZY_void syncErrorHandler(PZY_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TODO: This is the function to error printing - adjust basically datatypes */
PZY_void printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("ERR_T %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.pzy_eofType);
		break;
	case MNID_T:
		printf("MNID %s\n", t.attribute.midLexeme);
		break;
	case VNID_T :
		printf("VNID %s\n", t.attribute.idLexeme);
		break;
	case SL_T:
		printf("SL %s\n", bGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case FID_T:
		printf("FID %s\n", t.attribute.formatSpecifier);
		break;
	case NUML_T:
		printf("NUML %d\n", t.attribute.longValue);
		break;
	case NUMF_T:
		printf("NUMF %f\n", t.attribute.doubleValue);
		break;
	case COMA_T:
		printf("COMA %c\n", ',');
		break;
	case ASS_T:
		printf("ASS %c\n", '=');
		break;
	case RO_T:
		printf("RO %s\n", RELAOP(t.attribute.relationalOperator) );
		break;
	case AO_T:
		printf("AO %c\n", ARTHOP(t.attribute.arithmeticOperator));
		break;
	case LO_T:
		printf("LO %c\n", LOGIOP(t.attribute.logicalOperator));
		break;
	case CCAT_T:
		printf("CC %c%c\n", '+','+');
		break;
	case CHARL_T:
		printf("CHARL %c\n", t.attribute.charValue);
		break;
	case KW_T:
		printf("KW %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case EOS_T:
		printf("BrPaEos %c\n",BRACENEOS(t.code) );
		break;
	case RTE_T:
		printf("GRAVE ERR | Run-time-ERROR\n");
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> PEASY { <session> }
 * FIRST(<program>)= {KW_T (START), KW_T(END) ,KW_T(SEOF_T) , FIRST(<session>) }.
 ***********************************************************
 */
PZY_void program() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == START)
			matchToken(KW_T, START);

		session();
	if (lookahead.attribute.codeType == END) matchToken(KW_T, END);


	case SEOF_T:
		if (lookahead.code != SEOF_T) {
			printError();
			session();
			if (lookahead.attribute.codeType == END) matchToken(KW_T, END);
			if (lookahead.code == SEOF_T) break;
		} // Empty
		break;
	case END:
		if (lookahead.attribute.codeType == END) matchToken(KW_T, END);
		if (lookahead.code == SEOF_T)
			break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
* if ((strncmp(lookahead.attribute.idLexeme, "WRITE&", 6) == 0)) {
			statements();
			break;
		}
*/

/*
 ************************************************************
 * Session statement
 * BNF: <session> ->  <data session> | <code session> | <session prime>
 * <session prime> -> <session> | e
 * 
 * FIRST(<session>)= { < FIRST(<data session>) >, < FIRST(<code session>) >, e }.
 ***********************************************************
 */
PZY_void session() {

	switch (lookahead.code) {
		if (lookahead.code == RBR_T) return; 
		if (lookahead.attribute.codeType == END) break;
	case VNID_T:
	case LPR_T:
	case AO_T:
	case LO_T:
	case RO_T:
	case ASS_T:
	case KW_T:
	case MNID_T:
		if (lookahead.code == RBR_T) return;
		dataSession();

		codeSession();	
	case RBR_T:
		if (lookahead.code == RBR_T) return;
		if (lookahead.attribute.codeType == END) {
			break;
		}
	case SEOF_T:
		if (lookahead.code == SEOF_T) {
			break;
		}
		session();
		return;
	default:
		if (lookahead.code == RBR_T) return;
		return;
	}
	
	printf("%s%s\n", STR_LANGNAME, ": Session parsed");
}

/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> <optDeclaration> | e
 * <optDeclaration> -> <VarListDeclarations> ; | <functionDeclaration>
 * 
 * 
 * FIRST(<dataSession>)= { FIRST(<optVarListDecAss>) , KWT(ASS_T), KWT(COMA_T), 
						   FIRST(<optAssignmentExpression>), FIRST(<functionDeclaration>),
						   KWT(EOS_T) ,e }
 ***********************************************************
 */
PZY_void dataSession() {
	if (lookahead.attribute.codeType == END
		|| lookahead.code == POUT || lookahead.code == RIN
		|| lookahead.code == CONSOLE || lookahead.code == FILEW
		|| lookahead.code == AO_T|| lookahead.code == RO_T
		|| lookahead.code == LO_T || lookahead.code == RBR_T) return;
	optVarListDecAss();
	if (lookahead.attribute.codeType == ASS_T) {
		matchToken(KW_T, ASS_T);
		optAssignmentExpression();
	}
	if (lookahead.code == MNID_T) statement(); //functionDeclaration
	
	if(lookahead.code == EOS_T)
		matchToken(EOS_T, EOS_T);

	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}

/*
 ************************************************************
 * Optional Var List Declarations
 * use Switch to simulate  -> <opt_datatype> <variable> <opt ( = | , | e)>  <optVarlist> <opt ( = | , | e)> <assignmentExpression> <prime> ; 
 * each case is used as an Identifier to Assignment declaration of <type> to expression
 * 
 * BNF: <optVarlistDecAss> -> <varlist_declarations><optAssignmentExpression> <optVarListDecAssPrime> ;
 * <optVarlistDecAssPrime> -> <opt ( = | , | e)> <optAssignmentExpression> <optVarListDecAssPrime> | e
 * 
 * FIRST(<optVarlistDecAssPrime>) = { e , KWT(ASS_T), KWT(COMA_T), 
								FIRST (<dataListype>) , FRIST(VarListDeclarations), 
								FIRST(optAssignmentExpression), KWT(EOS_T) }.

  FIRST(<optVarListDecAss>) = { e , FIRST (<dataListype>) , FRIST(VarListDeclarations) }.
								
  FOLLOW(<optVarListDecAss>) = { KWT(ASS_T),KWT(COMA_T), KWT(EOS_T) }
 ***********************************************************
 */
PZY_void optVarListDecAss() {

	switch (lookahead.attribute.codeType) {
	case PZY_INT:
		matchToken(KW_T, PZY_INT);
		multiIntAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != NUML_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);
				}
				goto multiIntAss;
				printf("%s%s\n", STR_LANGNAME, ": PZY_int Variable Identifier Assignment parsed");
				break;
			}
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_int Variable Identifier Declaration parsed");
		break;
	case PZY_UINT:
		matchToken(KW_T, PZY_UINT);
		multiUIntAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != NUML_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);
				}
				goto multiUIntAss;
			}
			printf("%s%s\n", STR_LANGNAME, ": PZY_uint Variable Identifier Assignment parsed");
			break;
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_uint Variable Identifier Delcaration parsed");
		break;
	case PZY_LONG:
		matchToken(KW_T, PZY_LONG);
		multiLongAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != NUML_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);
				}
				goto multiLongAss;
			}
			printf("%s%s\n", STR_LANGNAME, ": PZY_long Variable Identifier Assignment parsed");
			break;
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_long Variable Identifier Declaration parsed");
		break;
	case PZY_ULONG:
		matchToken(KW_T, PZY_ULONG);
		multiULongAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != NUML_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);	
				}
				goto multiULongAss;
			}
			printf("%s%s\n", STR_LANGNAME, ": PZY_ulong Variable Identifier Assignment parsed");
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_ulong Variable Identifier Declaration parsed");
		break;
	case PZY_SHORT:
		matchToken(KW_T, PZY_SHORT);
		multiShortAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != NUML_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);
				}
				goto multiShortAss;
			}
			printf("%s%s\n", STR_LANGNAME, ": PZY_short Variable Identifier Assignment parsed");
			break;
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_short Variable Identifier Declaration parsed");
		break;
	case PZY_FLOAT:
		matchToken(KW_T, PZY_FLOAT);
		multiFloatAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != NUMF_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);
				}
				goto multiFloatAss;
			}
			printf("%s%s\n", STR_LANGNAME, ": PZY_float Variable Identifier Assignment parsed");
			break;
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_float Variable Identifier Declaration parsed");
		break;
	case PZY_DOUBLE:
		matchToken(KW_T, PZY_DOUBLE);
		multiDoubleAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != NUMF_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);	
				}
				goto multiDoubleAss;	
			}
			printf("%s%s\n", STR_LANGNAME, ": PZY_double Variable Identifier Assignment parsed");
			break;
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_double Variable Identifier Declaration parsed");
		break;
	case CHARZ:
		matchToken(KW_T, CHARZ);
		multiCharzAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != SL_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);
					
				}
				goto multiCharzAss;
			}
			printf("%s%s\n", STR_LANGNAME, ": charz Variable Identifier Assignment parsed");
			break;
		}
		printf("%s%s\n", STR_LANGNAME, ": charz Variable Identifier Declaration parsed");
		break;
	case PZY_BOL:
		matchToken(KW_T, PZY_BOL);
		multiBoolAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != CHARL_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);		
				}
				goto multiBoolAss;
			}
			printf("%s%s\n", STR_LANGNAME, ": PZY_BOL Variable Identifier Assignment parsed");
			break;
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_BOL Variable Identifier Declaration parsed");
		break;
	case PZY_CHAR:
		matchToken(KW_T, PZY_CHAR);
		multiCharAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; }
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != CHARL_T) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);	
				}
				goto multiCharAss;
			}
			printf("%s%s\n", STR_LANGNAME, ": PZY_char Variable Idendifier Assignment parsed");
			break;
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_char Variable Idendifier Declaration parsed");
		break;
	case PZY_VOID:
		matchToken(KW_T, PZY_VOID);
		multiVoidAss:
		varListDeclarations();
		if (lookahead.code == EOS_T) { break; } 
		if (lookahead.code == ASS_T) {
			matchToken(ASS_T, OP_ASS);
			if (lookahead.code != VOID) break;
			else {
				optAssignmentExpression();
				if (lookahead.attribute.codeType == COMA_T) {
					matchToken(KW_T, COMA_T);
				}
				goto multiVoidAss;
			}
			printf("%s%s\n", STR_LANGNAME, ": PZY_void Variable Identifier Assignment parsed");
			break;
		}
		printf("%s%s\n", STR_LANGNAME, ": PZY_void Variable Identifier Declaration parsed");
		break;
	case END:
		break;
	default:
		return; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*
 ************************************************************
 * Var List Declarations
 * BNF: <varlist_declarations> -> <varlist_declaration><var_listPrime> | e
 * <var_listPrime> -> , VNID_T <var_listPrime> |e
 * <varList_declaration> -> VNID_T
 * 
 * FIRST(<varlistPrime>) = { VNID_T , KWT(COMA_T) , e}.
 ***********************************************************
 */
PZY_void varListDeclarations() {
	//if (lookahead.code != VNID_T) return;
	if (lookahead.code == VNID_T)
		matchToken(VNID_T, NO_ATTR);


	if (lookahead.attribute.assSepOperator == OP_COMA) {
		matchToken(COMA_T, OP_COMA);
		varListDeclarations();
		return;
	}


	printf("%s%s\n", STR_LANGNAME, ": Variable List Declarations parsed");
}
/*
 ************************************************************
 * Datatype List Prime
 * BNF: <datatype_list_Prime> -> <datatypeList> <data_list_Prime> | e
 * <data_list_Prime> -> , <datatypeList> <varList_declaration> | e
 * <datatypeList> -> <datatype> | e
 * 
 * FIRST(<datatype_list_Prime>) = { FIRST(<datatypeList>) ,VNID_T , KWT(COMA_T), e }.
 ***********************************************************
 */
PZY_void datatypeListPrime() {
	if (lookahead.code == VNID_T)
		matchToken(VNID_T, NO_ATTR);

	if (lookahead.attribute.assSepOperator == OP_COMA) {
		matchToken(COMA_T, OP_COMA);
		datatypeList();
		return;
	}


	printf("%s%s\n", STR_LANGNAME, ": Datatype List Prime parsed");
}
/*
 ************************************************************
 * Data Type List 
 * BNF: <Data Type List> -> <DataType> | e

 * <datatype> -> PZY_int | PZY_double | PZY_long | PZY_ulong | PZY_char
 *				| charz | PZY_void |...
 * 
 * FIRST(<Data Type list>) = { KW_T( <dataTypes> ) , e}.
 ***********************************************************
 */
PZY_void datatypeList() {
	switch (lookahead.attribute.codeType) {
	case PZY_INT:
		matchToken(KW_T, PZY_INT);
		printf("%s%s\n", STR_LANGNAME, ": PZY_int Identifier parsed");
		break;
	case PZY_UINT:
		matchToken(KW_T, PZY_UINT);
		printf("%s%s\n", STR_LANGNAME, ": PZY_uint Identifier parsed");
		break;
	case PZY_LONG:
		matchToken(KW_T, PZY_LONG);
		printf("%s%s\n", STR_LANGNAME, ": PZY_long Identifier parsed");
		break;
	case PZY_ULONG:
		matchToken(KW_T, PZY_ULONG);
		printf("%s%s\n", STR_LANGNAME, ": PZY_ulong Identifier parsed");
		break;
	case PZY_SHORT:
		matchToken(KW_T, PZY_SHORT);
		printf("%s%s\n", STR_LANGNAME, ": PZY_short Identifier parsed");
		break;
	case PZY_FLOAT:
		matchToken(KW_T, PZY_FLOAT);
		printf("%s%s\n", STR_LANGNAME, ": PZY_float Identifier parsed");
		break;
	case PZY_DOUBLE:
		matchToken(KW_T, PZY_DOUBLE);
		printf("%s%s\n", STR_LANGNAME, ": PZY_double Identifier parsed");
		break;
	case CHARZ:
		matchToken(KW_T, CHARZ);
		printf("%s%s\n", STR_LANGNAME, ": charz Identifier parsed");
		break;
	case PZY_BOL:
		matchToken(KW_T, PZY_BOL);
		printf("%s%s\n", STR_LANGNAME, ": PZY_BOL Identifier parsed");
		break;
	case PZY_CHAR:
		matchToken(KW_T, PZY_CHAR);
		printf("%s%s\n", STR_LANGNAME, ": PZY_char Identifier parsed");
		break;
	case PZY_VOID:
		matchToken(KW_T, PZY_VOID);
		printf("%s%s\n", STR_LANGNAME, ": PZY_void Identifier parsed");
		break;
	default:
		return; // Empty
	}
	datatypeListPrime();
	printf("%s%s\n", STR_LANGNAME, ": DatatypeList parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> { | ( | <optional statements> | ) | } | ; | e 
 * 
 * FIRST(<codeSession>)= { KWT(LBR_T), KWT(LPR_T), FIRST(<optionalstatments>) , e}
							
   FOLLOW(<codeSession>) = { KWT(RPR_T), KWT(RBR_T), KWT(EOS_T) }
 ***********************************************************
 */
PZY_void codeSession() {
	if (lookahead.attribute.codeType == END ) return;
	if (lookahead.code == RBR_T) return;
	if (lookahead.code == LPR_T) {
		matchToken(LPR_T, NO_ATTR);
	}

	optionalStatements();
	if (lookahead.code == RBR_T) return;
	if (lookahead.code == RPR_T) {
		matchToken(RPR_T, NO_ATTR);
		matchToken(EOS_T, EOS_T);
	}
	if (lookahead.code == EOS_T)
		matchToken(EOS_T, EOS_T);
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/*
 ************************************************************
 * Optional statement
 * BNF: <Optional Statement> -> <statement> | e
 * 
 * 
 * FIRST(<Optional Statement>) = { e , FIRST(<statement>), FIRST(<ConditionalStatement>),
									FIRST(<logical/arithmetic/relational EXPRESSION>),
									FIRST(<assignment statement>),
									FIRST(POUT), FIRST(RIN), VNID_T, MNID_T,
									KW(POUT), KW(RIN), KW(IF),KW(ElSEIF),
									KW(ElSE) ,KW(WHILE),KW(FOR), 
									FIRST(<functionDeclaration)>}
 * 
 ***********************************************************
 */
PZY_void optionalStatements() {
	switch (lookahead.code) {
	case RPR_T:break;
	case ASS_T:
		matchToken(ASS_T, OP_ASS);
		optAssignmentExpression();
		break;
	case KW_T:
		statement();
		break;
	case VNID_T: 		
		statement(); 
		break;
	case MNID_T:
		matchToken(MNID_T, NO_ATTR);
		functionDeclaration();
		break;
	case LBR_T:
		break;
	case AO_T:
		arithmeticExpression();
		break;
	case RO_T:
		relationalExpression();
		break;
	case LO_T:
		logicalExpression();
		break;
	case RBR_T: return;
	default:
		return; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}
/*
 ************************************************************
 * Assignment Statement
 * BNF: <assignment statement> -> = <opt assignment expression> | ;
 * 
 * FIRST(<assignment statement>) = { KW(ASS_T), FIRST(<optAssignmentExpression>),
									KW(EOS_T) }
 ***********************************************************
 */
PZY_void assignmentStatement() {
	optAssignmentExpression();
	if (lookahead.code == COMA_T) {
		assignmentStatement();
		return;
	}
	if (lookahead.code == EOS_T)
		matchToken(EOS_T, EOS_T);

	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}


/*
 ************************************************************
 * Option Assignment Expression
 * BNF: <opt Assignment Expression> ->  <assignment expression> | e
 * <assignment expression> -> 
 * 
 * FIRST(<opt Assignment Expression>) = { e, KW(ASS_T), NUMF_T, NUML_T, VNID_T,
											CHARL_T, SL_T, CCAT_T, MNID_T, FID_T, 
											KW(POUT), KW(RIN), FIRST(<arithmeticExpression>),
											FIRST(<logicalExpression>), FIRST(<relationalExpression>),
											FIRST(<functionCallStatement>) }
 ***********************************************************
 */
PZY_void optAssignmentExpression() {
	PZY_float tmpflt = 0.01f;
	PZY_double tmpdbl = 0.01;
	PZY_long tmpLong;
	PZY_ulong tmpuLong;
	PZY_int tmpInt;
	PZY_uint tmpuInt;
	PZY_char tmpChar = 0;
	switch (lookahead.code) {
	case NUMF_T:
		if ((tmpflt += lookahead.attribute.floatValue) == (PZY_float)lookahead.attribute.floatValue + 0.01f) {
			matchToken(NUMF_T, PZY_FLOAT);
			printf("%s%s\n", STR_LANGNAME, ": Pzy_float Assignment Expression parsed");
		}
		else if ((tmpdbl += lookahead.attribute.doubleValue) == (PZY_double)lookahead.attribute.floatValue + 0.01){
			matchToken(NUMF_T, PZY_DOUBLE);
			printf("%s%s\n", STR_LANGNAME, ": Pzy_double Assignment Expression parsed");
		}
		break;
	case NUML_T:
		if ((tmpInt = lookahead.attribute.intValue) == (PZY_int)lookahead.attribute.intValue){
			matchToken(NUML_T, PZY_INT);
			printf("%s%s\n", STR_LANGNAME, ": Pzy_int Assignment Expression parsed");
		}
		else if ((tmpuInt = lookahead.attribute.ulongValue) == (PZY_uint)lookahead.attribute.ulongValue){
			matchToken(NUML_T, PZY_UINT);
			printf("%s%s\n", STR_LANGNAME, ": Pzy_uint Assignment Expression parsed");
		}
		else if ((tmpLong = lookahead.attribute.longValue) == (PZY_long)lookahead.attribute.longValue){
			matchToken(NUML_T, PZY_LONG);
			printf("%s%s\n", STR_LANGNAME, ": Pzy_long Assignment Expression parsed");
		}
		else if ((tmpuLong = lookahead.attribute.ulongValue) == (PZY_ulong)lookahead.attribute.ulongValue){
			matchToken(NUML_T, PZY_ULONG);
			printf("%s%s\n", STR_LANGNAME, ": Pzy_ulong Assignment Expression parsed");
		}
		break;
	case CHARL_T:
		if ((tmpChar = lookahead.attribute.charValue) == (PZY_char)lookahead.attribute.charValue){
			matchToken(CHARL_T, PZY_CHAR);
			printf("%s%s\n", STR_LANGNAME, ": Pzy_char Assignment Expression parsed");
		}
		break;
	case SL_T:
		matchToken(SL_T, NO_ATTR);
		if (lookahead.code == COMA_T) break;
		if (lookahead.code == CCAT_T) {
			optAssignmentExpression();
			return;
		}
		printf("%s%s\n", STR_LANGNAME, ": charz Assignment Expression parsed");
		break;
	case FID_T:
		matchToken(FID_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Format Specifier Assignment Expression parsed");
		break;
	case CCAT_T:
		matchToken(CCAT_T, CON_CAT);
		if (lookahead.code == SL_T) {
			optAssignmentExpression();
			printf("%s%s\n", STR_LANGNAME, ": charz concatenation Assignment Expression parsed");
			return;
		}
		break;
	case MNID_T:
		matchToken(MNID_T, NO_ATTR);
		functionCallStatement();
		printf("%s%s\n", STR_LANGNAME, ": Function Call Assignment Expression parsed");
		break;
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case RIN:
			matchToken(KW_T, RIN);
			inputStatement();
			break;
		case POUT:
			matchToken(KW_T, POUT);
			outputStatement();
			break;
		}
		break;
	case RO_T:
	case LO_T:
	case AO_T:
	case VNID_T:
		statement();
		break;
	default:
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Option Assignment Expression parsed");
}

/*
 ************************************************************
 * Function Declaration
 * 
 * BNF: <functionDeclaration> -> ( <datatypelistPrime> ) <functionDefinition> 
 * <functionDefinition> { <session> } | ; | e
 * 
 * FIRST(<functionDeclaration>) = { e, FIRST(<datatypeListPrime>), FIRST(<functionDefinition),
 *									FIRST(<session>) ,KW(LPR_T), KW(RPR_T), KW(LBR_T) }
 * FOLLOW(<functionDeclaration>) = { e , KW(RBR_T), KW(RPR_T), KW(EOS_T); }
 * 
 ***********************************************************
 */
PZY_void functionDeclaration() {
	//parameter
	matchToken(LPR_T, NO_ATTR);
	datatypeList();
	datatypeListPrime();
	if (lookahead.code == RPR_T)
		matchToken(RPR_T, NO_ATTR);
	if (lookahead.code == EOS_T) {
		matchToken(EOS_T, EOS_T);
		goto FUNCDECLARED;
	}	

	//if defining function
	if (lookahead.code == LBR_T) {
		matchToken(LBR_T, NO_ATTR);
		session();
	}
	if (lookahead.code == RBR_T)
		matchToken(RBR_T, NO_ATTR);
	FUNCDECLARED:
	printf("%s%s\n", STR_LANGNAME, ": functionDeclaration parsed");
}
/*
 ************************************************************
 * Function Call Statement
 * BNF: <functionCallStatement> -> ( <statement> <statementPrime> ) ;
 * <statementPrime> -> , <statement> <statementPrime> | e
 * 
 * FIRST(<functionCallStatement>) = { FIRST(<statement>) , FIRST(<statementPrime>) , 
 *										KW(LPR_T) }
 * FOLLOW(<functionCallStatement>) = { FOLLOW(<statementPrime>), KW(COMA_T) , KW(RPR_T), KW(EOS_T) }
 * 
 * FIRST(<statementPrime>) = { e, VNID, MNID, NUML_T, NUMF_T, SL_T, CHARL_T, FID_T, 
 *								FIRST(<optAssignmentStatement>), KW(COMA_T),
								FIRST(logical/arithmetic/relationalExpression) }
 ***********************************************************
 */
PZY_void functionCallStatement() {
	matchToken(LPR_T, NO_ATTR);
	if(lookahead.code == VNID_T || lookahead.code == NUML_T ||
		lookahead.code == NUMF_T || lookahead.code == SL_T ||
		lookahead.code == CHARL_T)
		while (lookahead.code != RPR_T) {
			statement();
		}
		
	if (lookahead.code == RPR_T)
		matchToken(RPR_T, NO_ATTR);

	if (lookahead.code == EOS_T)
		matchToken(EOS_T, EOS_T);

	printf("%s%s\n", STR_LANGNAME, ": functionCall Statement parsed");
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> ->  <assignment statement> | <Conditional statement> |
 *						<input statement> | <output statement> | <return statement> |
 *						<functionStatement>  | e
 * 
 * FIRST(<statement>) = { e, MNID_T, VNID_T, SL_T, CHARL_T, FID_T,
 *							NUML_T, NUMF_T, FIRST(<datatypelist>),
							KW_T(IF), KW_T(ELSEIF), KW_T(ELSE),
							KW_T(WHILE), KW_T(FOR), 
 *							KW_T(RIN), KW_T(POUT), KW_T(RETURN), 
							KW_T(ASS_T, KW_T(COMA_T), KW_T(CCAT_T),
							FIRST(<conditionalStatement>), FIRST(<assignmentStatement>)}
 ***********************************************************
 */
PZY_void statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case PZY_INT:
		case PZY_UINT:
		case PZY_LONG:
		case PZY_ULONG:
		case PZY_FLOAT:
		case PZY_DOUBLE:
		case PZY_CHAR:
		case PZY_BOL:
		case PZY_VOID:
			datatypeList();
			break;
		case CHARZ:
			datatypeList();
			printf("%s%s\n", STR_LANGNAME, ": charz Identifier Declaration parsed");
			break;
		case IF:
		case ELSE_IF:
		case ELSE:
		case WHILE:
		case FOR:
			conditionalStatement();
			break;
		case RETURN:
			matchToken(KW_T, RETURN);
			if (lookahead.code != EOS_T) optionalStatements();
			if (lookahead.code == EOS_T) matchToken(EOS_T, EOS_T);
			break;
		case RIN:
			matchToken(KW_T, RIN);
			inputStatement();
			break;
		case POUT:
			matchToken(KW_T, POUT);
			outputStatement();
			break;
		case FILEW:
			matchToken(KW_T, FILEW);
			break;
		case KEYB:
			matchToken(KW_T, KEYB);
			break;
		case CONSOLE:
			matchToken(KW_T, CONSOLE);
			break;
		case END:
		default:
			return;
		}
		break;
	case MNID_T:
		matchToken(MNID_T, NO_ATTR);
		functionDeclaration();
		break;
	case NUML_T:
		matchToken(NUML_T, NO_ATTR);
		if (lookahead.code == COMA_T) break;
		if (lookahead.code != EOS_T && lookahead.code != RPR_T) {
			statement();
			return;
		}
		break;
	case NUMF_T:
		matchToken(NUMF_T, NO_ATTR);
		if (lookahead.code == COMA_T) break;
		if (lookahead.code == RO_T || lookahead.code == LO_T ||
			lookahead.code == AO_T ) {
			statement();
			return;
		}
		break;
	case SL_T:
		matchToken(SL_T, NO_ATTR);
		if (lookahead.code == COMA_T) break;
		if (lookahead.code == CCAT_T) {
			statement();
		}
		printf("%s%s\n", STR_LANGNAME, ": charz Identifier parsed");
		break;
	case FID_T:
		matchToken(FID_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Format Specifier Identifier parsed");
		break;
	case CCAT_T:

		matchToken(CCAT_T, CON_CAT);
		if (lookahead.code == SL_T){
			statement();
			return;
		}
		printf("%s%s\n", STR_LANGNAME, ": Concatenation Identifier parsed");
		break;
	case ASS_T:
		matchToken(ASS_T, OP_ASS);
		assignmentStatement();
		break;
	case VNID_T:
		matchToken(VNID_T, NO_ATTR);
		if (lookahead.code == ASS_T) { statement(); return; }
		if (lookahead.code == COMA_T) break;
		if (lookahead.code != EOS_T && lookahead.code != RPR_T) {
			statement();
			return;
		}
		break;
	case RO_T:
		relationalExpression();
		return;
	case LO_T:
		logicalExpression();
		return;
	case AO_T:
		arithmeticExpression();
		return;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		return;
	case LBR_T:
		matchToken(LBR_T, NO_ATTR);
		return;
	case RBR_T:
	case COMA_T:
	case RPR_T: // parenthesis end of statement
	case EOS_T:
		return;// end of statement
	default:
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Relational Expression statement
 * BNF: <relationalExpression> -> <VARIABLE/Literal> <relationalExpressionPrime>
 * <relationalExpressionPrime> -> <opt < | > | != | == > <VARIABLE/Literal> <relationalExpressionPrime> | e
 * 
 * FIRST(<relationalExpression>) = { e, FIRST(<opt <| > | != | == > ),  FIRST(<VARIABLE/Literal>),
									SL_T, NUMF_T, VNID_T, CHARL_T, KW(LPR_T) ,
									KW(OP_GT), KW(OP_LT), KW(OP_NE), KW(OP_EQ) }
 ***********************************************************
 */
PZY_void relationalExpression() {
	if (lookahead.code == SL_T || lookahead.code == NUML_T ||
		lookahead.code == NUMF_T || lookahead.code == VNID_T ||
		lookahead.code == CHARL_T || lookahead.code == LPR_T) {
		statement();
	}
	if (lookahead.attribute.relationalOperator == OP_GT) {
		matchToken(RO_T, OP_GT);
		printf("%s%s\n", STR_LANGNAME, ": Relational Operator Greater Than parsed");
	}
	else if (lookahead.attribute.relationalOperator == OP_LT){
		matchToken(RO_T, OP_LT);
		printf("%s%s\n", STR_LANGNAME, ": Relational Operator Less Than parsed");
	}
	else if (lookahead.attribute.relationalOperator == OP_EQ) {
		matchToken(RO_T, OP_EQ);
		printf("%s%s\n", STR_LANGNAME, ": Relational Operator Equals parsed");
	}
	else if (lookahead.attribute.relationalOperator == OP_NE) {
		matchToken(RO_T, OP_NE);
		printf("%s%s\n", STR_LANGNAME, ": Relational Operator Not Equals parsed");
	}
	if (lookahead.code == SL_T || lookahead.code == NUML_T ||
		lookahead.code == NUMF_T || lookahead.code == VNID_T ||
		lookahead.code == CHARL_T || lookahead.code == LPR_T) {
		relationalExpression();
		return;
	}
	if (lookahead.code == RPR_T) matchToken(RPR_T, NO_ATTR);
	if (lookahead.code == EOS_T) matchToken(EOS_T, EOS_T);

	printf("%s%s\n", STR_LANGNAME, ": relationalExpression parsed");
}
/*
 ************************************************************
 * Arithmetic Expression statement
 * BNF: <ArithmeticExpression> -> <VARIABLE/Literal> <arithmeticExpressionPrime>
 * <arithmeticExpressionPrime> -> <opt * | / | + | - >  <VARIABLE/Literal> <arithmeticExpressionPrime> | e
 *
 * FIRST(<arithmeticExpression>) = { e, FIRST(<opt * | / | + | - > ),  FIRST(<VARIABLE/Literal>),
								   NUMF_T, VNID_T, CHARL_T, KW(LPR_T) ,
								   KW(OP_ADD), KW(OP_SUB), KW(OP_DIV), KW(OP_MUL) }
 ***********************************************************
 */
PZY_void arithmeticExpression() {

	if (lookahead.code == NUML_T ||
		lookahead.code == NUMF_T || lookahead.code == VNID_T ||
		lookahead.code == CHARL_T || lookahead.code == LPR_T) {
		statement();
	}
	if (lookahead.attribute.arithmeticOperator == OP_ADD) {
		matchToken(AO_T, OP_ADD);
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic Operator ADD parsed");
	}
	else if (lookahead.attribute.arithmeticOperator == OP_SUB) {
		matchToken(AO_T, OP_SUB);
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic Operator Subtract parsed");
	}
	else if (lookahead.attribute.arithmeticOperator == OP_MUL) {
		matchToken(AO_T, OP_MUL);
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic Operator Multiply parsed");
	}
	else if (lookahead.attribute.arithmeticOperator == OP_DIV) {
		matchToken(AO_T, OP_DIV);
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic Operator Divide parsed");
	}
	if (lookahead.code == NUML_T ||
		lookahead.code == NUMF_T || lookahead.code == VNID_T ||
		lookahead.code == CHARL_T || lookahead.code == LPR_T) {
		arithmeticExpression();
		return;
	}
	if (lookahead.code == RPR_T) matchToken(RPR_T, NO_ATTR);
	if (lookahead.code == EOS_T) matchToken(EOS_T, EOS_T);

	printf("%s%s\n", STR_LANGNAME, ": arithmeticExpression parsed");
}
/*
 ************************************************************
 * Logical Expression statement
* BNF: <logicalExpression> -> <VARIABLE/Literal> <logicalExpressionPrime>
 * <logicalExpressionPrime> -> <opt '|' | ^ | & >  <VARIABLE/Literal> <logicalExpressionPrime> | e
 *
 * FIRST(<logicalExpression>) = { e, FIRST(<opt '|' | ^ | & >),  FIRST(<VARIABLE/Literal>),
								  SL_T, NUMF_T, VNID_T, CHARL_T, KW(LPR_T),
								  KW(OP_AND), KW(OP_NOT), KW(OP_OR) }
 ***********************************************************
 */
PZY_void logicalExpression() {
	if (lookahead.code == SL_T || lookahead.code == NUML_T ||
		lookahead.code == NUMF_T || lookahead.code == VNID_T ||
		lookahead.code == CHARL_T || lookahead.code == LPR_T) {
		statement();
	}
	if (lookahead.attribute.logicalOperator == OP_AND) {
		matchToken(LO_T, OP_AND);
		printf("%s%s\n", STR_LANGNAME, ": Logical Operator AND parsed");
	}
	else if (lookahead.attribute.logicalOperator == OP_OR){
		matchToken(LO_T, OP_OR);
		printf("%s%s\n", STR_LANGNAME, ": Logical Operator OR parsed");
	}
	else if (lookahead.attribute.logicalOperator == OP_NOT){
		matchToken(LO_T, OP_NOT);
		printf("%s%s\n", STR_LANGNAME, ": Logical Operator NOT parsed");
	}
	if (lookahead.code == SL_T || lookahead.code == NUML_T ||
		lookahead.code == NUMF_T || lookahead.code == VNID_T ||
		lookahead.code == CHARL_T || lookahead.code == LPR_T) {
		logicalExpression();
		return;
	}
	if (lookahead.code == RPR_T) matchToken(RPR_T, NO_ATTR);
	if (lookahead.code == EOS_T) matchToken(EOS_T, EOS_T);
	printf("%s%s\n", STR_LANGNAME, ": logicalExpression parsed");
}


/*
************************************************************
*Format Specifier List statement
* BNF: < formatSpeciferList> -> <formatSpecifier><formatSpecifierPRIME> , | e
* <formatSpecifierPrime> ->  <formatSpecifier> <formatSpecifierPrime> | e
* 
* FIRST(<formatSpecifierList>) = { FID_T , KW(COMA_T) , e }
***********************************************************
*/
PZY_void formatSpecifierList() {
	if (lookahead.code == VNID_T || lookahead.code == RPR_T) return;
	formatSpecifier();

	printf("%s%s\n", STR_LANGNAME, ": formatSpecifierList parsed");
}

/*
************************************************************
* Format Specifier Statement
* BNF: < formatSpecifer> -> <formatSpecifier><formatSpecifierPRIME> | e
*
* FIRST(<formatSpecifierList>) = { FID_T , e }
***********************************************************
*/
PZY_void formatSpecifier() {
	if (lookahead.code != FID_T) return;
	statement();
	if (lookahead.code != COMA_T) {
		formatSpecifierList();
		return;
	}
	else matchToken(KW_T, OP_COMA);
	printf("%s%s\n", STR_LANGNAME, ": formatSpecifier Statement parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> POUT (<output statementPrime>);
 * <output Statement prime> -> <formatSpecifierList> <outputVariableList> <<CONSOLE>|<FILEW>> | e
 * 
 * FIRST(<output statement>) = { e, KW_T(POUT), KW_T(LPR_T) , KW_T(COMA_T),
								KW_T(CONSOLE), KW_T(FILEW), FIRST(<formatSpecifierList>), FID
								FIRST(<outputVariableList>) ,VNID, SL_T, NUML_T, NUMF_T,CHARL_T }
 ***********************************************************
 */
PZY_void outputStatement() {
	if (lookahead.code == LPR_T) matchToken(LPR_T, NO_ATTR);
	
	formatSpecifierList();
	while (lookahead.code != RPR_T && lookahead.attribute.codeType != CONSOLE && lookahead.attribute.codeType != FILEW) {
		if (lookahead.code == COMA_T) {
			matchToken(KW_T, OP_COMA);
		}
		outputVariableList();
		if (lookahead.code == RPR_T || lookahead.attribute.codeType == CONSOLE
			|| lookahead.attribute.codeType == FILEW) break;
		if (lookahead.code == EOS_T || lookahead.code==RBR_T) break;
	}
	if (lookahead.attribute.codeType == FILEW) statement();
	if (lookahead.attribute.codeType == CONSOLE) statement();
	if (lookahead.code == RBR_T) return;
	if (lookahead.code == RPR_T) matchToken(RPR_T, NO_ATTR);
	if (lookahead.code == EOS_T) matchToken(EOS_T, EOS_T);
	

	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variableList> <variable listPrime> , | e
 * <variableListPrime> -> , <variableList> <variablelistPrime> | e
 * 
 * FIRST(<opt_variable_list>) = { VNID_T, NUML_T, NUMF_T, SL_T, CHARL_T,
									KW(COMA_T) , e }
 ***********************************************************
 */
PZY_void outputVariableList() {
	switch (lookahead.code) {
	case KW_T:
	case SL_T:
	case NUML_T:
	case NUMF_T:
	case CHARL_T:
	case VNID_T:
		statement();
		break;
	default:
		//empty
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}
/*
************************************************************
* Input statement
* BNF:<inputStatement> -> RIN(<inputStatement>) ; 
* <inputStatement> -> <formatSpecifierList> <inputVariableList> <<KEYB> | <FILEW>> | e
* 
** FIRST(<input statement>) = { e, KW_T(RIN), KW_T(LPR_T) , KW_T(COMA_T),
								KW_T(KEYB), KW_T(FILEW), FIRST(<formatSpecifierList>), FID
								FIRST(<inputVariableList>) ,VNID, SL_T, NUML_T, NUMF_T,CHARL_T }
***********************************************************
*/
PZY_void inputStatement() {
	matchToken(LPR_T, NO_ATTR);
	formatSpecifierList();
	while (lookahead.code != RPR_T && lookahead.attribute.codeType != KEYB && lookahead.attribute.codeType != FILEW) {
		if (lookahead.code == COMA_T) {
			matchToken(KW_T, OP_COMA);
		}
		inputVariableList();
		if (lookahead.code == RPR_T || lookahead.attribute.codeType == KEYB
			|| lookahead.attribute.codeType == FILEW) break;
		if (lookahead.code == EOS_T || lookahead.code == RBR_T) break;

	}
	if (lookahead.attribute.codeType == KEYB) statement();
	if (lookahead.attribute.codeType == FILEW) statement();
	if (lookahead.code == RBR_T) return;
	if (lookahead.code == RPR_T) matchToken(RPR_T, NO_ATTR);
	if (lookahead.code == EOS_T) matchToken(EOS_T, EOS_T);

	printf("%s%s\n", STR_LANGNAME, ": input statement parsed");
}

/*
 ************************************************************
 * Input Variable List
 * BNF: <opt_variable list> -> <variableList> <variable listPrime> , | e
 * <variableListPrime> -> , <variableList> <variablelistPrime> | e
 *
 * FIRST(<opt_variable_list>) = { VNID_T, NUML_T, NUMF_T, SL_T, CHARL_T,
									KW(COMA_T) , e }
 ***********************************************************
 */
PZY_void inputVariableList() {
	switch (lookahead.code) {
	case KW_T:
	case SL_T:
	case NUML_T:
	case NUMF_T:
	case VNID_T:
		statement();
		break;
	default:
		//empty
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": input variable list parsed");
}

/*
************************************************************
* Conditional statement
* BNF: <condidtionalStatement> -> <ifStatement> | <IterativeStatement> 
* 
* 
* FIRST(<conditionalStatement>) = { KW(IF), KW(WHILE), KW(FOR), FIRST(<ifStatement>), 
									FIRST(<iterativeStatement>) } 
***********************************************************
*/
PZY_void conditionalStatement() {
	switch (lookahead.attribute.codeType) {
	case IF:
		ifStatement();
		break;
	case WHILE:
		matchToken(KW_T, WHILE);
		iterativeStatement();
		break;
	case FOR:
		iterativeStatement();
		break;
	default:
		return;
	}
	
	printf("%s%s\n", STR_LANGNAME, ": Conditional Statement parsed");
}

/*
************************************************************
* If-else statement
* BNF:<ifStatement> -> if ( <Condition EXPRESSION> ) { <sessionPrime/(W/O START)> }<elseifprime> 
* elseIfprime -> <opt else | else_if > ( <condition Expression > ) { <sessionPrime> } | <elsePrime>
* elsePrime -> { <sessionPrime> } | e
* <condition Expression> -> <optAssignmentExpression>
* 
* FIRST(<ifStatement>) = { KW(IF), KW(ELSE), KW(ELSEIF), KW(LPR_T), KW<LBR_T>, FIRST(<optAssignmentExpression>),
*							FIRST(<sessionPRime>), ... }
***********************************************************
*/
PZY_void ifStatement() {
	matchToken(KW_T, IF);
ELSEIFSTATE:
	if (lookahead.code == LPR_T) {
		matchToken(LPR_T, NO_ATTR);
	}
	optAssignmentExpression();
	if (lookahead.code == RPR_T) matchToken(RPR_T, NO_ATTR);

	if (lookahead.code == LBR_T) matchToken(LBR_T, NO_ATTR);

	if (lookahead.code == IF ||
		lookahead.code == FOR ||
		lookahead.code == WHILE) {
		conditionalStatement();
		if (lookahead.code == RBR_T) {
			matchToken(RBR_T, NO_ATTR);
			return;
		}
	}
	if (lookahead.code == RBR_T) {
		matchToken(RBR_T, NO_ATTR);
		return;
	}
	datatypeList();
	session();

	if (lookahead.code == RBR_T)
		matchToken(RBR_T, NO_ATTR);

	switch (lookahead.attribute.codeType) {
	case ELSE_IF:
		if (lookahead.attribute.codeType == ELSE_IF) {
			matchToken(KW_T, ELSE_IF);
			goto ELSEIFSTATE;
		}
		break;
	case ELSE:
		if (lookahead.attribute.codeType == ELSE) {
			matchToken(KW_T, ELSE);
			if (lookahead.code == LBR_T)
				matchToken(LBR_T, NO_ATTR);
			session();
		}
		if (lookahead.code == RBR_T)
			matchToken(RBR_T, NO_ATTR);
		break;
	default: break;
	}
	printf("%s%s\n", STR_LANGNAME, ": if Statement parsed");
}

/*
************************************************************
* Iterative Statement
* BNF: <Iterative Statement> -> <FOR LOOP> | <WHILE LOOP>
* <FOR LOOP> -> for ( <assignmentStatement> ; <relationalExpression> ; <arithmeticExpression> ) { <sessionPrime> }
* <WHILE LOOP> -> while ( <condition Expression> ) { <sessionPrime> }
* FIRST(<iterativeStatement>) = { KW(FOR), KW(WHILE)...,FIRST(<sessionPRIME>) }
***********************************************************
*/
PZY_void iterativeStatement() {
	if (lookahead.code == FOR) {
		matchToken(KW_T, FOR);
		matchToken(LPR_T, NO_ATTR);
		datatypeList();
		assignmentStatement();
		if (lookahead.code == EOS_T) {
			matchToken(EOS_T, EOS_T);
			relationalExpression();
		}
		if (lookahead.code == EOS_T) {
			matchToken(EOS_T, EOS_T);
			arithmeticExpression();
		}
	}
	else {
		matchToken(LPR_T, NO_ATTR);
		statement();
	}
	if (lookahead.code == RPR_T)
		matchToken(RPR_T, NO_ATTR);
	if (lookahead.code == LBR_T)
		matchToken(LBR_T, NO_ATTR);
	if (lookahead.code == IF ||
		lookahead.code == FOR ||
		lookahead.code == WHILE) {
		conditionalStatement();
		if (lookahead.code == RBR_T)
			matchToken(RBR_T, NO_ATTR);
		return;
	}
	session();
	if (lookahead.code == RBR_T)
		matchToken(RBR_T, NO_ATTR);

	printf("%s%s\n", STR_LANGNAME, ": Iterative Statement parsed");
}