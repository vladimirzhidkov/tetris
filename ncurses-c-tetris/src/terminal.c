#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "include/terminal.h"

struct termios orig_termios;

void disableRawModeTerminal() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	char* showCursorCmd = "\x1b[?25h";
	write(STDOUT_FILENO, showCursorCmd, strlen(showCursorCmd));
}

// TODO: add comments
void enableRawModeTerminal() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawModeTerminal);

	struct termios raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void initTerminal(void) {
	enableRawModeTerminal();
	// TODO: hook fn to Ctrl+c

	// Set STDIN_FILENO file descriptor to non-blocking mode
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(STDIN_FILENO, F_SETFL, flags);

	// Set STDOUT_FILENO file descriptor to non-blocking mode
	flags = fcntl(STDOUT_FILENO, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(STDOUT_FILENO, F_SETFL, flags);

	// clear screen and hide cursor
	char* clearScreenCmd = "\x1b[2J";
	write(STDOUT_FILENO, clearScreenCmd, strlen(clearScreenCmd));
	char* hideCursorCmd = "\x1b[?25l";
	write(STDOUT_FILENO, hideCursorCmd, strlen(hideCursorCmd));
}
// TODO: writeToTerminal
void sendToTerminal(char* frame, int size) {
	write(STDOUT_FILENO, frame, size);
}

char getChar() {
	unsigned char c;
	return (read(STDIN_FILENO, &c, sizeof(c)) < 1) ? 0 : c;
}