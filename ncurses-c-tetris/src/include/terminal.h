#ifndef TERMINAL_H
#define TERMINAL_H

void initTerminal(void);
void sendToTerminal(char* frame, int size);
char getChar();

#endif // TERMINAL_H