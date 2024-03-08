#ifndef MAIN_H
#define MAIN_H

#include "board.h"
#include "tetromino.h"
#include "view.h"
#include "settings.h"

struct Game {
	struct Board* board;
	struct Tetromino* tetromino;
	struct View* view;
	int score;
	int level;
	int lines_cleared;
	int update_rate;
};

struct Game* gameCreate(void);
void gameDestroy(struct Game*);
void gameExit(struct Game*); 

// Events
void gameEventLinesCleared(struct Game*, int lines_cleared);
void gameEventTetroFixedToBoard(struct Game*);

#endif // MAIN_H