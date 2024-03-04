#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "include/main.h"
#include "include/terminal.h"

struct Game* createGame(void) {
	struct Game* g = malloc(sizeof(struct Game));
	g->board = createBoard(BOARD_HEIGHT, BOARD_WIDTH, g);
	g->tetromino = createTetromino();
	g->view = createView(g);
	g->score = 0;
	g->level = 0;
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
			if (readTetromino(g->tetromino, cellY, cellX)) {
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
	g->view->tetromino_stroke = g->level + 48; // convert level number to char
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
	while (1) {
		long long last_updated_time_ms = current_time_ms();
		// loop for a bit, constantly reading from stdin
		int update_rate = UPDATE_RATE_BASE_MS - UPDATE_RATE_REDUCTION_MS * g->level;
		while (current_time_ms() - last_updated_time_ms < update_rate) {
			unsigned char c;
			if ((c = getChar()) == 0) {
			 	continue;
			 }
			switch (c) {
				case CTRL_MOVELEFT:
				 	moveLeftTetromino(g->tetromino);
					if (checkCollision(g)) {
						moveRightTetromino(g->tetromino);
					}
					break;
				case CTRL_MOVERIGHT:
				 	moveRightTetromino(g->tetromino);
					if (checkCollision(g)) {
						moveLeftTetromino(g->tetromino);
					}
					break;
				case CTRL_SPEEDUP:
				 	moveDownTetromino(g->tetromino);
					if (checkCollision(g)) {
						moveUpTetromino(g->tetromino);
						fixTetrominoToBoard(g->board);
						renderLeftSideView(g->view);
						spawnTetromino(g);
					}
					break;
				case CTRL_ROTATE:
					rotateClockwiseTetromino(g->tetromino);
					if (checkCollision(g)) {
						rotateCounterClockwiseTetromino(g->tetromino);
					}
					break;
				case CTRL_QUIT:
					exitGame(g);
			}
			renderBoardView(g->view);
		}
		// drop the tetromino by one step
		moveDownTetromino(g->tetromino);
		if (checkCollision(g)) {
			moveUpTetromino(g->tetromino);
			fixTetrominoToBoard(g->board);
			renderLeftSideView(g->view);
			spawnTetromino(g);
		}
		renderBoardView(g->view);
	}
}

int main() {
	struct Game* g = createGame();
	initTerminal();
	renderLeftSideView(g->view);
	renderBoardBorders(g->view);
	renderRightSideView(g->view);
	renderBoardView(g->view);

	spawnTetromino(g);
	startGameLoop(g);
	return 0;
}