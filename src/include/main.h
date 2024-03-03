#ifndef MAIN_H
#define MAIN_H

#include "board.h"
#include "tetromino.h"
#include "view.h"

#define UPDATE_RATE_MS 1000
#define SPEED_INCREASE_MS 90
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES_PER_LEVEL 10

struct Game {
	struct Board* board;
	struct Tetromino* tetromino;
	struct View* view;
	int score;
	int level;
	int lines_cleared;
	int update_rate_ms;
};

struct Game* createGame(void);
void destroyGame(struct Game*);
void exitGame(struct Game*); 

void spawnPiece(struct Game*);
int checkCollision(struct Game* g);

void linesClearedEvent(struct Game*, int lines_cleared);


#endif // MAIN_H