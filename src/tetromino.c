#include <stdlib.h>
#include <string.h>
#include "include/tetromino.h"

#define TETROPOOL_SIZE 7
#define TETROMINO_SIZE 4

/*
Tetromino Shapes:
-	straight line
O	square
T	T shape.
S	zigzag oriented horizontally
Z	mirrored S shape
J	J shape
L	mirrored J shape
*/
const int tetroPool[][TETROMINO_SIZE][TETROMINO_SIZE] = {
	{
		{0, 0, 0, 0},
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},
		{1, 1, 1, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 1, 1},
		{0, 0, 0, 0}
	},
	{
		{0, 0, 1, 0},
		{0, 0, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	},
	{
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	}
};

int readTetromino(struct Tetromino* t, int row, int col) {
	return *(t->matrix + row * t->size + col);
}

void writeTetromino(struct Tetromino* t, int row, int col, int value) {
	*(t->matrix + row * t->size + col) = value;
}

void transpose(struct Tetromino* t) {
	for (int i = 0; i < t->size; i++) {
		for (int j = i + 1; j < t->size; j++) {
			int temp = readTetromino(t, i, j);
			writeTetromino(t, i, j, readTetromino(t, j, i));
			writeTetromino(t, j, i, temp);
		}
	}
}

void reverseRows(struct Tetromino* t) {
	for (int i = 0; i < t->size; i++) {
		for (int j = 0, k = t->size - 1; j < k; j++, k--) {
			int temp = readTetromino(t, i, j);
			writeTetromino(t, i, j, readTetromino(t, i, k));
			writeTetromino(t, i, k, temp);
		}
	}
}

void reverseColumns(struct Tetromino* t) {
	for (int i = 0; i < t->size; i++) {
		for (int j = 0, k = t->size - 1; j < k; j++, k--) {
			int temp = readTetromino(t, j, i);
			writeTetromino(t, j, i, readTetromino(t, k, i));
			writeTetromino(t, k, i, temp);
		}
	}
}

struct Tetromino* createTetromino() {
	struct Tetromino* t = malloc(sizeof(struct Tetromino));
	t->x = 0;
	t->y = 0;
	t->size = TETROMINO_SIZE;
	t->matrix = malloc(sizeof(int) * t->size * t->size);
	return t;
}

void destroyTetromino(struct Tetromino* t) {
	free(t->matrix);
	free(t);
}

void randomizeTetromino(struct Tetromino* t) {
	int i = rand() % TETROPOOL_SIZE;
	memcpy(t->matrix, &tetroPool[i], sizeof(int) * t->size * t->size);
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