#ifndef AI
#define AI

#include <stdlib.h>

class AI 
{

	public:
		int player;
		Board *board;
		int gen;

		AI(int p, Board b)
		{	
			player = p;
			board = b;
			srand (time(NULL));
			
		}

		int pick(vector<int> moves)
		{
			//Use a random number generator to randomly pick a move from the moves
			return moves.at(rand() % moves.size()); //generates random number between 0 and moves.size()
			//if you get randomseg faults might be accessing over array bounds
		}

		int chooseMove(void)
		{
			vector<int> moves = board->getAllMoves(player);
			return pick(moves);
		}

};




#endif