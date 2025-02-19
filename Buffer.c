/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: John Hoang
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
***********************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2022
* Author: John Hoang
* Course: CST 8152 – Compilers, Lab Section: [013]
* Assignment: A12.
* Date: Jan 01 2022
* Professor: Paulo Sousa / Abdullah Kadri
* Purpose: This file is the main code for Buffer (A12)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

/*
***********************************************************
* Function name: bCreate
* Purpose: Creates the buffer according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: John Hoang
* History/Versions: Ver 2.1
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to Buffer)
* Algorithm: Allocation of memory according to inicial (default) values.
* TO_DO:
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer bCreate(PZY_int size, PZY_int increment, PZY_int mode) {
	BufferPointer b ;//= (BufferPointer)calloc(0, sizeof(Buffer))

	/* TO_DO: Defensive programming */
	if (size<0 || size>PZY_MAX_SIZE)
		return NULL;
	if (!size) {
		size = BUFFER_DEFAULT_SIZE;
		increment = BUFFER_DEFAULT_INCREMENT;
	}
	if (!increment)
		mode = MODE_FIXED;
	b = (BufferPointer)calloc(1, sizeof(Buffer));
	/* TO_DO: Defensive programming */
	if (b == NULL)
		return NULL;
	b->string = (charz)malloc(size);
	/* TO_DO: Defensive programming */
	if (!b->string) {
		free(b);
		return NULL;
	}

	b->mode = mode;
	b->size = size;
	b->increment = increment;
	/* TO_DO: Initialize flags */
	b->flags = PZY_DEFAULT_FLAG;
	/* TO_DO: The created flag must be signalized as EMP */
	b->flags = b->flags | PZY_EMP;

	return b;
}


/*
***********************************************************
* Function name: bAddChar
* Purpose: Adds a char to buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   ch = char to be added
* Return value:
*	bPointer (pointer to Buffer)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer bAddChar(BufferPointer const pBuffer, PZY_char ch) {
	charz tempbuf;
	PZY_int newSize;
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return NULL;
	/* TO_DO: Reset Realocation */
	pBuffer->flags = pBuffer->flags & PZY_RST_RL;
	/* TO_DO: Test the inclusion of chars */
	if (pBuffer->position.writePos * (PZY_int)sizeof(PZY_char) < pBuffer->size) {
		/* TO_DO: This buffer is NOT full */
		pBuffer->flags = pBuffer->flags & PZY_RST_FUL;
	}
	else {
		/* TO_DO: Reset Full flag */
		pBuffer->flags = pBuffer->flags | PZY_FUL;
		switch (pBuffer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			newSize = pBuffer->size + pBuffer->increment;
			/* TO_DO: Defensive programming */
			if (newSize < 0 || newSize <= pBuffer->size) {
				return NULL;
			}
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		case MODE_MULTI:
			newSize = pBuffer->size * pBuffer->increment;
			/* TO_DO: Defensive programming */
			if (newSize < 0 || newSize <= pBuffer->size) {
				return NULL;
			}
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		default:
			return NULL;
		}
		tempbuf = (charz)realloc(pBuffer->string, newSize);
		/* TO_DO: Defensive programming */
		if (!tempbuf)
			return NULL;
		if (tempbuf != pBuffer->string) {
			/* TO_DO: Set Relocation */
			pBuffer->flags = pBuffer->flags | PZY_RL;
			pBuffer->string = tempbuf;
		}
		pBuffer->size = newSize;
	}
	/* TO_DO: Add the char */
	pBuffer->flags = pBuffer->flags & PZY_RST_EMP;
	pBuffer->string[pBuffer->position.writePos++] = ch;
	return pBuffer;
}
/*
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

/*
***********************************************************
* Function name: bClear
* Purpose: Clears the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BOL bClear(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return PZY_FALSE;
	pBuffer->position.writePos = pBuffer->position.markPos = pBuffer->position.readPos = 0;
	/* TO_DO: Adjust flags original */
	pBuffer->flags = PZY_DEFAULT_FLAG;
	return PZY_TRUE;
}

/*
***********************************************************
* Function name: bDestroy
* Purpose: Releases the buffer address
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BOL bDestroy(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	/* BufferPointer is a constant??? cant free , freeing at main buffer will
	free memory of all structur*/
	/* TO_DO: Clear buffer data */
	/* if (!pBuffer)
		return PZY_FALSE;
	charz tmp;
	
	tmp =(charz) realloc((*pBuffer).string, BUFFER_DEFAULT_SIZE);
	if (!tmp)
		return PZY_FALSE;
	if (tmp != pBuffer->string) {
		pBuffer->string = tmp;
	}
	//free(tmp);
	tmp = NULL;


	if (!pBuffer->string) return PZY_FALSE;

	bClear(pBuffer);
	pBuffer->flags = 0x00;
	pBuffer->size = 0;
	pBuffer->increment = 0;
	pBuffer->mode = 0;*/
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return PZY_FALSE;
	free(pBuffer->string);
	free(pBuffer);

	return PZY_TRUE;
}

/*
***********************************************************
* Function name: bIsFull
* Purpose: Checks if buffer is full
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BOL bIsFull(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return PZY_FALSE;
	/* TO_DO: Check flag if buffer is FUL */
	if (pBuffer->flags & PZY_CHK_FUL)
		return PZY_TRUE;

	return PZY_FALSE;
}

