#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "include/view.h"

// private helper functions
char * str_repeat_char(char c, int n);

// public functions
View* View_Init(Board *b, Tetromino *t) {
	View* me = malloc(sizeof(View));
	me->board_pixel_width = VIEW_BOARD_PIXEL_WIDTH;
	me->board_pixel_height = VIEW_BOARD_PIXEL_HEIGH;

	// ncurses settings
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_YELLOW);

	// create Next Shape window 
	int wnext_height = t->size * me->board_pixel_height + 2;
	int wnext_width = t->size * me->board_pixel_width + 2;
	me->wnext = newwin(wnext_height, wnext_width, 1, 0);

	// create Game Board window
	// +2 for borders
	int gameboard_width = b->width * me->board_pixel_width + 2;
	int gameboard_height = b->height * me->board_pixel_height + 2;
	me->wboard = newwin(gameboard_height, gameboard_width, 0, wnext_width);
	
	// create Score window
	me->wscore = newwin(gameboard_height, 20, 1, wnext_width + gameboard_width + 1);
	return me;
}

void View_Destroy(View* me) {
	delwin(me->winstructions);
	delwin(me->wboard);
	delwin(me->wscore);
	delwin(me->wnext);
	endwin();
	free(me);
}

char * str_repeat_char(char c, int n) {
	char* str = (char*)malloc((n + 1) * sizeof(char));
	memset(str, c, n);
	str[n] = 0;
	return str;
}

int isTetroPart(int board_x, int board_y, Tetromino* t) {
	int tetro_x = board_x - t->x;
	int tetro_y = board_y - t->y;
	if (tetro_x >= 0 && tetro_x < t->size && tetro_y >=0 && tetro_y < t->size) {
		return Tetromino_Read(t, tetro_x, tetro_y);
	} else {
		return 0;
	}
}

void View_RenderGameBoard(View* me, Board *b, Tetromino *t) {
	char* cell = str_repeat_char(' ', me->board_pixel_width);
	wclear(me->wboard);
	for (int y = 0, row = 1; y < b->height; y++, row += me->board_pixel_height) {
		for (int i = 0; i < me->board_pixel_height; ++i) {
			wmove(me->wboard, row + i, 1);
			for (int x = 0; x < b->width; x++) {
				int value = Board_Read(b, x, y) || isTetroPart(x, y, t);
				wattron(me->wboard, COLOR_PAIR(value));
				wprintw(me->wboard, cell);
				wattroff(me->wboard, COLOR_PAIR(value));
			}
		}
	}
	box(me->wboard, 0, 0);
	wrefresh(me->wboard);
	free(cell);
}

void View_RenderScoreBoard(View* me, int level, int lines, int score) {
	wclear(me->wscore);
	wattrset(me->wscore, A_BOLD);
	wprintw(me->wscore, "Level: %d\n", level);
	wprintw(me->wscore, "Lines: %d\n", lines);
	wprintw(me->wscore, "Score: %d\n", score);
	wrefresh(me->wscore);
}

void View_RenderNextShape(View* me, Tetromino * t) {
	char* cell = str_repeat_char(' ', me->board_pixel_width);
	wclear(me->wnext);
	wattrset(me->wnext, A_BOLD);
	wprintw(me->wnext, " Next:");
	for (int y = 0, row = 2; y < t->size; y++, row += me->board_pixel_height) {
		for (int i = 0; i < me->board_pixel_height; ++i) {
			wmove(me->wnext, row + i, 1);
			for (int x = 0; x < t->size; x++) {
				int value = Tetromino_ReadNext(t, x, y);
				wattron(me->wnext, COLOR_PAIR(value));
				wprintw(me->wnext, cell);
				wattroff(me->wnext, COLOR_PAIR(value));
			}
		}
	}
	free(cell);
	wrefresh(me->wnext);
}