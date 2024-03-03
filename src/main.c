#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "include/main.h"
#include "include/terminal.h"

struct Game* createGame(void) {
	struct Game* g = malloc(sizeof(struct Game));
	g->board = createBoard(BOARD_HEIGHT, BOARD_WIDTH, g);
	g->tetromino = createTetromino();
	g->view = createView();
	g->update_rate_ms = UPDATE_RATE_MS;
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
					y >= g->board->height || readBoard(g->board, y, x))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

void spawnPiece(struct Game* g) {
	randomizeTetromino(g->tetromino);
	g->tetromino->x = (g->board->width - g->tetromino->size) / 2;
	g->tetromino->y = 0;
	if (checkCollision(g)) {
		exitGame(g);
	}
}

void linesClearedEvent(struct Game* g, int lines_cleared) {
	// update lines cleard
	g->lines_cleared += lines_cleared;
	// update level
	g->level = g->lines_cleared / LINES_PER_LEVEL;
	// update score
	int baseValue;
	switch (lines_cleared) {
		case 1: baseValue = 40; break;
		case 2: baseValue = 100; break;
		case 3: baseValue = 300; break;
		case 4: baseValue = 1200; break;
		default: baseValue = 0;
	}
	g->score += baseValue * (g->level + 1);
}

long long current_time_ms() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return (long long)time.tv_sec * 1000 + time.tv_usec / 1000;
}

int main() {
	struct Game* g = createGame();
	initTerminal();
	spawnPiece(g);
	renderLeftSideView(g);
	renderBoardView(g);
	while (1) {
		long long last_updated_time_ms = current_time_ms();

		// loop for a bit, constantly reading from stdin
		int update_rate = g->update_rate_ms - g->level * SPEED_INCREASE_MS;
		while (current_time_ms() - last_updated_time_ms < update_rate) {
			unsigned char c;
			if ((c = getChar()) == 0) {
			 	continue;
			 }
			switch (c) {
				case 'a':
				 	moveLeftTetromino(g->tetromino);
					if (checkCollision(g)) {
						moveRightTetromino(g->tetromino);
					}
					break;
				case 'd':
				 	moveRightTetromino(g->tetromino);
					if (checkCollision(g)) {
						moveLeftTetromino(g->tetromino);
					}
					break;
				case 's':
				 	moveDownTetromino(g->tetromino);
					if (checkCollision(g)) {
						moveUpTetromino(g->tetromino);
						fixTetrominoToBoard(g->board, g->tetromino);
						renderLeftSideView(g);
						spawnPiece(g);
					}
					break;
				case 'w':
					rotateClockwiseTetromino(g->tetromino);
					if (checkCollision(g)) {
						rotateCounterClockwiseTetromino(g->tetromino);
					}
					break;
			}
			renderBoardView(g);
		}
		// drop the tetromino by one step
		moveDownTetromino(g->tetromino);
		if (checkCollision(g)) {
			moveUpTetromino(g->tetromino);
			fixTetrominoToBoard(g->board, g->tetromino);
			renderLeftSideView(g);
			spawnPiece(g);
		}
		renderBoardView(g);
	}
	return 0;
}