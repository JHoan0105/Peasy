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
* File name: compilers.h
* Compiler: MS Visual Studio 2022
* Author: John Hoang
* Course: CST 8152 – Compilers, Lab Section: [013]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa, Abdulla
* Purpose: This file defines the functions called by main function.
* Function list: mainBuffer(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Logical constants - adapt for your language */
#define PZY_TRUE	1
#define PZY_FALSE	0

/* You should add your own constant definitions here */
#define PZY_MAX_SIZE				SHRT_MAX-1	/* maximum capacity */


/*
------------------------------------------------------------
Data types definitions
------------------------------------------------------------
*/
/* TO_DO: Define your typedefs
**/
typedef char			PZY_char;
typedef short			PZY_int;
typedef float			PZY_float;
typedef long			PZY_long;
typedef double			PZY_double;
typedef short			PZY_short;

typedef void			PZY_void;

typedef unsigned char	BOL;
typedef unsigned char	FLG;

typedef unsigned int	PZY_uint;
typedef unsigned char	PZY_uchar;
typedef unsigned long	PZY_ulong;
typedef PZY_char* charz;



/*
------------------------------------------------------------
Programs:
1: Buffer - invokes MainBuffer code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_BUFFER = '1',
	PGM_SCANNER = '2',
	PGM_PARSER = '3'
};

/*
------------------------------------------------------------
Main functions signatures
(Codes will be updated during next assignments)
------------------------------------------------------------
*/
PZY_int mainBuffer(PZY_int argc, PZY_char** argv);
PZY_int mainScanner	(PZY_int argc, PZY_char** argv);

/*
PZY_int mainParser	(PZY_int argc, PZY_chr** argv);
*/
#endif
