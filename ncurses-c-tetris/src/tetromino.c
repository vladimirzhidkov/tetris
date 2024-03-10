#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/tetromino.h"

#define TETROPOOL_SIZE 7
#define TETROMINO_SIZE 4

static int tetroPool[TETROPOOL_SIZE][TETROMINO_SIZE * TETROMINO_SIZE] = {
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

Tetromino* Tetromino_Init() {
	Tetromino* me = malloc(sizeof(Tetromino));
	me->x = 0;
	me->y = 0;
	me->size = TETROMINO_SIZE;
	srand(time(NULL)); // seed random generator
	Tetromino_Randomize(me);
	return me;
}

void Tetromino_Destroy(Tetromino* me) {
	free(me);
}

void Tetromino_Randomize(Tetromino* me) {
	int i = rand() % TETROPOOL_SIZE;
	me->current = me->next;
	me->next = tetroPool[i];
}

int Tetromino_Read(Tetromino* me, int x, int y) {
	return *(me->current + y * me->size + x);
}

int Tetromino_ReadNext(Tetromino* me, int x, int y) {
	return *(me->next + y * me->size + x);
}

void Tetromino_Write(Tetromino* me, int x, int y, int value) {
	*(me->current + y * me->size + x) = value;
}

void transpose(Tetromino* me) {
	for (int i = 0; i < me->size; i++) {
		for (int j = i + 1; j < me->size; j++) {
			int temp = Tetromino_Read(me, j, i);
			Tetromino_Write(me, j, i, Tetromino_Read(me, i, j));
			Tetromino_Write(me, i, j, temp);
		}
	}
}

void reverseRows(Tetromino* me) {
	for (int i = 0; i < me->size; i++) {
		for (int j = 0, k = me->size - 1; j < k; j++, k--) {
			int temp = Tetromino_Read(me, j, i);
			Tetromino_Write(me, j, i, Tetromino_Read(me, k, i));
			Tetromino_Write(me, k, i, temp);
		}
	}
}

void reverseColumns(Tetromino* me) {
	for (int i = 0; i < me->size; i++) {
		for (int j = 0, k = me->size - 1; j < k; j++, k--) {
			int temp = Tetromino_Read(me, i, j);
			Tetromino_Write(me, i, j, Tetromino_Read(me, i, k));
			Tetromino_Write(me, i, k, temp);
		}
	}
}

void Tetromino_RotateClockwise(Tetromino* me) {
	transpose(me);
	reverseRows(me);
}

void Tetromino_RotateCounterClockwise(Tetromino* me) {
	transpose(me);
	reverseColumns(me);
}

void Tetromino_MoveRight(Tetromino* me) {
	me->x++;
}
void Tetromino_MoveLeft(Tetromino* me) {
	me->x--;
}
void Tetromino_MoveDown(Tetromino* me) {
	me->y++;
}
void Tetromino_MoveUp(Tetromino* me) {
	me->y--;
}