#ifndef VIEW_H
#define VIEW_H

#include "main.h"
#include "settings.h"
#define VIEW_BOARD_PIXELWIDTH 2
#define VIEW_BOARD_PIXELHEIGH 1
#define VIEW_BOARD_LEFTBORDER "**"
#define VIEW_BOARD_RIGHTBORDER "**"
#define VIEW_BOARD_BOTTOMBORDER_CELL '*'
#define VIEW_BOARD_CELL "  "
#define VIEW_TETROMINO_CELL "00"
#define VIEW_LEFTSIDE_WIDTH 20
struct View {
	char* frame;
	struct Game* game;

	int leftside_width;

	int board_pixel_width;
	int board_pixel_height;
	int board_border_thickness;
	char board_border_stroke;
	char tetromino_stroke;
	char board_background_stroke;
};

struct View* createView(struct Game*);
void destroyView(struct View*);
void renderBoardView(struct View*); 
void renderLeftSideView(struct View*);
void renderRightSideView(struct View*);
void renderBoardBorders(struct View*);

#endif // VIEW_H