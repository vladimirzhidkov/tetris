#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "include/main.h"
#include "include/terminal.h"

struct Game* createGame(void) {
	struct Game* g = malloc(sizeof(struct Game));
	g->board = createBoard(BOARD_HEIGHT, BOARD_WIDTH);
	g->tetromino = createTetromino();
	g->view = createView();
	g->update_rate_ms = UPDATE_RATE_MS;
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

long long current_time_ms() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return (long long)time.tv_sec * 1000 + time.tv_usec / 1000;
}

int main() {
	struct Game* g = createGame();
	initTerminal();
	spawnPiece(g);
	renderBoardView(g->view, g->board, g->tetromino);
	sendToTerminal(g->view->board, g->view->board_size);
	while (1) {
		long long last_updated_time_ms = current_time_ms();
		while (current_time_ms() - last_updated_time_ms < UPDATE_RATE_MS) {
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

			renderBoardView(g->view, g->board, g->tetromino);
			sendToTerminal(g->view->board, g->view->board_size);
		}
		moveDownTetromino(g->tetromino);
		if (checkCollision(g)) {
			moveUpTetromino(g->tetromino);
			fixTetrominoToBoard(g->board, g->tetromino);
			spawnPiece(g);
		}
		renderBoardView(g->view, g->board, g->tetromino);
		sendToTerminal(g->view->board, g->view->board_size);
	}
	return 0;
}