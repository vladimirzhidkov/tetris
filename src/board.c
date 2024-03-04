#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "include/board.h"
#include "stdio.h"

struct Board* createBoard(int height, int width, struct Game* g) {
	struct Board* b = malloc(sizeof(struct Board));
	b->game = g;
	b->height = height; 
	b->width = width;
	int matrix_size = sizeof(int) * height * width;
	b->matrix = malloc(matrix_size);
	memset(b->matrix, 0, matrix_size);
	return b;
}

void destroyBoard(struct Board* b) {
	free(b->matrix);
	free(b);
}

void writeBoard(struct Board* b, int x, int y, int value) {
	*(b->matrix + y * b->width + x) = value; 
}

int readBoard(struct Board* b, int x, int y) {
	return *(b->matrix + y * b->width + x);
}

int clearBoardLines(struct Board* b) {
	int count = 0;
	for (int y = 0; y < b->height; y++) {
		int isFullLine= 1;
		// iterate over row looking for a full line
		for (int x = 0; x < b->width; x++) {
			if (readBoard(b, x, y) == 0) {
				isFullLine = 0;
				break;
			}
		}
		if (isFullLine) {
			count++;
			// Shift rows up starting from current row 
			for (int i = y * b->width - 1; i >= 0; i--) {
				b->matrix[i + b->width] = b->matrix[i];
			}
			// add zero line at the top
			memset(b->matrix, 0, sizeof(int) * b->width);
		}
	}
	return count;
}

void fixTetrominoToBoard(struct Board* b) {
	struct Tetromino* t = b->game->tetromino;
	for (int y = 0; y < t->size; y++) {
		for (int x = 0; x < t->size; x++) {
			if (readTetromino(t, y, x)) {
				writeBoard(b, t->x + x, t->y + y, 1);
			}
		}
	}
	int line_count = clearBoardLines(b);
	linesClearedEvent(b->game, line_count);
}