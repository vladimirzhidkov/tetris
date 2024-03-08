#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "include/view.h"

// private functions
char * str_repeat_char(char c, int n);

// public functions
struct View* viewCreate(struct Game* g) {
	struct View* v = malloc(sizeof(struct View));
	v->game = g;
	v->board_pixel_width = VIEW_BOARD_PIXEL_WIDTH;
	v->board_pixel_height = VIEW_BOARD_PIXEL_HEIGH;

	// ncurses settings
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_YELLOW);

	// create Next Shape window 
	int wnext_height = g->tetromino->size * v->board_pixel_height + 2;
	int wnext_width = g->tetromino->size * v->board_pixel_width + 2;
	v->wnext = newwin(wnext_height, wnext_width, 1, 0);

	// create Game Board window
	// +2 for borders
	int gameboard_width = g->board->width * v->board_pixel_width + 2;
	int gameboard_height = g->board->height * v->board_pixel_height + 2;
	v->wboard = newwin(gameboard_height, gameboard_width, 0, wnext_width);
	
	// create Score window
	v->wscore = newwin(gameboard_height, 20, 1, wnext_width + gameboard_width + 1);
	// v->winstructions = newwin(gameboard_height - wnext_height, v->leftside_width, wnext_height, 1);
	return v;
}

void viewDestroy(struct View* v) {
	delwin(v->winstructions);
	delwin(v->wboard);
	delwin(v->wscore);
	delwin(v->wnext);
	endwin();
	free(v);
}

char * str_repeat_char(char c, int n) {
	char* str = (char*)malloc((n + 1) * sizeof(char));
	memset(str, c, n);
	str[n] = 0;
	return str;
}

int isTetroPart(int board_x, int board_y, struct Tetromino* t) {
	int tetro_x = board_x - t->x;
	int tetro_y = board_y - t->y;
	if (tetro_x >= 0 && tetro_x < t->size && tetro_y >=0 && tetro_y < t->size) {
		return tetroRead(t, tetro_x, tetro_y);
	} else {
		return 0;
	}
}

void viewRenderGameBoard(struct View* v) {
	struct Board* b = v->game->board;
	struct Tetromino* t = v->game->tetromino;
	char* cell = str_repeat_char(' ', v->board_pixel_width);
	wclear(v->wboard);
	for (int y = 0, row = 1; y < b->height; y++, row += v->board_pixel_height) {
		for (int i = 0; i < v->board_pixel_height; ++i) {
			wmove(v->wboard, row + i, 1);
			for (int x = 0; x < b->width; x++) {
				int value = boardRead(b, x, y) || isTetroPart(x, y, t);
				wattron(v->wboard, COLOR_PAIR(value));
				wprintw(v->wboard, cell);
				wattroff(v->wboard, COLOR_PAIR(value));
			}
		}
	}
	box(v->wboard, 0, 0);
	wrefresh(v->wboard);
	free(cell);
}

void viewRenderScoreBoard(struct View* v) {
	wclear(v->wscore);
	wattrset(v->wscore, A_BOLD);
	wprintw(v->wscore, "Level: %d\n", v->game->level);
	wprintw(v->wscore, "Lines: %d\n", v->game->lines_cleared);
	wprintw(v->wscore, "Score: %d\n", v->game->score);
	wrefresh(v->wscore);
}

void viewRenderNextShape(struct View* v) {
	struct Tetromino* t = v->game->tetromino;
	char* cell = str_repeat_char(' ', v->board_pixel_width);
	wclear(v->wnext);
	wattrset(v->wnext, A_BOLD);
	wprintw(v->wnext, " Next:");
	for (int y = 0, row = 2; y < t->size; y++, row += v->board_pixel_height) {
		for (int i = 0; i < v->board_pixel_height; ++i) {
			wmove(v->wnext, row + i, 1);
			for (int x = 0; x < t->size; x++) {
				int value = tetroReadNext(t, x, y);
				wattron(v->wnext, COLOR_PAIR(value));
				wprintw(v->wnext, cell);
				wattroff(v->wnext, COLOR_PAIR(value));
			}
		}
	}
	free(cell);
	wrefresh(v->wnext);
}