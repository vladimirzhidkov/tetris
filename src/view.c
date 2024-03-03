#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/view.h"
#include "include/terminal.h"

#define BOARD_FRAME_SIZE 800
#define FRAME_SIZE 800

struct View* createView(void) {
	struct View* v = malloc(sizeof(struct View));
	v->frame = malloc(sizeof(int) * FRAME_SIZE);
	return v;
}

void destroyView(struct View* v) {
	free(v->frame);
	free(v);
}

/*
   \x1b   the escape character
   [      the control sequence introducer (CSI)
   1;10H  the command to move to 1st row 10th col
          relative to H (Home; top left corner)
*/
int moveCursor(char* buf, int row, int column) {
	char cmd[10];
	sprintf(cmd, "\x1b[%d;%dH", row, column);
	strcpy(buf, cmd);
	return strlen(cmd);
}

int renderLevel(char* buf, int level) {
	char txt[100];
	sprintf(txt, "Level: %d", level);
	strcpy(buf, txt);
	return strlen(txt);
}

int renderLinesCleared(char* buf, int lines_cleared) {
	char txt[100];
	sprintf(txt, "Lines: %d", lines_cleared);
	strcpy(buf, txt);
	return strlen(txt);
}

int renderScore(char* buf, int score) {
	char txt[100];
	sprintf(txt, "Score: %d", score);
	strcpy(buf, txt);
	return strlen(txt);
}
 
int isTetroPart(int board_x, int board_y, struct Tetromino* t) {
	int tetro_x = board_x - t->x;
	int tetro_y = board_y - t->y;
	if (tetro_x >= 0 && tetro_x < t->size && tetro_y >=0 && tetro_y < t->size) {
		return readTetromino(t, tetro_y, tetro_x);
	} else {
		return 0;
	}
}

void renderBoardView(struct Game* g) {
	char* offset = g->view->frame;
	int board_y;
	for (board_y = 0; board_y < g->board->height; board_y++) {
		// left border
		offset += moveCursor(offset, board_y + 1, LEFT_SIDE_VIEW_WIDTH);
		strcpy(offset, LEFT_BORDER);
		offset += strlen(LEFT_BORDER);
		// board row
		for (int board_x = 0; board_x < g->board->width; board_x++) {
			if (readBoard(g->board, board_y, board_x) ||
				isTetroPart(board_x, board_y, g->tetromino))
			{
				// TODO: change to level number
				strcpy(offset, TETROCELL);
				offset += strlen(TETROCELL);
			} else
			{
				strcpy(offset, BOARD_BACKGROUND);
				offset += strlen(BOARD_BACKGROUND);
			}
		}
		// right border
		strcpy(offset, RIGHT_BORDER);
		offset += strlen(RIGHT_BORDER);
	}
	// bottom border
	offset += moveCursor(offset, board_y + 1, LEFT_SIDE_VIEW_WIDTH + strlen(LEFT_BORDER));
	int bottom_len = g->board->width * strlen(BOARD_BACKGROUND);
	memset(offset, BOTTOM_BORDER, bottom_len);
	offset += bottom_len;
	*offset = '\0';
	int size = strlen(g->view->frame);
	sendToTerminal(g->view->frame, size);
}

void renderLeftSideView(struct Game* g) {
	char* offset = g->view->frame;
	offset += moveCursor(offset, 2, 2);
	offset += renderLevel(offset, g->level);
	offset += moveCursor(offset, 3, 2);
	offset += renderLinesCleared(offset, g->lines_cleared);
	offset += moveCursor(offset, 4, 2);
	offset += renderScore(offset, g->score);
	*offset = '\0';
	int frame_size = strlen(g->view->frame);
	sendToTerminal(g->view->frame, frame_size);
}