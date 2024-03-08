#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include "main.h"
#include "settings.h"

struct View {
	struct Game* game;
	int leftside_width;
	WINDOW * wscore;
	WINDOW * wnext;
	WINDOW * wboard;
	WINDOW * winstructions;
	int board_pixel_width;
	int board_pixel_height;
};

struct View* createView(struct Game*);
void destroyView(struct View*);
void viewRenderGameBoard(struct View*); 
void viewRenderInstructions(struct View*);
void viewRenderScoreBoard(struct View*);
void viewRenderNextShape(struct View*);
#endif // VIEW_H