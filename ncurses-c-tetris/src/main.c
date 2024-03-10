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
	Game* g = Game_Init();
	Game_SpawnTetro(g);
	
	// render views
	View_RenderGameBoard(g->view, g->board, g->tetromino);
	View_RenderScoreBoard(g->view, g->level, g->lines_cleared, g->score);

	// game loop
	Game_StartEventLoop(g);
	return 0;
}