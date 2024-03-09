#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/tetromino.h"

#define TETROPOOL_SIZE 7
#define TETROMINO_SIZE 4

// private functions
void transpose(struct Tetromino* t);
void reverseRows(struct Tetromino* t);
void reverseColumns(struct Tetromino* t);

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

struct Tetromino* tetroCreate() {
	struct Tetromino* t = malloc(sizeof(struct Tetromino));
	t->x = 0;
	t->y = 0;
	t->size = TETROMINO_SIZE;
	srand(time(NULL)); // seed random generator
	tetroRandomize(t);
	return t;
}

void tetroDestroy(struct Tetromino* t) {
	free(t);
}

void tetroRandomize(struct Tetromino* t) {
	int i = rand() % TETROPOOL_SIZE;
	t->current = t->next;
	t->next = tetroPool[i];
}

int tetroRead(struct Tetromino* t, int x, int y) {
	return *(t->current + y * t->size + x);
}

int tetroReadNext(struct Tetromino* t, int x, int y) {
	return *(t->next + y * t->size + x);
}

void tetroWrite(struct Tetromino* t, int x, int y, int value) {
	*(t->current + y * t->size + x) = value;
}

void transpose(struct Tetromino* t) {
	for (int i = 0; i < t->size; i++) {
		for (int j = i + 1; j < t->size; j++) {
			int temp = tetroRead(t, j, i);
			tetroWrite(t, j, i, tetroRead(t, i, j));
			tetroWrite(t, i, j, temp);
		}
	}
}

void reverseRows(struct Tetromino* t) {
	for (int i = 0; i < t->size; i++) {
		for (int j = 0, k = t->size - 1; j < k; j++, k--) {
			int temp = tetroRead(t, j, i);
			tetroWrite(t, j, i, tetroRead(t, k, i));
			tetroWrite(t, k, i, temp);
		}
	}
}

void reverseColumns(struct Tetromino* t) {
	for (int i = 0; i < t->size; i++) {
		for (int j = 0, k = t->size - 1; j < k; j++, k--) {
			int temp = tetroRead(t, i, j);
			tetroWrite(t, i, j, tetroRead(t, i, k));
			tetroWrite(t, i, k, temp);
		}
	}
}

void tetroRotateClockwise(struct Tetromino* t) {
	transpose(t);
	reverseRows(t);
}

void tetroRotateCounterClockwise(struct Tetromino* t) {
	transpose(t);
	reverseColumns(t);
}

void tetroMoveRight(struct Tetromino* t) {
	t->x++;
}
void tetroMoveLeft(struct Tetromino* t) {
	t->x--;
}
void tetroMoveDown(struct Tetromino* t) {
	t->y++;
}
void tetroMoveUp(struct Tetromino* t) {
	t->y--;
}