#ifndef BOARD_H 
#define BOARD_H

#include "tetromino.h"
#include "main.h"

struct Board {
	int* matrix;
	int height;
	int width;
	struct Game* game; // for sending change updates
};

struct Board* createBoard(int height, int width, struct Game*);
void destroyBoard(struct Board*);
int clearBoardLines(struct Board*);
void fixTetrominoToBoard(struct Board*);
void writeBoard(struct Board*, int x, int y, int value);
int readBoard(struct Board*, int x, int y);

#endif // BOARD_H