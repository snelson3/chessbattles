#include "Board.C"
#include <iostream>
#include <errno.h>

class Gamemaster
{
	public:
		Board board;
		bool checkmate;
		int p[2];
		int t;

		Gamemaster(void)
		{
			board = Board();
			checkmate = false;
			p[0] = 0;
			p[1] = 1;
			t = 0;
		}

		const char* getPlayer(void) {if (p[t] == 0) return "White"; if (p[t] == 1) return "Black";
									return "Err";}

		void makeMove(void)
		{
			std::cout<<"It is currently "<<getPlayer()<<"'s turn"<<endl;
		}

		void changeTurn(void)
		{
			if (t == 0) t++;
			else t--;
			if (t > 1 || t < 0) perror("Wrong turn");
		}

		void isCheckmate(void)
		{
			std::cout<<endl<<"is it checkmate ";
			int i;
			std::cin>>i;
			if (i) checkmate = true;
		}
};