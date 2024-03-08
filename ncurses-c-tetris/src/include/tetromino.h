#ifndef TETROMINO_H 
#define TETROMINO_H 

struct Tetromino {
	int* current;
	int* next;
	int size;
	int x;
	int y;
};

// create/destroy
struct Tetromino* tetroCreate(void);
void tetroDestroy(struct Tetromino*);
void tetroRandomize(struct Tetromino*);
// access
int tetroRead(struct Tetromino*, int x, int y);
int tetroReadNext(struct Tetromino* t, int x, int y);
void tetroWrite(struct Tetromino*, int x, int y, int value);
// transform
void tetroMoveRight(struct Tetromino*);
void tetroMoveLeft(struct Tetromino*);
void tetroMoveDown(struct Tetromino*);
void tetroMoveUp(struct Tetromino*);
void tetroRotateClockwise(struct Tetromino*);
void tetroRotateCounterClockwise(struct Tetromino*);

#endif // TETROMINO_H