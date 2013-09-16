/***************************************************************************
 *   Copyright (C) 2005 by Alejandro Atienza Ramos			   *
 *   atienzar256@yahoo.es						   *
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


#include "dstr.h"

int main(int argc, char *argv[])
{
	/* Memory declarations */
	char buffer[BUFLEN];
	int x, y, d, flag;
	inputQueue iq;
	gap sudoku[81];

	/* Memory initialization */
	initSudoku(sudoku);
	flag = 1;
	initInput(&iq);

	/* Data adquisition */
	do{
		fgets(buffer, BUFLEN, stdin);
		if(EOF == sscanf(buffer, "%d %d %d", &x, &y, &d) || !(x%10) || !(y%10) || !(d%10))
			flag = 0;
		else
			addInput(&iq, x%10, y%10, d%10);
	}while(flag);

	/* Data processing */
	while(isNotEmpty(&iq)){
		while(isNotEmpty(&iq)){
			while(isNotEmpty(&iq))
				/* First level guessing: Remaining posibilities for a gap */
				consumeInput(sudoku, &iq);
			/* Second level guessing: Remaining posibilities for a number */
			checkNumbers(sudoku, &iq);
		}
		/* Third level guessing: Treating a permutable group of numbers/gaps as a whole */
		//permutations(sudoku, &iq);
	}

	/* Results */
	printSudoku(sudoku);
	checkSudoku(sudoku);

  return EXIT_SUCCESS;
}

void initSudoku(gap* sudoku){
	/* Initialization of the sudoku data structure */

	int i,j,k;

	for(i=0;i<9;i++)
		for(j=0;j<9;j++){
			sudoku[i+9*j].number = 0;
			sudoku[i+9*j].posib[0] = 9;
			for(k=1;k<10;k++)
				sudoku[i+9*j].posib[k] = 1;
			}
	return;
}

void initInput(inputQueue * q){
	/* Initialization of the input queue */
	if(!q)
		return;
	q->head=q->tail=NULL;
	return;
}

int isNotEmpty(inputQueue * q){
	if(q && q->head)
		return 1;
	return 0;
}

int addInput(inputQueue * q, unsigned short int x, unsigned short int y, unsigned short int data){
	/* If a number has been guessed, it is added to the queue of inputs */

	input *ptr = malloc(sizeof(input));

	if(!ptr)
		return ERR;
	ptr->x = x;
	ptr->y = y;
	ptr->data = data;

	if(q->tail && q->head){
		ptr->next = NULL;
		q->tail->next = ptr;
		q->tail = ptr;
	}else{
		ptr->next = NULL;
		q->tail = q->head = ptr;
	}
	return OK;
}

void consumeInput(gap * sudoku, inputQueue * q){
	/* This function consumes an input from the queue and changes the sudoku, reducing permutation posibilities */

	unsigned short int x, y, i, data;
	input * ptr = q->head;
	q->head = ptr->next;
	x = ptr->x;
	y = ptr->y;
	data = ptr->data;
	free(ptr);
	if(!q->head)
		q->tail=NULL;

	if(DEBUG)
		fprintf(stdout, "%d %d %d:\n", x, y, data);
	x--;y--;
	sudoku[x+9*y].number = data;
	for(i=0;i<10;i++)
		sudoku[x+9*y].posib[i]=0;
	eliminateFromCol(sudoku, q, x, y, data);
	eliminateFromRow(sudoku, q, x, y, data);
	eliminateFromSquare(sudoku, q, x, y, data);
	if(DEBUG)
		printSudoku(sudoku);

	return;
}

/* The following functions are used to reduce posibilities of numbers in the sudoku */

