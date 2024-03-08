#ifndef TETROMINO_H 
#define TETROMINO_H 


struct Tetromino {
	int* matrix;
	int* next;
	int size;
	int x;
	int y;
};

struct Tetromino* createTetromino(void);
void destroyTetromino(struct Tetromino*);
void randomizeTetromino(struct Tetromino*);
int readTetromino(struct Tetromino*, int x, int y);
int readNextTetromino(struct Tetromino* t, int x, int y);
void writeTetromino(struct Tetromino*, int x, int y, int value);
void moveRightTetromino(struct Tetromino*);
void moveLeftTetromino(struct Tetromino*);
void moveDownTetromino(struct Tetromino*);
void moveUpTetromino(struct Tetromino*);
void rotateClockwiseTetromino(struct Tetromino*);
void rotateCounterClockwiseTetromino(struct Tetromino*);

#endif // TETROMINO_H