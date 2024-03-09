#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ncurses.h>
#include "include/game.h"

// private functions
int checkCollision(struct Game* g);
long long current_time_ms();
int calcScore(int lines_cleared, int level);


// public functions
struct Game* gameCreate(void) {

	struct Game* g = malloc(sizeof(struct Game));
	g->board = boardCreate(BOARD_HEIGHT, BOARD_WIDTH, g);
	g->tetromino = tetroCreate();
	g->view = viewCreate(g);
	g->score = 0;
	g->level = 0;
	g->update_rate = GAME_UPDATE_RATE_BASE_MS - GAME_UPDATE_RATE_REDUCTION_MS * g->level;
	return g;
}

void gameDestroy(struct Game* g) {
	boardDestroy(g->board);
	tetroDestroy(g->tetromino);
	viewDestroy(g->view);
	free(g);
}

void gameExit(struct Game* g) {
	int level = g->level;
	int lines = g->lines_cleared;
	int score = g->score;
	gameDestroy(g);
	printf("Game Over\n");
	printf("Level:%d\n", level);
	printf("Lines:%d\n", lines);
	printf("Score:%d\n", score);
	exit(0);
}

int checkCollision(struct Game* g) {
	for (int cellY = 0; cellY < g->tetromino->size; cellY++) {
		for (int cellX = 0; cellX < g->tetromino->size; cellX++) {
			if (tetroRead(g->tetromino, cellX, cellY)) {
				int x = g->tetromino->x + cellX;
				int y = g->tetromino->y + cellY;
				if (x < 0 || x >= g->board->width ||
					y >= g->board->height || boardRead(g->board, x, y))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

void gameSpawnTetro(struct Game* g) {
	tetroRandomize(g->tetromino);
	g->tetromino->x = (g->board->width - g->tetromino->size) / 2;
	g->tetromino->y = 0;
	if (checkCollision(g)) {
		gameExit(g);
	}
	viewRenderNextShape(g->view);
}

int calcScore(int lines_cleared, int level) {
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

// gets called if lines_cleard > 0
void gameEventLinesCleared(struct Game* g, int lines_cleared) {
	g->lines_cleared += lines_cleared;
	g->level = g->lines_cleared / GAME_RULES_LINES_PER_LEVEL;
	g->update_rate = GAME_UPDATE_RATE_BASE_MS - GAME_UPDATE_RATE_REDUCTION_MS * g->level;
	g->score += calcScore(lines_cleared, g->level);
}

void gameEventTetroFixedToBoard(struct Game* g) {
	viewRenderScoreBoard(g->view);
	gameSpawnTetro(g);
}

void gameEventKeyPressed(struct Game* g, int key) {
	switch (key) {
		case KEY_LEFT:
			tetroMoveLeft(g->tetromino);
			if (checkCollision(g)) {
				tetroMoveRight(g->tetromino);
			}
			break;
		case KEY_RIGHT:
			tetroMoveRight(g->tetromino);
			if (checkCollision(g)) {
				tetroMoveLeft(g->tetromino);
			}
			break;
		case KEY_DOWN:
		case ' ':
			tetroMoveDown(g->tetromino);
			if (checkCollision(g)) {
				tetroMoveUp(g->tetromino);
				boardFixTetroToBoard(g->board);
			}
			break;
		case KEY_UP:
			tetroRotateClockwise(g->tetromino);
			if (checkCollision(g)) {
				tetroRotateCounterClockwise(g->tetromino);
			}
			break;
		case CTRL_QUIT:
			gameExit(g);
	}
	viewRenderGameBoard(g->view);
}

long long current_time_ms() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return (long long)time.tv_sec * 1000 + time.tv_usec / 1000;
}

void gameStartEventLoop(struct Game* g) {
	long long last_updated_time_ms = current_time_ms();
	int key;
	while (1) {
		if (current_time_ms() - last_updated_time_ms > g->update_rate) {
			tetroMoveDown(g->tetromino);
			if (checkCollision(g)) {
				tetroMoveUp(g->tetromino);
				boardFixTetroToBoard(g->board);
			}
			viewRenderGameBoard(g->view);
			last_updated_time_ms = current_time_ms();
		}
		if ((key = getch()) != ERR) {
			gameEventKeyPressed(g, key);
		}
	}
}