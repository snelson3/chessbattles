#ifndef DISPLAY
#define DISPLAY
#include "Board.C"

class Display
{
	public:
		Board *b;
		Display(Board *board){b = board;}
		virtual void update(){;}
		virtual void getMove(int *i, int *j, int *m, int *n){;}
};

#endif