#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "include/board.h"


struct Board* createBoard(int height, int width) {
	struct Board* b = malloc(sizeof(struct Board));
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

void clearBoardLines(struct Board* b) {
	for (int y = 0; y < b->height; y++) {
		bool isFullLine= true;
		for (int x = 0; x < b->width; x++) {
			if (readBoard(b, y, x) == 0) {
				isFullLine = false;
				break;
			}
		}
		if (isFullLine) {
			// Shift rows up starting from y
			int* flat_ptr = (int* )b->matrix;
			for (int i = y * b->width - 1; i >= 0; i--) {
				flat_ptr[i + b->width] = flat_ptr[i];
			}
			// add zero line at the top
			memset(flat_ptr, 0, sizeof(int) * b->width);
		}
	}
}

void fixTetrominoToBoard(struct Board* b, struct Tetromino* t) {
	for (int y = 0; y < t->size; y++) {
		for (int x = 0; x < t->size; x++) {
			if (readTetromino(t, y, x)) {
				writeBoard(b, t->y + y, t->x + x, 1);
			}
		}
	}
	clearBoardLines(b);
}