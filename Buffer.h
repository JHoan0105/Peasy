/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: John Hoang
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
************************************************************
* File name: Buffer.h
* Compiler: MS Visual Studio 2022
* Author: John Hoang
* Course: CST 8152 – Compilers, Lab Section: [013]
* Assignment: A12.
* Date: Jan 01 2022
* Professor: Paulo Sousa, Abdullah Kadri
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer) */
enum BUFFERMODES {
	MODE_FIXED = 'f',
	MODE_ADDIT = 'a',
	MODE_MULTI = 'm'
};

/* TO_DO: Adjust all constants to your Language */

#define BUFFER_ERROR				(-1)		/* General error message */
#define FILE_ERROR					-1			/* File Error message*/
#define BUFFER_EOF					-1		/* General EOF */
#define EMPTY_FILE					0		/* file empty */	
#define BUFFER_MEM_ERR				0		/* bLoad will return error if addchar return 0 */


/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (Peasy) .................................. */

#define BUFFER_DEFAULT_SIZE			200			/* default initial buffer capacity and min buffer size */
#define BUFFER_DEFAULT_INCREMENT	100			/* default increment factor and min increment factor*/
#define BUFFER_MAX_SIZE				PZY_MAX_SIZE		/* Max buffer size*/
#define BUFFER_MAX_INC				1000		/* Max increment*/

/* Add your bit-masks constant definitions here - Defined for Peasy*/
/* BITS                                (7654.3210) */

/* instead of RST i use exclusive ^ on checker */
#define PZY_DEFAULT_FLAG			0x0F 	/* (0000.1111)_2 = (015)_10 */
#define PZY_RL						0x80	/* flag when memory is allocated/reallocated otherwise reset*/
#define PZY_CHK_RL					0x80	/* check allocation flag */
#define PZY_RST_RL					0x7F	/* check allocation flag */

#define PZY_EOB						0x40	/* End of Buffer flag */
#define PZY_CHK_EOB					0x40	/* check allocation flag */
#define PZY_RST_EOB					0xBF	/* CHECK End of Buffer flag */

#define PZY_FUL						0x20	/* Buffer Full flag */
#define PZY_CHK_FUL					0xDF	/* CHECK Buffer Full flag */
#define PZY_RST_FUL					0x20	/* CHECK Buffer Full flag */

#define PZY_EMP						0x10	/* Buffer empty flag*/
#define PZY_CHK_EMP					0xEF	/* CHECK Buffer empty flag*/
#define PZY_RST_EMP					0x10	/* CHECK Buffer empty flag*/

#define MEMORY_ERR_FLG				0x01	/* Null return on malloc/realloc. Memory Leak*/
#define MEMORY_INC_ERR				0x02	/* Change increment size to manage buffer size */
#define MEMORY_CHK					0xF0
#define CHK_FLAG					0xFF

/* TO_DO: BIT 7: REL = Relocation */
/* TO_DO: BIT 6: EOB = EndOfBuffer */
/* TO_DO: BIT 5: FUL = Full */
/* TO_DO: BIT 4: EMP = Empty */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TO_DO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	PZY_int markPos;			/* the offset (in chars) to the mark location */
	PZY_int readPos;			/* the offset (in chars) to the get-character location */
	PZY_int writePos;			/* the offset (in chars) to the add-character location */
} Position;

/* Buffer structure */
typedef struct buffer {
	PZY_char* string;			/* pointer to the beginning of character array (character buffer) */
	PZY_int	size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	PZY_int	increment;			/* character array increment factor */
	PZY_int	mode;				/* operational mode indicator */
	FLG	flags;					/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;		/* Offset / position field */


} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* TO_DO: Define your function declarations using your language specification */

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	bCreate(PZY_int, PZY_int, PZY_int);
BufferPointer	bAddChar(BufferPointer const, PZY_char);
BOL		bRetract(BufferPointer const);
BOL		bRestore(BufferPointer const);
BOL		bRecover(BufferPointer const);
BOL		bClear(BufferPointer const);
BOL		bDestroy(BufferPointer const);
BOL		bSetMark(BufferPointer const, PZY_int);
BOL		bIsEmpty(BufferPointer const);
BOL		bIsFull(BufferPointer const);
BOL		bIsRealocated(BufferPointer const);
PZY_int		bPrint(BufferPointer const);
PZY_int		bLoad(BufferPointer const, FILE* const);
/* Getters */
PZY_int		bGetSize(BufferPointer const);
PZY_int		bGetWritePos(BufferPointer const);
PZY_int		bGetMarkPos(BufferPointer const);
PZY_int		bGetReadPos(BufferPointer const);
PZY_int		bGetIncrement(BufferPointer const);
PZY_int		bGetMode(BufferPointer const);
PZY_char		bGetChar(BufferPointer const);
PZY_char* bGetContent(BufferPointer const, PZY_int);
FLG		bGetFlags(BufferPointer const);


#endif
