/*
 * main.c
 *
 *  Created on: Jul 8, 2014
 *      Author: cameron
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char value;
	char *potential;
} cell;

typedef struct {
	int solvedCells;
	int totalCells;
	int numChanges;
	cell **boardCells;

} board;


board *loadPuzzle(char *filename);
void freeBoard(board *bd);
void printPuzzle(board *bd);
void testSquares(board *bd);
void testRows(board *bd);
void testColumns(board *bd);

int main(int argc, char **argv) {
	board *puzzle;
	puzzle = loadPuzzle(argv[1]);
	printPuzzle(puzzle);
	while (puzzle->solvedCells != puzzle->totalCells) {
		puzzle->numChanges = 0;
		testSquares(puzzle);
		testRows(puzzle);
		testColumns(puzzle);
		if (puzzle->numChanges) {
			printf("Failed to solve");
			break;
			//todo: allow the user to guess a cell? Automatically guess a cell?
		}
	}
	printPuzzle(puzzle);
	freeBoard(puzzle);

	return 0;
}

board *loadPuzzle(char *filename) {
	return NULL;
}
void freeBoard(board *bd) {

}
void printPuzzle(board *bd) {

}
void testSquares(board *bd) {

}
void testRows(board *bd) {

}
void testColumns(board *bd) {

}



