#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ncurses.h>

#include "include/main.h"
#include "include/terminal.h"

struct Game* createGame(void) {
	// ncurses settings
	initscr();
	refresh();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	timeout(0);
	struct Game* g = malloc(sizeof(struct Game));
	g->board = createBoard(BOARD_HEIGHT, BOARD_WIDTH, g);
	g->tetromino = createTetromino();
	g->view = createView(g);
	g->score = 0;
	g->level = 0;
	g->update_rate = UPDATE_RATE_BASE_MS - UPDATE_RATE_REDUCTION_MS * g->level;
	return g;
}

void destroyGame(struct Game* g) {
	destroyBoard(g->board);
	destroyTetromino(g->tetromino);
	destroyView(g->view);
	free(g);
}

void exitGame(struct Game* g) {
	destroyGame(g);
	puts("\nGame Over\n");
	exit(0);
}

int checkCollision(struct Game* g) {
	for (int cellY = 0; cellY < g->tetromino->size; cellY++) {
		for (int cellX = 0; cellX < g->tetromino->size; cellX++) {
			if (readTetromino(g->tetromino, cellX, cellY)) {
				int x = g->tetromino->x + cellX;
				int y = g->tetromino->y + cellY;
				if (x < 0 || x >= g->board->width ||
					y >= g->board->height || readBoard(g->board, x, y))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

void spawnTetromino(struct Game* g) {
	randomizeTetromino(g->tetromino);
	g->tetromino->x = (g->board->width - g->tetromino->size) / 2;
	g->tetromino->y = 0;
	if (checkCollision(g)) {
		exitGame(g);
	}
}

void linesClearedEvent(struct Game* g, int lines_cleared) {
	g->lines_cleared += lines_cleared;
	g->level = g->lines_cleared / RULES_LINES_PER_LEVEL;
	g->update_rate = UPDATE_RATE_BASE_MS - UPDATE_RATE_REDUCTION_MS * g->level;
	// update score
	int base_value;
	switch (lines_cleared) {
		case 1: base_value = RULES_BASEVALUE_LEVEL1; break;
		case 2: base_value = RULES_BASEVALUE_LEVEL2; break;
		case 3: base_value = RULES_BASEVALUE_LEVEL3; break;
		case 4: base_value = RULES_BASEVALUE_LEVEL4; break;
		default: base_value = 0;
	}
	g->score += base_value * (g->level + 1);
}

long long current_time_ms() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return (long long)time.tv_sec * 1000 + time.tv_usec / 1000;
}

void startGameLoop(struct Game* g) {
	long long last_updated_time_ms = current_time_ms();
	while (1) {
		if (current_time_ms() - last_updated_time_ms > g->update_rate) {
			moveDownTetromino(g->tetromino);
			if (checkCollision(g)) {
				moveUpTetromino(g->tetromino);
				fixTetrominoToBoard(g->board);
				viewRenderScoreBoard(g->view);
				spawnTetromino(g);
			}
			viewRenderGameBoard(g->view);
			last_updated_time_ms = current_time_ms();
		}
		int c = getch();
		if (c == ERR) {
			continue;
		}
		switch (c) {
			case KEY_LEFT:
				moveLeftTetromino(g->tetromino);
				if (checkCollision(g)) {
					moveRightTetromino(g->tetromino);
				}
				break;
			case KEY_RIGHT:
				moveRightTetromino(g->tetromino);
				if (checkCollision(g)) {
					moveLeftTetromino(g->tetromino);
				}
				break;
			case KEY_DOWN:
				moveDownTetromino(g->tetromino);
				if (checkCollision(g)) {
					moveUpTetromino(g->tetromino);
					fixTetrominoToBoard(g->board);
					viewRenderScoreBoard(g->view);
					spawnTetromino(g);
				}
				break;
			case KEY_UP:
				rotateClockwiseTetromino(g->tetromino);
				if (checkCollision(g)) {
					rotateCounterClockwiseTetromino(g->tetromino);
				}
				break;
			case CTRL_QUIT:
				exitGame(g);
		}
		viewRenderGameBoard(g->view);
	}
}

int main() {
	struct Game* g = createGame();
	spawnTetromino(g);
	viewRenderInstructions(g->view);
	viewRenderGameBoard(g->view);
	viewRenderScoreBoard(g->view);
	startGameLoop(g);
	return 0;
}