#ifndef BOARD_H 
#define BOARD_H

#include "tetromino.h"

typedef struct {
	int *board;
	int height;
	int width;
} Board;

Board* Board_Init(int height, int width);
void Board_Destroy(Board *);
int Board_ClearLines(Board *);
void Board_FixTetroToBoard(Board *, Tetromino *);
void Board_Write(Board *, int x, int y, int value);
int Board_Read(Board *, int x, int y);

#endif // BOARD_H