void eliminateFromCol(gap * sudoku, inputQueue *iq, unsigned short int column, unsigned short int y, unsigned short int number){
	/* Eliminates from the column the given number */

	unsigned short int i, j;

	for(j=0;j<9;j++)
		if(j!=y && sudoku[column+9*j].posib[number]){
			sudoku[column+9*j].posib[number] = 0;
			if(1 == --sudoku[column+9*j].posib[0]){
				for(i=1;!sudoku[column+9*j].posib[i];i++)
					;
				if(DEBUG)
					printf("En la jugada %d%d%d la funcion %s ha puesto %d%d%d\n", column+1, y+1, number, "col", column+1, j+1, i);
				addInput(iq, column+1, j+1, i);
			}
		}
	return;
}
void eliminateFromRow(gap * sudoku, inputQueue *iq, unsigned short int x, unsigned short int row, unsigned short int number){
	/* Eliminates from the row the given number */

	unsigned short int i, j;

	for(i=0;i<9;i++)
		if(i!=x && sudoku[i+9*row].posib[number]){
			sudoku[i+9*row].posib[number] = 0;
			if(1 == --sudoku[i+9*row].posib[0]){
				for(j=1;!sudoku[i+9*row].posib[j];j++)
					;
				if(DEBUG)
					printf("En la jugada %d%d%d la funcion %s ha puesto %d%d%d\n", x+1, row+1, number, "row", i+1, row+1, j);
				addInput(iq, i+1, row+1, j);
			}
		}
	return;
}
void eliminateFromSquare(gap * sudoku, inputQueue *iq, unsigned short int x, unsigned short int y, unsigned short int number){
	/* Eliminates from the square the given number */

	unsigned short int i,j,k,l,m;
	
	for(j=0;j<3;j++)
		for(i=0;i<3;i++){
			k = (x/3)*3+i;
			l = (y/3)*3+j;
			if( (k != x || l != y) && sudoku[k+9*l].posib[number]){
				sudoku[k+9*l].posib[number] = 0;
				if(1 == --sudoku[k+9*l].posib[0]){
					for(m=1;!sudoku[k+9*l].posib[m];m++)
						;
					if(DEBUG)
						printf("En la jugada %d%d%d la funcion %s ha puesto %d%d%d\n", x+1, y+1, number, "sqr", k+1, l+1, m);
					addInput(iq, k+1, l+1, m);
				}
			}
		}
	return;
}



