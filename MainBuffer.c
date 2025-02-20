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
* Author: John Hoang
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer (A12)
* Function list: (...).
*************************************************************/

/*
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define directive does not have any effect on other compiler projects (gcc, Borland).
 */

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

 /* Check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */
PZY_void bErrorPrint(PZY_char* fmt, ...);
PZY_void displayBuffer(Buffer* ptr_Buffer);
PZY_long getFileSize(PZY_char* fname);
PZY_int isNumber(const PZY_char* ns);
PZY_void startBuffer(PZY_char*, PZY_char*, PZY_char, PZY_int, PZY_int);

/*
************************************************************
* Main function from Buffer
* Parameters:
*   argc / argv = Parameters from command prompt
* Return value:
*	Success operation.
************************************************************
*/

PZY_int mainBuffer(PZY_int argc, PZY_char** argv) {

	/* Create source input buffer */
	PZY_char* program = argv[0];
	PZY_char* input = argv[2];
	PZY_char mode = MODE_FIXED;
	//PZY_char mode = MODE_ADDIT;
	PZY_int isAnsiC = !ANSI_C;		/* ANSI C flag */
	PZY_int size = 0, increment = 0, wrongNumber = 0;
	//PZY_int size = 1000, increment = 900, wrongNumber = 0;

	/* Check if the compiler option is set to compile ANSI C */
	/* __DATE__, __TIME__, __LINE__, __FILE__, __STDC__ are predefined preprocessor macros*/
	/*
	if (isAnsiC) {
		bErrorPrint("Date: %s  Time: %s", __DATE__, __TIME__);
		bErrorPrint("ERROR: Compiler is not ANSI C compliant!\n");
		exit(EXIT_FAILURE);
	}
	*/

	/* Missing file name or/and mode parameter */
	if (argc <= 2) {
		bErrorPrint("\nDate: %s  Time: %s", __DATE__, __TIME__);
		bErrorPrint("\nRuntime error at line %d in file %s\n", __LINE__, __FILE__);
		bErrorPrint("%s\b\b\b\b%s%s", argv[0], ": ", "Missing parameters.");
		bErrorPrint("Usage: <Option=0> <SourceFile> [<Mode>]");
		exit(EXIT_FAILURE);
	}

	if (argc == 4) {
		mode = *argv[3];
		switch (mode) {
		case MODE_FIXED: case MODE_ADDIT: case MODE_MULTI: break;
		default:
			bErrorPrint("%s%s%c%s%c%s%c%s", program, ": Wrong mode - choose: ",
				MODE_FIXED, ", ", MODE_ADDIT, ", ", MODE_MULTI, ".");
			exit(EXIT_FAILURE);
		}
	}
	/* Read additional parameters, if any */
	if (argc == 6) {
		mode = *argv[3];
		if (isNumber(argv[4]))size = (PZY_short)atoi(argv[4]); else wrongNumber = 1;
		if (isNumber(argv[5]))increment = (PZY_short)atoi(argv[5]); else wrongNumber = 1;
		if (wrongNumber) {
			bErrorPrint("\nDate: %s  Time: %s", __DATE__, __TIME__);
			bErrorPrint("\nRuntime error at line %d in file %s\n", __LINE__, __FILE__);
			bErrorPrint("%s\b\b\b\b%s", argv[0], ": Missing or wrong number parameters.");
			bErrorPrint("Usage: <Option=0> <SourceFile> [<Mode> <Size> <Increment>]");
			exit(EXIT_FAILURE);
		}
	}



	startBuffer(program, input, mode, size, increment);

	/* Return success */
	printf("\nExiting Compiler");
	return (EXIT_SUCCESS);
}

