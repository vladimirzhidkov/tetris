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
	int update_rate_base;
	int update_rate_reduction;
};

struct Game* createGame(void);
void destroyGame(struct Game*);
void exitGame(struct Game*); 
void spawnPiece(struct Game*);
int checkCollision(struct Game* g);
void linesClearedEvent(struct Game*, int lines_cleared);

#endif // MAIN_H