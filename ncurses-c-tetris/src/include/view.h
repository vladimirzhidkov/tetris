#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include "game.h"
#include "settings.h"

struct View {
	struct Game* game;
	WINDOW * wscore;
	WINDOW * wnext;
	WINDOW * wboard;
	WINDOW * winstructions;
	int board_pixel_width;
	int board_pixel_height;
};

struct View* viewCreate(struct Game*);
void viewDestroy(struct View*);
void viewRenderGameBoard(struct View*); 
void viewRenderScoreBoard(struct View*);
void viewRenderNextShape(struct View*);

#endif // VIEW_H