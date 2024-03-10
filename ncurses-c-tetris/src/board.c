#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "include/board.h"

Board* Board_Init(int height, int width) {
	Board* me = malloc(sizeof(Board));
	me->height = height; 
	me->width = width;
	int matrix_size = sizeof(int) * height * width;
	me->board = malloc(matrix_size);
	memset(me->board, 0, matrix_size);
	return me;
}

void Board_Destroy(Board* me) {
	free(me->board);
	free(me);
}

void Board_Write(Board* me, int x, int y, int value) {
	*(me->board + y * me->width + x) = value; 
}

int Board_Read(Board* me, int x, int y) {
	return *(me->board + y * me->width + x);
}

int Board_ClearLines(Board* me) {
	int count = 0;
	for (int y = 0; y < me->height; y++) {
		int isFullLine= 1;
		// iterate over row looking for a full line
		for (int x = 0; x < me->width; x++) {
			if (Board_Read(me, x, y) == 0) {
				isFullLine = 0;
				break;
			}
		}
		if (isFullLine) {
			count++;
			// Shift rows up starting from current row 
			for (int i = y * me->width - 1; i >= 0; i--) {
				me->board[i + me->width] = me->board[i];
			}
			// add zero line at the top
			memset(me->board, 0, sizeof(int) * me->width);
		}
	}
	return count;
}

void Board_FixTetroToBoard(Board* me, Tetromino *t) {
	for (int y = 0; y < t->size; y++) {
		for (int x = 0; x < t->size; x++) {
			if (Tetromino_Read(t, x, y)) {
				Board_Write(me, t->x + x, t->y + y, 1);
			}
		}
	}
}