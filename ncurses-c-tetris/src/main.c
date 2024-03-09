#include <ncurses.h>
#include "include/game.h"

int main() {

	// ncurses settings
	initscr();
	refresh();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	timeout(0);

	// init game	
	struct Game* g = gameCreate();
	gameSpawnTetro(g);
	
	// render views
	viewRenderGameBoard(g->view);
	viewRenderScoreBoard(g->view);

	// game loop
	gameStartEventLoop(g);
	return 0;
}