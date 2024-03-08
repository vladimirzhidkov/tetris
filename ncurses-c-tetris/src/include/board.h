#ifndef BOARD_H 
#define BOARD_H

#include "tetromino.h"
#include "main.h"

struct Board {
	int* board;
	int height;
	int width;
	struct Game* game; // for sending change updates
};

struct Board* boardCreate(int height, int width, struct Game*);
void boardDestroy(struct Board*);
int boardClearLines(struct Board*);
void boardFixTetroToBoard(struct Board*);
void boardWrite(struct Board*, int x, int y, int value);
int boardRead(struct Board*, int x, int y);

#endif // BOARD_H