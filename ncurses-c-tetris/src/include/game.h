#ifndef GAME_H
#define GAME_H

typedef struct Game Game;

#include "board.h"
#include "tetromino.h"
#include "view.h"
#include "settings.h"

struct Game {
	Board *board;
	Tetromino *tetromino;
	View *view;
	int score;
	int level;
	int lines_cleared;
	int update_rate;
};

Game* Game_Init(void);
void Game_Destroy(Game *);
void Game_Exit(Game *); 
void Game_SpawnTetro(Game *);
void Game_StartEventLoop(Game *);
int Game_CheckCollision(Game *);
void Game_KeyPressed(Game *, int key);

#endif // GAME_H