#ifndef DISPLAY
#define DISPLAY
#include "Board.C"

class Display
{
	public:
		Board *b;
		Display(){;}
		virtual void update(){;}
		virtual void getMove(int *i, int *j, int *m, int *n){;}
};

#endif