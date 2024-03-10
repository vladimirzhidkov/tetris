#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include "board.h"
#include "tetromino.h"
#include "settings.h"

typedef struct {
	WINDOW * wscore;
	WINDOW * wnext;
	WINDOW * wboard;
	WINDOW * winstructions;
	int board_pixel_width;
	int board_pixel_height;
} View;

View* View_Init(Board *, Tetromino *);
void View_Destroy(View *);
void View_RenderGameBoard(View *, Board *, Tetromino *); 
void View_RenderScoreBoard(View *, int level, int lines, int score);
void View_RenderNextShape(View *, Tetromino *);

#endif // VIEW_H