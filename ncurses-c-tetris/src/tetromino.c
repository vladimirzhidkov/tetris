#include <stdlib.h>
#include <string.h>
#include "include/tetromino.h"

#define TETROPOOL_SIZE 7
#define TETROMINO_SIZE 4

int tetroPool[TETROPOOL_SIZE][TETROMINO_SIZE * TETROMINO_SIZE] = {
	{ // line-shape
		0, 0, 0, 0,
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0
	},
	{ // O-shape
		0, 0, 0, 0,
		0, 1, 1, 0,
		0, 1, 1, 0,
		0, 0, 0, 0
	},
	{ // T-shape
		0, 0, 0, 0,
		1, 1, 1, 0,
		0, 1, 0, 0,
		0, 0, 0, 0
	},
	{ // S-shape
		0, 0, 0, 0,
		0, 1, 1, 0,
		1, 1, 0, 0,
		0, 0, 0, 0
	},
	{ // Z-shape
		0, 0, 0, 0,
		0, 1, 1, 0,
		0, 0, 1, 1,
		0, 0, 0, 0
	},
	{ // J-shape
		0, 0, 1, 0,
		0, 0, 1, 0,
		0, 1, 1, 0,
		0, 0, 0, 0
	},
	{ // L-shape
		0, 1, 0, 0,
		0, 1, 0, 0,
		0, 1, 1, 0,
		0, 0, 0, 0
	}
};

int readTetromino(struct Tetromino* t, int x, int y) {
	return *(t->matrix + y * t->size + x);
}
int readNextTetromino(struct Tetromino* t, int x, int y) {
	return *(t->next + y * t->size + x);
}
void writeTetromino(struct Tetromino* t, int x, int y, int value) {
	*(t->matrix + y * t->size + x) = value;
}

void transpose(struct Tetromino* t) {
	for (int i = 0; i < t->size; i++) {
		for (int j = i + 1; j < t->size; j++) {
			int temp = readTetromino(t, j, i);
			writeTetromino(t, j, i, readTetromino(t, i, j));
			writeTetromino(t, i, j, temp);
		}
	}
}

void reverseRows(struct Tetromino* t) {
	for (int i = 0; i < t->size; i++) {
		for (int j = 0, k = t->size - 1; j < k; j++, k--) {
			int temp = readTetromino(t, j, i);
			writeTetromino(t, j, i, readTetromino(t, k, i));
			writeTetromino(t, k, i, temp);
		}
	}
}

void reverseColumns(struct Tetromino* t) {
	for (int i = 0; i < t->size; i++) {
		for (int j = 0, k = t->size - 1; j < k; j++, k--) {
			int temp = readTetromino(t, i, j);
			writeTetromino(t, i, j, readTetromino(t, i, k));
			writeTetromino(t, i, k, temp);
		}
	}
}

struct Tetromino* createTetromino() {
	struct Tetromino* t = malloc(sizeof(struct Tetromino));
	t->x = 0;
	t->y = 0;
	t->size = TETROMINO_SIZE;
	randomizeTetromino(t);
	return t;
}

void destroyTetromino(struct Tetromino* t) {
	free(t);
}

void randomizeTetromino(struct Tetromino* t) {
	int i = rand() % TETROPOOL_SIZE;
	t->matrix = t->next;
	t->next = tetroPool[i];
}

void rotateClockwiseTetromino(struct Tetromino* t) {
	transpose(t);
	reverseRows(t);
}

void rotateCounterClockwiseTetromino(struct Tetromino* t) {
	transpose(t);
	reverseColumns(t);
}

void moveRightTetromino(struct Tetromino* t) {
	t->x++;
}
void moveLeftTetromino(struct Tetromino* t) {
	t->x--;
}
void moveDownTetromino(struct Tetromino* t) {
	t->y++;
}
void moveUpTetromino(struct Tetromino* t) {
	t->y--;
}