void printSudoku(gap * sudoku){
	/* Prints the sudoku */
	int i, j, k;

	for(j=0;j<9;j++){
		for(i=0;i<9;i++)
			if(DEBUG){
				for(k=0;k<10;k++)
					fprintf(stdout, "%d", sudoku[i+9*j].posib[k]);
				fprintf(stdout, " ");
			}else{
				fprintf(stdout, "%d ", sudoku[i+9*j].number);
			}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
	return;
}

void checkSudoku(gap * sudoku){
	/* Checks the result */
	int vector[10];
	int i, j, k, l, flag;

	/* Row */
	for(j=0;j<9;j++){
		flag=0;
		for(i=0;i<10;i++)
			vector[i]=0;
		vector[0]++;
		for(i=0;i<9;i++)
			vector[sudoku[i+9*j].number]++;
		for(i=0;i<10;i++)
			if(vector[i] != 1)
				flag++;
		if(flag)
			fprintf(stderr, "Error en la fila %d\n", j);
	}

	/* Column */
	for(i=0;i<9;i++){
		flag=0;
		for(j=0;j<10;j++)
			vector[j]=0;
		vector[0]++;
		for(j=0;j<9;j++)
			vector[sudoku[i+9*j].number]++;
		for(j=0;j<10;j++)
			if(vector[j] != 1)
				flag++;
		if(flag)
			fprintf(stderr, "Error en la columna %d\n", i);
	}
	/* Square */
	for(i=0;i<9;i+=3)for(j=0;j<9;j+=3){
		flag=0;
		for(k=0;k<10;k++)
			vector[k]=0;
		vector[0]++;
		for(k=0;k<3;k++)
			for(l=0;l<3;l++)
				vector[sudoku[(i+k)+9*(j+l)].number]++;
		for(k=0;k<10;k++)
			if(vector[k] != 1)
				flag++;
		if(flag)
			fprintf(stderr, "Error en el cuadrado %d,%d\n", i/3,j/3);
	}
	return;
}

void checkNumbers(gap * sudoku, inputQueue * iq){
	/* Checks for numbers with only one gap posibility. (Earlier functions searched for gaps with only one number posibility) */
	input vector[10];
	int i, j, k, l, q;

	/* Row iteration */
	for(j=0;j<9;j++){
		for(i=0;i<10;i++){
			vector[i].x = vector[i].y = -1;
			vector[i].data = 0;
		}
		for(i=0;i<9;i++)
			for(q=1;q<10;q++)
				if(sudoku[i+9*j].posib[q]){
					vector[q].data++;
					vector[q].x = i+1;
					vector[q].y = j+1;
				}
		for(i=1;i<10;i++)
			if(vector[i].data == 1){
				if(DEBUG)
					printf("La parte %s de la funcion checkNumbers ha puesto %d%d%d\n", "row", vector[i].x, vector[i].y, i);
				addInput(iq, vector[i].x, vector[i].y, i);
			}
	}

	/* Column iteration */
	for(i=0;i<9;i++){
		for(j=0;j<10;j++){
			vector[j].x = vector[j].y = -1;
			vector[j].data = 0;
		}
		for(j=0;j<9;j++)
			for(q=1;q<10;q++)
				if(sudoku[i+9*j].posib[q]){
					vector[q].data++;
					vector[q].x = i+1;
					vector[q].y = j+1;
				}
		for(j=1;j<10;j++)
			if(vector[j].data == 1){
				if(DEBUG)
					printf("La parte %s de la funcion checkNumbers ha puesto %d%d%d\n", "col", vector[j].x, vector[j].y, j);
				addInput(iq, vector[j].x, vector[j].y, j);
			}
	}

	/* Square iteration */
	for(i=0;i<9;i+=3)for(j=0;j<9;j+=3){
		for(k=0;k<10;k++){
			vector[k].x = vector[k].y = -1;
			vector[k].data = 0;
		}
		for(k=0;k<3;k++)
			for(l=0;l<3;l++){
				i+=k;j+=l;
				for(q=1;q<10;q++)
					if(sudoku[i+9*j].posib[q]){
						vector[q].data++;
						vector[q].x = i+1;
						vector[q].y = j+1;
					}
				i-=k;j-=l;
			}
		for(k=1;k<10;k++)
			if(vector[k].data == 1){
				if(DEBUG)
					printf("La parte %s de la funcion checkNumbers ha puesto %d%d%d\n", "sqr", vector[k].x, vector[k].y, k);
				addInput(iq, vector[k].x, vector[k].y, k);
			}
	}

	/* End of function */
	return;
}

void permutations(gap * sudoku, inputQueue * iq){
	/* This function detects not fixed numbers that are already restricted to one square and one column or row */
	int i, j, n, flag;
	int v[3];

	/* Column search */
	for(i=0;i<9;i++)
		for(n=1;n<10;n++){
			v[0]=v[1]=v[2]=0;
			flag=1;
			for(j=0;flag&&j<9;j++)
				if(sudoku[i+9*j].number == n)
					flag = 0;
				else if(sudoku[i+9*j].posib[n])
					v[j/3]=1;
			if(flag && v[0]+v[1]+v[2]==1)
				for(j=0;j<9;j++)
					if(!v[j/3] && sudoku[i+9*j].posib[n]){
						sudoku[i+9*j].posib[n]=0;
						if(1 == --sudoku[i+9*j].posib[0]){
							for(n=1;!sudoku[i+9*j].posib[n];n++)
								;
							if(DEBUG)
								printf("La funcion permutations ha puesto %d%d%d\n", i, j, n);
							addInput(iq, i, j, n);
						}
					}
		}

	/* Row search */
	for(j=0;j<9;j++)
		for(n=1;n<10;n++){
			v[0]=v[1]=v[2]=0;
			flag=1;
			for(i=0;flag&&i<9;i++)
				if(sudoku[i+9*j].number == n)
					flag = 0;
				else if(sudoku[i+9*j].posib[n])
					v[i/3]=1;
			if(flag && v[0]+v[1]+v[2]==1)
				for(i=0;i<9;i++)
					if(!v[i/3] && sudoku[i+9*j].posib[n]){
						sudoku[i+9*j].posib[n]=0;
						if(1 == --sudoku[i+9*j].posib[0]){
							for(n=1;!sudoku[i+9*j].posib[n];n++)
								;
							if(DEBUG)
								printf("La funcion permutations ha puesto %d%d%d\n", i, j, n);
							addInput(iq, i, j, n);
						}
					}
		}


	return;
}

/*
Third level guessing:

If inside a square a number is restricted to a column or row, those restrictions should be reflected in the sudoku. The same way, if in a column or a row a number is restricted to a specific square, it should be reflected too. Obviously, in a row every number is restricted to a column, and viceversa, but that is already reflected in the written code.

A fourth level would involve try-and-error methods.

Alejandro Atienza Ramos
*/
