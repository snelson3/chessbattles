#include "Display.C"
#include "Board.C"

class Display_gui : public Display
{
	public:
		Board *b;
		Display_gui(Board *board) : Display(board) {;}
		virtual void update(){;}
		virtual void getMove(int *i,int *j,int *m,int *n){;}
};