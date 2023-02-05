#include "frdm_bsp.h"
#include "MKL05Z4.h"
#include "stdbool.h"

enum Type {
	NONE = 0,
	PAWN,
	ROOK,
	BISHOP,
	KNIGHT,
	QUEEN,
	KING,
};

enum Color {
	BLACK = 1,
	WHITE,
};

enum Occupation {
	OCCUPIED = 0,
	EMPTY,
};

struct Piece {
	enum Type type;
	enum Color color;
	enum Occupation is_occupied;
};

struct Board {
	struct Piece pieces[8][8];
	char column[8];
	char row[8];
};

void check(struct Board *detected_board);
void keypad_init(void);
