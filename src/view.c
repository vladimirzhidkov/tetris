#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/view.h"
#include "include/board.h"
#include "include/tetromino.h"

#define BOARD_FRAME_SIZE 800

struct View* createView(void) {
	struct View* v = malloc(sizeof(struct View));
	v->board_size = BOARD_FRAME_SIZE;
	v->board = malloc(sizeof(int) * BOARD_FRAME_SIZE);
	return v;
}

void destroyView(struct View* v) {
	free(v->board);
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

int isTetroPart(int board_x, int board_y, struct Tetromino* t) {
	int tetro_x = board_x - t->x;
	int tetro_y = board_y - t->y;
	if (tetro_x >= 0 && tetro_x < t->size && tetro_y >=0 && tetro_y < t->size) {
		return readTetromino(t, tetro_y, tetro_x);
	} else {
		return 0;
	}
}

void renderBoardView(struct View* v, struct Board* b, struct Tetromino* t) {
	char* offset = (char*)v->board;
	int board_y;
	for (board_y = 0; board_y < b->height; board_y++) {
		// left border
		offset += moveCursor(offset, board_y + 1, LEFT_SIDE_VIEW_WIDTH);
		strcpy(offset, LEFT_BORDER);
		offset += strlen(LEFT_BORDER);
		// board row
		for (int board_x = 0; board_x < b->width; board_x++) {
			if (readBoard(b, board_y, board_x) || isTetroPart(board_x, board_y, t)) {
				strcpy(offset, TETROCELL);
				offset += strlen(TETROCELL);
			} else {
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
	int bottom_len = b->width * strlen(BOARD_BACKGROUND);
	memset(offset, BOTTOM_BORDER, bottom_len);
	offset += bottom_len;
	*offset = '\0';
	v->board_size = strlen(v->board);
}

void renderLeftSideView(struct View* v, struct Board* b) {

}