/*
***********************************************************
* Function name: bGetWritePos
* Purpose: Returns the position of char to be added
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	addcPosition value
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_int bGetWritePos(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return BUFFER_ERROR;
	if (pBuffer->position.writePos > pBuffer->size) return 0;

	return pBuffer->position.writePos;
}

/*
***********************************************************
* Function name: bGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_int bGetSize(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return BUFFER_ERROR;

	return pBuffer->size;
}

/*
***********************************************************
* Function name: bGetMode
* Purpose: Returns the operational mode
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_int bGetMode(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return BUFFER_ERROR;
	if (pBuffer->mode <= 0)
		pBuffer->mode = MODE_FIXED;

	return pBuffer->mode;
}


/*
***********************************************************
* Function name: bGetMarkPos
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	mark offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_int bGetMarkPos(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return BUFFER_ERROR;
	/* search for the mark/symbol
	A mark is a location in the buffer, which indicates the position
	of a specific character (for example, the beginning of a
	word or a phrase)*/
	if (!pBuffer)
		return BUFFER_ERROR;

	return pBuffer->position.markPos;
}


/*
***********************************************************
* Function name: bSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BOL bSetMark(BufferPointer const pBuffer, PZY_int mark) {
	/* TO_DO: Defensive programming */
	if (!pBuffer || mark<0 || mark > pBuffer->position.writePos) return PZY_FALSE;
	

	/* TO_DO: Adjust the mark */
	pBuffer->position.markPos = mark;
	return PZY_TRUE;
}

/*
***********************************************************
* Function name: bPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_int bPrint(BufferPointer const pBuffer) {
	PZY_int cont = 0;
	PZY_char c;
	/* TO_DO: Defensive programming */
	if (!pBuffer || !(pBuffer->string))
		return BUFFER_ERROR;
	c = bGetChar(pBuffer);
	/* TO_DO: Check flag if buffer EOB has achieved */
	while (!(pBuffer->flags & PZY_CHK_EOB)) {
		cont++;
		printf("%c", c);
		c = bGetChar(pBuffer);
	}
	return cont;
}

/*
***********************************************************
* Function name: bLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   fi = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_int bLoad(BufferPointer const pBuffer, FILE* const fi) {
	PZY_int size = 0;
	PZY_char c;
	/* TO_DO: Defensive programming */
	if (!fi || !pBuffer)
		return BUFFER_ERROR;
	c = (PZY_char)fgetc(fi);
	while (!feof(fi)) {
		if (!bAddChar(pBuffer, c)) {
			ungetc(c, fi);
			return BUFFER_ERROR;
		}
		c = (char)fgetc(fi);
		size++;
	}
	if (ferror(fi))
		return BUFFER_ERROR;
	/* TO_DO: Defensive programming */

	pBuffer->position.readPos = 0;
	/* undo the empty flag */
	if (bIsEmpty(pBuffer)) pBuffer->flags ^= PZY_CHK_EMP;

	return size;
}

/*
***********************************************************
* Function name: bIsEmpty
* Purpose: Checks if buffer is empty.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BOL bIsEmpty(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return PZY_FALSE;

	/* TO_DO: Check if it is empty */

	if (pBuffer->flags & PZY_CHK_EMP)
		return PZY_TRUE;

	return PZY_FALSE;
}

/*
***********************************************************
* Function name: bGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_char bGetChar(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (!pBuffer)
		return BUFFER_ERROR;
	if (pBuffer->position.readPos == pBuffer->position.writePos) {
		/* TO_DO: Set EOB flag */
		pBuffer->flags = pBuffer->flags | PZY_EOB;
		return BUFFER_EOF;
	}
	/* TO_DO: Reset EOB flag */
	pBuffer->flags = pBuffer->flags & PZY_RST_EOB;
	return pBuffer->string[pBuffer->position.readPos++];
}


/*
***********************************************************
* Function name: bRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BOL bRecover(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return PZY_FALSE;
	pBuffer->position.readPos = 0;
	pBuffer->position.markPos = 0;
	return PZY_TRUE;
}


/*
***********************************************************
* Function name: bRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BOL bRetract(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return PZY_FALSE;
	if (!pBuffer || pBuffer->position.readPos == 0)
		return PZY_FALSE;
	pBuffer->position.readPos--;
	return PZY_TRUE;
}


/*
***********************************************************
* Function name: bRestore
* Purpose: Resets the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
BOL bRestore(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return PZY_FALSE;
	pBuffer->position.readPos = pBuffer->position.markPos;
	return PZY_TRUE;
}


/*
***********************************************************
* Function name: bGetReadPos
* Purpose: Returns the value of getCPosition.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The readPos offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_int bGetReadPos(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return BUFFER_ERROR;
	if (pBuffer->position.readPos > pBuffer->position.writePos) return BUFFER_ERROR;

	return pBuffer->position.readPos;
}


/*
***********************************************************
* Function name: bGetIncrement
* Purpose: Returns the buffer increment.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_int bGetIncrement(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return BUFFER_ERROR;


	return pBuffer->increment;
}


/*
***********************************************************
* Function name: bGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
PZY_char* bGetContent(BufferPointer const pBuffer, PZY_int pos) {
	/* TO_DO: Defensive programming */
	if (!pBuffer || pos < 0 || pos > pBuffer->position.writePos)
		return NULL;

	return pBuffer->string + pos;
}


/*
***********************************************************
* Function name: bGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
FLG bGetFlags(BufferPointer const pBuffer) {
	/* TO_DO: Defensive programming */
	if (pBuffer == NULL) return PZY_FALSE;

	return pBuffer->flags;
}
#else
#define bGetFlags(pBuffer) ((pBuffer)?(pBuffer->flags):(RT_FAIL_1))
#endif
