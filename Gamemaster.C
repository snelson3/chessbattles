#include "Board.C"

class Gamemaster
{
	public:
		Board board;
		bool checkmate;

		Gamemaster(void)
		{
			board = new Board();
			checkmate = true;


		}

		void makeMove(void)
		{
			;
		}

		void changeTurn(void){;}

		void isCheckmate(void){;}
};