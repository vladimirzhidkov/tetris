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

int renderLineAt(char* buf, char* line, int row, int col) {
	int char_count = 0;
	char cmd[10];
	sprintf(cmd, "\x1b[%d;%dH", row, col);
	strcpy(buf, cmd);
	buf += strlen(cmd);
	char_count += strlen(cmd);
	strcpy(buf, line);
	char_count += strlen(line);
	return char_count;
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
	sendToTerminal(g->view->frame, strlen(g->view->frame));
}

void renderLeftSideView(struct Game* g) {
	char* offset = g->view->frame;
	char line [100];
	int row = 2;
	int col = 2;
	sprintf(line, "Level: %d", g->level);
	offset += renderLineAt(offset, line, row++, col);
	sprintf(line, "Lines: %d", g->lines_cleared);
	offset += renderLineAt(offset, line, row++, col);
	sprintf(line, "Score: %d", g->score);
	offset += renderLineAt(offset, line, row++, col);
	*offset = '\0';
	sendToTerminal(g->view->frame, strlen(g->view->frame));
}

void renderRightSideView(struct View* v) {
	char* offset = v->frame;
	char line [100];	
	int row = 2;
	int col = LEFT_SIDE_VIEW_WIDTH + 
		strlen(LEFT_BORDER) +
		BOARD_WIDTH * strlen(BOARD_BACKGROUND) +
		strlen(RIGHT_BORDER) + 2;
	sprintf(line, "%c: left", CTRL_MOVELEFT);
	offset += renderLineAt(offset, line, row++, col);
	sprintf(line, "%c: right", CTRL_MOVERIGHT);
	offset += renderLineAt(offset, line, row++, col);
	sprintf(line, "%c: rotate", CTRL_ROTATE);
	offset += renderLineAt(offset, line, row++, col);
	sprintf(line, "%c: speed up", CTRL_SPEEDUP);
	offset += renderLineAt(offset, line, row++, col);
	sprintf(line, "%c: quit", CTRL_QUIT);
	offset += renderLineAt(offset, line, row++, col);
	*offset = '\0';
	sendToTerminal(v->frame, strlen(v->frame));
}
