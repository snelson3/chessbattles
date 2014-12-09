#ifndef BOARD
#define BOARD
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include "Piece.C"
using std::endl;

class Board
{
	public:
		Piece* board[8][8];
		int active[2];
		int moveset[4];
		bool mwaiting;
		vector<int> Threats;

		// White Black Empty
		//   0     1     2

		Piece *getPiece(int i, int j){ return board[i][j];}

		Board(void)
		{
			mwaiting = false;
			active[0] = 9;
			active[1] = 9;
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
					// std::cout<<"Player "<<board[i][j]->getPlayer()<<" Piece "<<board[i][j]->getName();
					// std::cout<<" i "<<i<<" j "<<j<<endl;
				}

			// board[0][5] = new King(0);
			// board[7][5] = new King(1);

			// board[4][4] = new Knight(0);
		}

		void setMoveWaiting(int i, int j, int m, int n)
		{
			moveset[0] = i;
			moveset[1] = j;
			moveset[2] = m;
			moveset[3] = n;
			mwaiting = true;
		}

		void clearMoveWaiting()
		{
			mwaiting = false;
		}

		bool getMoveWaiting()
		{ return mwaiting;}

		void setThreats(vector<int> threats)
		{
			Threats = threats;
		}

		void clearThreats()
		{
			vector<int> empty;
			Threats = empty;
		}

		void setActive(int i, int j)
		{
			active[0] = i;
			active[1] = j;
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

		 }

		bool isThreat(int i, int j)
		{
			//return true;
			for (std::vector<int>::iterator z = Threats.begin(); z != Threats.end(); ++z)
			{
				if (board[0][0]->loc1d(i,j) == z[0])
					return true;
			}
			return false;
		}
//When separating displays make 2 classes, one for console display, one for gui display., callable

		void checkThreats(int i, int j){
			std::cout<<"CHECKING " << i<<" "<< j<<" FOR THREATS\n";
			std::cout<<"########################################################" << endl;
			std::cout<<"########################################################" << endl;
			//std::cerr<<"THE SEG FAULT HAPPENS BEFORE GETTING THE THREATS";
			vector<int> threats = board[i][j]->getThreats(i,j,board);
			//std::cerr<<"I'm CONFUSED THAT SHOULD HAVE BEEN WHERE THE THREATS WERE";
			//for (std::vector<int>::iterator z = threats.begin(); z != threats.end(); ++z)
			//	std::cout<<"THREAT "<<z[0];
			for (int m = 7; m >= 0; m--)
			{
				for (int n = 0; n < 8; n++)
				{
					std::cout<<"# ";
					if (i == m && j == n)
					{
						//write the name of the piece
						std::cout<<board[i][j]->n;
					}
					else
					{
						bool q = false;
						 for(std::vector<int>::iterator z = threats.begin(); z != threats.end(); ++z)
						 {
						 	if (board[i][j]->loc1d(m,n) == z[0])
						 	{
						 		q = true;
						 		std::cout<<" X ";
						 	}
						 }
						 if (q == false)
						 	std::cout<<"   ";
						//if its a threat write the X
						//else write
					}
					std::cout<<" #";
				}
			std::cout<<endl<<"########################################################" << endl;
			}
		};


		bool isCheck(int t)
		{
			int g;
			int h;

			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					if (board[i][j]->isKing())
						if(board[i][j]->getPlayerNum() == t)
							{
								//std::cerr<<"Found the king at "<<i<<" "<<j<<"\n";
								g = i;
								h = j;
							}

			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (board[i][j]->getPlayerNum() != 2 && board[i][j]->getPlayerNum() != t)
					{
						vector<int> moves = board[i][j]->getThreats(i,j,board);
						for(std::vector<int>::iterator mo = moves.begin(); mo != moves.end(); ++mo)
						{
							int t;
							int y;
							board[i][j]->loc2d(&t,&y,*mo);
							if (t == g && h == y)
							{
								//std::cerr<<"R TRUE";
								return true;
							}
						}
					}
				}
			}
			return false;
		}

};

#endif