/*
 * main.c
 *
 *  Created on: Jul 8, 2014
 *      Author: cameron
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	int value;
	int *potential;
} cell;

typedef struct {
	int solvedCells;
	int totalCells;
	int numChanges;
	int size;
	int sectionSize;
	cell **boardCells;

} board;


board *loadPuzzle(char *filename);
void freeBoard(board *bd);
void printPuzzle(board *bd);
void printPuzzleSolve(board *bd);
void testSquares(board *bd);
void testRows(board *bd);
void testColumns(board *bd);
void testSquare(board *bd, int row, int col);
void testRow(board *bd, int row);
void testCol(board *bd, int col);
void evaluateCell(cell *cell, board *bd);

int main(int argc, char **argv) {
	board *puzzle;
	puzzle = loadPuzzle(argv[1]);
	printPuzzle(puzzle);
	int numFailedLoops = 0;
	int iterationNo = 0;
	while (puzzle->solvedCells != puzzle->totalCells) {
		printf("Iteration: %i\n", ++iterationNo);
		puzzle->numChanges = 0;
		testSquares(puzzle);
		testRows(puzzle);
		testColumns(puzzle);
		printPuzzle(puzzle);
		if (puzzle->numChanges == 0) {
			numFailedLoops++;
			if (numFailedLoops == 3) {
				printf("Failed to solve\n");
				//todo: allow the user to guess a cell? Automatically guess a cell?
				break;
			}

		} else {
			puzzle->numChanges = 0;
			numFailedLoops = 0;
		}
	}
	printPuzzle(puzzle);
	//freeBoard(puzzle);

	return 0;
}

board *loadPuzzle(char *filename) {
	int i, j;
	board *puzzle = (board *)calloc(sizeof(board), sizeof(board));
		FILE *fp = fopen(filename, "r");
		if (fp == NULL) {
			perror("Failed to open file:");
		}
		fscanf(fp, "%i", &puzzle->size);
		fscanf(fp, "%i", &puzzle->sectionSize);

		puzzle->totalCells=puzzle->size*puzzle->size;
		puzzle->solvedCells = 0;
		puzzle->numChanges = 0;

		puzzle->boardCells = calloc(sizeof(cell *)*puzzle->size, sizeof(cell *)*puzzle->size);
		for (i = 0; i < puzzle->size; i++) {
			puzzle->boardCells[i] = calloc(sizeof(cell)*puzzle->size,sizeof(cell)*puzzle->size);
			for (j = 0; j < puzzle->size; j++) {
				puzzle->boardCells[i][j].potential = calloc(sizeof(char)*puzzle->size, sizeof(char)*puzzle->size);
				fscanf(fp, "%i", &puzzle->boardCells[i][j].value);
				if (puzzle->boardCells[i][j].value != 0) {
					puzzle->solvedCells++;
				}
			}
		}

		fclose(fp);
	return puzzle;
}

void freeBoard(board *bd) {
	int i, j;
	for (i = 0; i < bd->size; i++) {
		for (j = 0; j < bd->size; j++) {
			free(bd->boardCells[i][j].potential);
		}
		free(bd->boardCells[i]);
	}
	free(bd->boardCells);
	free(bd);
}

void printPuzzle(board *bd) {
	int i, j;
	printf("------------------------\n");
	for (i = 0; i < bd->size; i++) {
		printf("|");
		for (j = 0; j < bd->size; j++) {
			if (bd->boardCells[i][j].value != 0) {
				printf("%i ", bd->boardCells[i][j].value);
			} else {
				printf("  ");
			}
			if ((j+1) % bd->sectionSize == 0) {
				printf("| ");
			}
		}
		printf("\n");
		if ((i+1) % bd->sectionSize == 0) {
			printf("------------------------\n");
		}

	}
	printPuzzleSolve(bd);
}

void printPuzzleSolve(board *bd) {
	int i, j, z, temp;
	printf("------------------------\n");
	for (i = 0; i < bd->size; i++) {
		printf("|");
		for (j = 0; j < bd->size; j++) {
			if (bd->boardCells[i][j].value == 0) {
				temp=0;
				for (z = 0; z < bd->size; z++) {
					temp += bd->boardCells[i][j].potential[z];
				}
				printf("%i ", bd->size-temp);
			} else {
				printf("  ");
			}
			if ((j+1) % bd->sectionSize == 0) {
				printf("| ");
			}
		}
		printf("\n");
		if ((i+1) % bd->sectionSize == 0) {
			printf("------------------------\n");
		}

	}
}

void testSquares(board *bd) {
	int i;
	for (i=0; i < bd->size; i++) {
		testSquare(bd, i%3*3, (i/3)*3);
	}
}

void testSquare(board *bd, int row, int col) {
	int i,j, temp;
	for (i=0; i < bd->size; i++) {
		evaluateCell(&bd->boardCells[row+i%3][col+i/3], bd);
		if (bd->boardCells[row+i%3][col+i/3].value != 0) {
			temp = bd->boardCells[row+i%3][col+i/3].value-1;
			for (j=0; j < bd->size; j++) {
				bd->boardCells[row+j%3][col+j/3].potential[temp]=1;
			}
		}
	}
}


void testRows(board *bd) {
	int i;
	for (i=0; i < bd->size; i++) {
		testRow(bd, i);
	}
}

void testRow(board *bd, int row) {
	int i,j, temp;
	for (i=0; i < bd->size; i++) {
		evaluateCell(&bd->boardCells[row][i], bd);
		if (bd->boardCells[row][i].value != 0) {
			temp = bd->boardCells[row][i].value-1;
			for (j=0; j < bd->size; j++) {
				bd->boardCells[row][j].potential[temp]=1;
			}
		}
	}
}

void testColumns(board *bd) {
	int i;
	for (i=0; i < bd->size; i++) {
		testCol(bd, i);
	}
}

void testCol(board *bd, int col) {
	int i,j, temp;
	for (i=0; i < bd->size; i++) {
		evaluateCell(&bd->boardCells[i][col], bd);
		if (bd->boardCells[i][col].value != 0) {
			temp = bd->boardCells[i][col].value-1;
			for (j=0; j < bd->size; j++) {
				bd->boardCells[j][col].potential[temp]=1;
			}
		}
	}

}

void evaluateCell(cell *cell, board *bd) {
	int i, last=0, count = 0;
	if (cell->value == 0) {
		for (i=0; i < bd->size; i++) {
			if (cell->potential[i] == 0) {
				last = i+1;
				count++;
			}
		}
		if (count == 1) {
			bd->solvedCells++;
			bd->numChanges++;
			cell->value=last;
		}
	}
}