/*
************************************************************
* Buffer starting method
* Params:
*	- Program: Name of the program
*	- Input: Filename
*	- Mode: Operational mode
*	- Size: Buffer capacity
*	- Increment: buffer increment.
************************************************************
*/
PZY_void startBuffer(PZY_char* program, PZY_char* input, PZY_char mode, PZY_int size, PZY_int increment) {

	BufferPointer bufferp;		/* pointer to Buffer structure */
	FILE* fileHandler;			/* input file handle */
	PZY_int loadSize = 0;			/* the size of the file loaded in the buffer */
	PZY_char symbol;				/* symbol read from input file */

	/* Create buffer */
	/* Casting char to increment to truncate? */
	bufferp = bCreate(size, (PZY_int)increment, mode);




	if (bufferp == NULL || !(bufferp->flags & PZY_CHK_EMP)) {
		bErrorPrint("%s%s", program,
			": Cannot allocate buffer - Use: buffer <input> <mode> <size> <increment>.");
		bErrorPrint("Filename: %s %c %d %d\n", input, mode, size, increment);
		printf("The value of the flags field is: 0x%02hX\n",
			bGetFlags(bufferp));
		exit(EXIT_FAILURE);
	}

	/* Open source file */
	if ((fileHandler = fopen(input, "r")) == NULL) {

		bErrorPrint("%s%s%s", program, ": Cannot open file: ", input);
		printf("The value of the flags field is: 0x%02hX\n",
			bGetFlags(bufferp));
		exit(EXIT_FAILURE);
	}

	/* Load source file into input buffer  */

	printf("Reading file %s ....Please wait\n", input);
	loadSize = bLoad(bufferp, fileHandler);

	/* file empty */
	if (loadSize == EMPTY_FILE) {
		printf("Does not seems to have data to read");
		loadSize = BUFFER_ERROR;
	}
	/* memory reallocation error */
	if ((bufferp->flags ^ CHK_FLAG) & (MEMORY_ERR_FLG)) {
		bErrorPrint("%s\n%s", program,
			": Cannot re-allocate new memory to buffer \n- Use: buffer <input> <mode> <size> <increment>.");
	}

	if ((bufferp->flags ^ CHK_FLAG) & (MEMORY_INC_ERR)) {
		bErrorPrint("%s\n%s", program,
			":Increment factor too large -BUFFER SIZE set to DEFAULT MAX - \nUse: buffer <input> <mode> <size> <increment>.");
		bErrorPrint("\nFilename: %s %c %d %d\n", input, mode, size, increment);
	}
	/* If the input file has not been completely loaded, find the file size and print the last symbol loaded */
	if (loadSize == BUFFER_ERROR) {
		printf("The input file %s %s\n", input, "has not been completely loaded.");
		printf("Current size of buffer: %d.\n", bGetSize(bufferp));
		symbol = (PZY_char)fgetc(fileHandler);
		printf("Last character read from the input file is: %c %d\n", symbol, symbol);
		printf("Input file size: %ld\n", getFileSize(input));
		printf("The value of the flags field is: 0x%02hX\n",
			bGetFlags(bufferp));
	}
	/*******************************************************************************************************************************************************/

	/* Close source file */
	fclose(fileHandler);

	/*
	 * Finishes the buffer: add end of file character (EOF) to the buffer display again
	 */
	if ((loadSize != BUFFER_ERROR) && (loadSize != 0)) {
		if (!bAddChar(bufferp, BUFFER_EOF)) {
			bErrorPrint("%s%s%s", program, ": ", "Error in compacting buffer.");
		}
	}
	displayBuffer(bufferp);
	/* Free the dynamic memory used by the buffer */
	bDestroy(bufferp);
	
}

/*
************************************************************
* Error printing function with variable number of arguments
* Params: Variable arguments, using formats from C language.
*	- Internal vars use list of arguments and types from stdarg.h
*   - NOTE: The format is using signature from C Language
************************************************************
*/

PZY_void bErrorPrint(PZY_char* fmt, ...) {
	/* Initialize variable list */
	va_list ap;
	va_start(ap, fmt);

	(PZY_void)vfprintf(stderr, fmt, ap);
	va_end(ap);

	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}

/*
************************************************************
* Print function
*	- Params: buffer to print all properties.
************************************************************
*/

PZY_void displayBuffer(Buffer* ptr_Buffer) {
	printf("\nPrinting buffer parameters:\n\n");

	printf("The capacity of the buffer is:  %d\n",
		bGetSize(ptr_Buffer));
	printf("The current size of the buffer is:  %d\n",
		bGetWritePos(ptr_Buffer));
	printf("The operational mode of the buffer is: %c\n",
		bGetMode(ptr_Buffer));
	printf("The increment factor of the buffer is:  %lu\n",
		bGetIncrement(ptr_Buffer));
	printf("The first symbol in the buffer is:  %c\n",
		bGetWritePos(ptr_Buffer) ? *bGetContent(ptr_Buffer, 0) : ' ');
	printf("The value of the flags field is: 0x%02hX\n",
		bGetFlags(ptr_Buffer));
	printf("\nPrinting buffer contents:\n\n");
	bRecover(ptr_Buffer);
	if (!bPrint(ptr_Buffer))
		printf("\nEmpty buffer\n");
}

/*
************************************************************
* Get buffer size
* Params:
*	- Filename: Name of the file
************************************************************
*/

PZY_long getFileSize(PZY_char* fname) {
	FILE* input;
	PZY_long flength;
	input = fopen(fname, "r");
	if (input == NULL) {
		bErrorPrint("%s%s", "Cannot open file: ", fname);
		return 0;
	}
	fseek(input, 0L, SEEK_END);
	flength = ftell(input);
	fclose(input);
	return flength;
}

/*
************************************************************
 * Tests for decimal-digit character string
 * Params:
 *		- String to be evaluated as numeric
 * Return:
 *		- Number value: Returns nonzero (true) if ns is a number; 0 (False) otherwise
************************************************************
*/

PZY_int isNumber(const PZY_char* ns) {
	PZY_char c; PZY_int i = 0;
	if (ns == NULL) return 0;
	while ((c = ns[i++]) == 0) {
		if (!isdigit(c)) return 0;
	}
	return 1;
}
