#ifndef VIEW_H
#define VIEW_H

#include "board.h"
#include "tetromino.h"

#define LEFT_SIDE_VIEW_WIDTH 25

#define BOARD_BACKGROUND ". "
#define BOTTOM_BORDER '='
#define LEFT_BORDER "<!"
#define RIGHT_BORDER "!>"

#define TETROCELL "11"

struct View {
	char* board;
	int board_size;
};

struct View* createView(void);
void destroyView(struct View*);

void renderBoardView(struct View*, struct Board*, struct Tetromino*);
void renderLeftSideView(struct View*, struct Board*);
#endif // VIEW_H