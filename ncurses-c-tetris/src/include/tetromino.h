#ifndef TETROMINO_H
#define TETROMINO_H

typedef struct {
	int* current;
	int* next;
	int size;
	int x;
	int y;
} Tetromino;

// create/destroy
Tetromino* Tetromino_Init(void);
void Tetromino_Destroy(Tetromino *);
void Tetromino_Randomize(Tetromino *);

// access
int Tetromino_Read(Tetromino *, int x, int y);
int Tetromino_ReadNext(Tetromino * t, int x, int y);
void Tetromino_Write(Tetromino *, int x, int y, int value);

// transform
void Tetromino_MoveRight(Tetromino *);
void Tetromino_MoveLeft(Tetromino *);
void Tetromino_MoveDown(Tetromino *);
void Tetromino_MoveUp(Tetromino *);
void Tetromino_RotateClockwise(Tetromino *);
void Tetromino_RotateCounterClockwise(Tetromino *);
void transpose(Tetromino *);
void reverseRows(Tetromino *);
void reverseColumns(Tetromino *);

#endif // TETROMINO_H