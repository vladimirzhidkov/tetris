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

void writeBoard(struct Board* b, int row, int col, int value) {
	*(b->matrix + row * b->width + col) = value; 
}

int readBoard(struct Board* b, int row, int col) {
	return *(b->matrix + row * b->width + col);
}

int clearBoardLines(struct Board* b) {
	int count = 0;
	for (int row = 0; row < b->height; row++) {
		int isFullLine= 1;
		// iterate over row looking for a full line
		for (int col = 0; col < b->width; col++) {
			if (readBoard(b, row, col) == 0) {
				isFullLine = 0;
				break;
			}
		}
		if (isFullLine) {
			count++;
			// Shift rows up starting from current row 
			for (int i = row * b->width - 1; i >= 0; i--) {
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
				writeBoard(b, t->y + y, t->x + x, 1);
			}
		}
	}
	int line_count = clearBoardLines(b);
	linesClearedEvent(b->game, line_count);
}