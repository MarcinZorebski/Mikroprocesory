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
	char coordinate_letter[8];
	char coordinate_number[8];
};

struct Board check(void);
void keypad_init(void);
