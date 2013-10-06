/***************************************************************************
 *   Copyright (C) 2005 by AlexAti (github.com user)                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
 #define DEBUG 1
#else
 #ifdef RELEASE
  #define DEBUG 0
 #else
  #define DEBUG 0
 #endif
#endif

/* Data strucuture */

typedef struct _gap{
	unsigned short int number;
	/* posib[0] says how many posib flags are high (1) from the following nine [1-9]*/
	/* If posib[0] <= 1 then the number can be guessed. */
	unsigned short int posib[10];
}gap;

typedef struct _input{
	unsigned short x,y,data;
	struct _input * next;
}input;

typedef struct _inputQueue{
	input * head;
	input * tail;
}inputQueue;

enum constants{
	BUFLEN = 64,
	OK = 0,
	ERR = -1
};

/* Function declarations */

	/* Sudoku initialization */
	void initSudoku(gap * sudoku);
	
	/* Input queue control functions */
	void initInput(inputQueue * q);
	int isNotEmpty(inputQueue * iq);
	int addInput(inputQueue * q, unsigned short int x, unsigned short int y, unsigned short int data);
	void consumeInput(gap * sudoku, inputQueue * q);
	
	/* posibility reduction fuctions */
	void eliminateFromCol(gap * sudoku, inputQueue *iq, unsigned short int column, unsigned short int y, unsigned short int number);
	void eliminateFromRow(gap * sudoku, inputQueue *iq, unsigned short int x, unsigned short int row, unsigned short int number);
	void eliminateFromSquare(gap * sudoku, inputQueue *iq, unsigned short int x, unsigned short int y, unsigned short int number);
	void checkNumbers(gap * sudoku, inputQueue * iq);
	void permutations(gap * sudoku, inputQueue * iq);

	/* Result print function */
	void printSudoku(gap * sudoku);
	void checkSudoku(gap * sudoku);
