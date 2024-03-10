#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ncurses.h>
#include "include/game.h"

// private helper functions
long long current_time_ms();
int calc_score(int lines_cleared, int level);


// public functions
Game* Game_Init(void) {
	Game* me = malloc(sizeof(Game));
	me->board = Board_Init(BOARD_HEIGHT, BOARD_WIDTH);
	me->tetromino = Tetromino_Init();
	me->view = View_Init(me->board, me->tetromino);
	me->score = 0;
	me->level = 0;
	me->update_rate = GAME_UPDATE_RATE_BASE_MS - GAME_UPDATE_RATE_REDUCTION_MS * me->level;
	return me;
}

void Game_Destroy(Game* me) {
	Board_Destroy(me->board);
	Tetromino_Destroy(me->tetromino);
	View_Destroy(me->view);
	free(me);
}

void Game_Exit(Game* me) {
	int level = me->level;
	int lines = me->lines_cleared;
	int score = me->score;
	Game_Destroy(me);
	printf("Game Over\n");
	printf("Level:%d\n", level);
	printf("Lines:%d\n", lines);
	printf("Score:%d\n", score);
	exit(0);
}

int Game_CheckCollision(Game* me) {
	for (int cellY = 0; cellY < me->tetromino->size; cellY++) {
		for (int cellX = 0; cellX < me->tetromino->size; cellX++) {
			if (Tetromino_Read(me->tetromino, cellX, cellY)) {
				int x = me->tetromino->x + cellX;
				int y = me->tetromino->y + cellY;
				if (x < 0 || x >= me->board->width ||
					y >= me->board->height || Board_Read(me->board, x, y))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

void Game_SpawnTetro(Game* me) {
	Tetromino_Randomize(me->tetromino);
	me->tetromino->x = (me->board->width - me->tetromino->size) / 2;
	me->tetromino->y = 0;
	if (Game_CheckCollision(me)) {
		Game_Exit(me);
	}
	View_RenderNextShape(me->view, me->tetromino);
}

int calc_score(int lines_cleared, int level) {
	int base_value;
	switch (lines_cleared) {
		case 1: base_value = GAME_RULES_BASEVALUE_LEVEL1; break;
		case 2: base_value = GAME_RULES_BASEVALUE_LEVEL2; break;
		case 3: base_value = GAME_RULES_BASEVALUE_LEVEL3; break;
		case 4: base_value = GAME_RULES_BASEVALUE_LEVEL4; break;
		default: base_value = 0;
	}
	return base_value * (level + 1);
}

void Game_UpdateState(Game* me, int lines_cleared) {
	if (lines_cleared) {
		me->lines_cleared += lines_cleared;
		me->level = me->lines_cleared / GAME_RULES_LINES_PER_LEVEL;
		me->update_rate = GAME_UPDATE_RATE_BASE_MS - GAME_UPDATE_RATE_REDUCTION_MS * me->level;
		me->score += calc_score(lines_cleared, me->level);
	}
}

void Game_FixTetroToBoard(Game *me) {
	Board_FixTetroToBoard(me->board, me->tetromino);
	int lines_cleared= Board_ClearLines(me->board);
	Game_UpdateState(me, lines_cleared);
	View_RenderScoreBoard(me->view, me->level, me->lines_cleared, me->score);
	Game_SpawnTetro(me);
}

void Game_NextTick(Game * me) {
	Tetromino_MoveDown(me->tetromino);
	if (Game_CheckCollision(me)) {
		Tetromino_MoveUp(me->tetromino);
		Game_FixTetroToBoard(me);
	}
}

void Game_KeyPressed(Game* me, int key) {
	switch (key) {
		case KEY_LEFT:
			Tetromino_MoveLeft(me->tetromino);
			if (Game_CheckCollision(me)) {
				Tetromino_MoveRight(me->tetromino);
			}
			break;
		case KEY_RIGHT:
			Tetromino_MoveRight(me->tetromino);
			if (Game_CheckCollision(me)) {
				Tetromino_MoveLeft(me->tetromino);
			}
			break;
		case KEY_DOWN:
		case ' ':
			Game_NextTick(me);
			break;
		case KEY_UP:
			Tetromino_RotateClockwise(me->tetromino);
			if (Game_CheckCollision(me)) {
				Tetromino_RotateCounterClockwise(me->tetromino);
			}
			break;
		case CTRL_QUIT:
			Game_Exit(me);
	}
	View_RenderGameBoard(me->view, me->board, me->tetromino);
}

long long current_time_ms() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return (long long)time.tv_sec * 1000 + time.tv_usec / 1000;
}

void Game_StartEventLoop(Game* me) {
	long long last_updated_time_ms = current_time_ms();
	int key;
	while (1) {
		if (current_time_ms() > last_updated_time_ms + me->update_rate) {
			Game_NextTick(me);
			View_RenderGameBoard(me->view, me->board, me->tetromino);
			last_updated_time_ms = current_time_ms();
		}
		if ((key = getch()) != ERR) {
			Game_KeyPressed(me, key);
		}
	}
}