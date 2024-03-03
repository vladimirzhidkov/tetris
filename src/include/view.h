#ifndef VIEW_H
#define VIEW_H

#include "main.h"
#include "settings.h"

struct View {
	char* frame;
	char* tetrocell;
	struct Game* game;
};

struct View* createView(struct Game*);
void destroyView(struct View*);
void renderBoardView(struct View*); 
void renderLeftSideView(struct View*);
void renderRightSideView(struct View*);

#endif // VIEW_H