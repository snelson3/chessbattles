#include <iostream>
#include <errno.h>
#include <stdio.h>
#include "Piece.C"
using std::endl;

class Board
{
	public:
		Piece* board[8][8];

		// White Black Empty
		//   0     1     2

		Piece *getPiece(int i, int j){ return board[i][j];}

		Board(void) 
		{
			for (int i  = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
				{
					if (i == 0)
					{
						if (j == 0 || j == 7)
							board[i][j] = new Rook(0);
						else if (j == 1 || j == 6)
							board[i][j] = new Knight(0);
						else if (j == 2 || j == 5)
							board[i][j] = new Bishop(0);
						else if (j == 3)
							board[i][j] = new Queen(0);
						else if (j == 4)
							board[i][j] = new King(0);
						else
							perror("Invalid location during setup");
					}
					else if (i == 7)
					{
						if (j == 0 || j == 7)
							board[i][j] = new Rook(1);
						else if (j == 1 || j == 6)
							board[i][j] = new Knight(1);
						else if (j == 2 || j == 5)
							board[i][j] = new Bishop(1);
						else if (j == 3)
							board[i][j] = new Queen(1);
						else if (j == 4)
							board[i][j] = new King(1);
						else
							perror("Invalid location during setup");			
					}
					else if (i == 1) board[i][j] = new Pawn(0);
					else if (i == 6) board[i][j] = new Pawn(1);
					else board[i][j] = new Empty(2);
				}
		}

		void displayBoard(){
			std::cout<<"########################################################" << endl;
			std::cout<<"########################################################" << endl;
			std::cout<<"########################################################" << endl;
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
					std::cout<<"# "<<board[7-i][j]->n<<" #";
				std::cout<<endl<<"########################################################" << endl;
			}

		// 	cout<<"########################################################" << endl;
		// 	cout<<"#     ## bKn ## bBi ## bQu ## bKi ## bBi ## bKn ## bRo #" << endl;
		// 	cout<<"########################################################" << endl;
		// 	cout<<"# bPa ## bPa ## bPa ## bPa ## bPa ## bPa ## bPa ## bPa #" << endl;
		// 	cout<<"########################################################" << endl;
		// 	cout<<"#     ##     ##     ##     ##     ##     ##     ##     #" << endl;
		// 	cout<<"########################################################" << endl;
		// 	cout<<"#     ##     ##     ##     ##     ##     ##     ##     #" << endl;
		// 	cout<<"########################################################" << endl;
		// 	cout<<"#     ##     ##     ##     ##     ##     ##     ##     #" << endl;
		// 	cout<<"########################################################" << endl;
		// 	cout<<"#     ##     ##     ##     ##     ##     ##     ##     #" << endl;
		// 	cout<<"########################################################" << endl;
		// 	cout<<"# wPa ## wPa ## wPa ## wPa ## wPa ## wPa ## wPa ## wPa #" << endl;
		// 	cout<<"########################################################" << endl;
		// 	cout<<"# wRo ## wKn ## wBi ## wQu ## wKi ## wBi ## wKn ## wRo #" << endl;
		// 	cout<<"########################################################" << endl;
		// }

		}
};
