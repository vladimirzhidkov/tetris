#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/view.h"
#include "include/terminal.h"

#define FRAME_SIZE 800

struct View* createView(struct Game* g) {
	struct View* v = malloc(sizeof(struct View));
	v->frame = malloc(sizeof(int) * FRAME_SIZE);
	v->tetrocell = "00";
	v->game = g;
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

void renderBoardView(struct View* v) {
	char* offset = v->frame;
	int board_y;
	for (board_y = 0; board_y < v->game->board->height; board_y++) {
		// left border
		offset += moveCursor(offset, board_y + 1, VIEW_LEFTSIDE_WIDTH);
		strcpy(offset, VIEW_BOARD_LEFTBORDER);
		offset += VIEW_BOARD_PIXELWIDTH;
		// board row
		for (int board_x = 0; board_x < v->game->board->width; board_x++) {
			if (readBoard(v->game->board, board_y, board_x) ||
				isTetroPart(board_x, board_y, v->game->tetromino))
			{
				// TODO: change to level number
				strcpy(offset, v->tetrocell);
				offset += strlen(v->tetrocell);
			} else
			{
				strcpy(offset, VIEW_BOARD_BACKGROUND);
				offset += strlen(VIEW_BOARD_BACKGROUND);
			}
		}
		// right border
		strcpy(offset, VIEW_BOARD_RIGHTBORDER);
		offset += VIEW_BOARD_PIXELWIDTH; 
	}
	// bottom border
	offset += moveCursor(offset, board_y + 1, VIEW_LEFTSIDE_WIDTH + VIEW_BOARD_PIXELWIDTH);
	int bottom_len = v->game->board->width * strlen(VIEW_BOARD_BACKGROUND);
	memset(offset, VIEW_BOARD_BOTTOMBORDER, bottom_len);
	offset += bottom_len;
	*offset = '\0';
	sendToTerminal(v->frame, strlen(v->frame));
}

void renderLeftSideView(struct View* v) {
	char* offset = v->frame;
	char line [100];
	int row = 2;
	int col = 2;
	sprintf(line, "Level: %d", v->game->level);
	offset += renderLineAt(offset, line, row++, col);
	sprintf(line, "Lines: %d", v->game->lines_cleared);
	offset += renderLineAt(offset, line, row++, col);
	sprintf(line, "Score: %d", v->game->score);
	offset += renderLineAt(offset, line, row++, col);
	*offset = '\0';
	sendToTerminal(v->frame, strlen(v->frame));
}

void renderRightSideView(struct View* v) {
	char* offset = v->frame;
	char line [100];	
	int row = 2;
	int col = VIEW_LEFTSIDE_WIDTH + VIEW_BOARD_PIXELWIDTH * 2 +
		BOARD_WIDTH * VIEW_BOARD_PIXELWIDTH + 2;
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
