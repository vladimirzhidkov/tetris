#ifndef VIEW_H
#define VIEW_H

#include "main.h"
#include "board.h"
#include "tetromino.h"

#define LEFT_SIDE_VIEW_WIDTH 25

#define BOARD_BACKGROUND ". "
#define BOTTOM_BORDER '='
#define LEFT_BORDER "<!"
#define RIGHT_BORDER "!>"

#define TETROCELL "11"

struct View {
	char* frame;
};

struct View* createView(void);
void destroyView(struct View*);

void renderBoardView(struct Game*); 
void renderLeftSideView(struct Game*);

#endif // VIEW_H