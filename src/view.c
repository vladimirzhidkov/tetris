#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/view.h"
#include "include/terminal.h"

#define FRAME_SIZE 800

struct View* createView(struct Game* g) {
	struct View* v = malloc(sizeof(struct View));
	v->frame = malloc(sizeof(int) * FRAME_SIZE);
	v->game = g;
	v->leftside_width = 20;
	v->board_pixel_width = 2;
	v->board_pixel_height = 1;
	v->board_border_thickness = 2;
	v->board_border_stroke = '*';
	v->tetromino_stroke = '0';
	v->board_background_stroke = ' ';
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

int renderSideBorders(struct View* v, char* frame) {
	char* start = frame;
	struct Board* b = v->game->board;
	int col = v->leftside_width;
	for (int row = 1; row <= b->height; ++row) {
		frame += moveCursor(frame, row, col);
		memset(frame, v->board_border_stroke, v->board_border_thickness);
		frame += v->board_border_thickness;
		memset(frame, ' ', b->width * v->board_pixel_width);
		frame += b->width * v->board_pixel_width;
		memset(frame, v->board_border_stroke, v->board_border_thickness);
		frame += v->board_border_thickness;	
	}
	return frame - start;
}

int renderBottomBorder(struct View* v, char* frame) {
	char* start = frame;
	int len = v->game->board->width * v->board_pixel_width + v->board_border_thickness * 2;
	int row = v->game->board->height * v->board_pixel_height + 1;
	int col = v->leftside_width;
	for (int i = 0; i < v->board_border_thickness; ++i) {
		frame += moveCursor(frame, row + i, col);
		memset(frame, v->board_border_stroke, len);
		frame += len;
	}
	return frame - start;
}

void renderBoardView(struct View* v) {
	struct Board* b = v->game->board;
	struct Tetromino* t = v->game->tetromino;
	char* frame = v->frame;
	for (int y = 0; y < b->height; y++) {
		int row = y + 1;
		int col = v->leftside_width + v->board_border_thickness;
		frame += moveCursor(frame, row, col);
		for (int x = 0; x < b->width; x++) {
			char stroke;
			if (readBoard(b, x, y) || isTetroPart(x, y, t)) {
				stroke = v->tetromino_stroke;
			} else {
				stroke = v->board_background_stroke;
			}
			memset(frame, stroke, v->board_pixel_width);
			frame += v->board_pixel_width;
		}
	}
	*frame = '\0';
	sendToTerminal(v->frame, strlen(v->frame));
}

void renderBoardBorders(struct View* v) {
	char* frame = v->frame;
	frame += renderSideBorders(v, frame);
	frame += renderBottomBorder(v, frame); 
	*frame = '\0';
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
	int col = VIEW_LEFTSIDE_WIDTH +
		strlen(VIEW_BOARD_LEFTBORDER) + strlen(VIEW_BOARD_RIGHTBORDER) +
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
