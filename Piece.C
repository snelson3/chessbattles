#ifndef PIECE
#define PIECE
#include <stdio.h>
#include <iostream>
#include "Move.C"
#include <vector>

using std::vector;

class Piece
{
	public:
		int player;
		char n[3];
		const char *name;
		bool moved;
		bool iKing;
	//	Piece b[8][8];

		Piece(int p)
		{
			player = p;
			moved = false;
			
		}


		void print(){
			std::cerr<<"TEST\n";
		}

		void setN()
		{
			if (player == 0)
				sprintf(n,"w%s",name);
			else if (player == 1)
				sprintf(n,"b%s",name);
			else sprintf(n," %s",name);
		}

		bool isKing()
		{
			//std::cerr<<"Is this breaking?";
			return iKing;
		}

		const char *getPlayer(){if (player == 0) return "White"; if (player == 1) return "Black"; return "";}

		int getPlayerNum(){return player;}

		const char *getName(){return name;}

		int loc1d(int i, int j)
		{
			//if (i < 0 || i > 7 || j < 0 || j > 7) return -1;
			//my threats array is getting locations that are out of bounds, but thats ok for now I think, its just messy
			return i*8+j;
		}

		void addThreat(int i, int j, vector<int> *mo)
		{
			if (i < 0 || i > 7 || j < 0 || j > 7) return;
			//if (b[i][j]->getPlayerNum() == getPlayerNum()) return;
			mo->push_back(loc1d(i,j));
			return;
		}

		void loc2d(int *i, int *j, int m)
		{
			//convert m into an i and a j
			//have to modulo m which is i, then do some multiplication and subtraction for j
			//std::cerr<<"IT BREAKS HERE KAMIKAMIHA";
			*i = m / 8;
			*j = m - (*i*8);
			//std::cerr<<"\nBANKAI!\n";
		}
		
		virtual int canMove(int i, int j, int m, int n, Piece *board[8][8])
		{
			return 1;
		}

		virtual vector<int> getThreats(int i, int j, Piece *board[8][8])
		{

			vector<int> threats;

			return threats;
		}
		

};

class Pawn : public Piece
{
	public:

		Pawn(int p) : Piece(p)
		{
			name = "Pa";
			iKing = false;
			setN();
		}

		virtual int canMove(int i, int j, int m, int n, Piece *board[8][8])
		{//r value of 3 means pawn moved so check for leveling up
						//This is where you need to figure out to change the sign
			//or just change the matrix for each player

			if (getPlayerNum() == 1)
			{	
				if (moved == false)
				{
					if ((i-2 == m && j == n) && board[m][n]->getPlayerNum() == 2) if (board[m+1][n]->getPlayerNum() == 2) return 3;
				}
				if ((i-1 == m && j == n) && board[m][n]->getPlayerNum() == 2) return 3;
				if ((i-1 == m)&&(j+1 == n || j-1 == n))
				{
					if (board[m][n]->getPlayerNum() != 2 && board[m][n]->getPlayerNum()!= player)
						return 3;
				}
			}
			if (getPlayerNum() == 0)
			{		
				if (moved == false)
				{
					// 			std::cerr<<"WAKKAWAKKA "<<board[m][n]->getPlayerNum()<<std::endl;
					// moved = true;
					if ((i+2 == m && j == n) && board[m][n]->getPlayerNum() == 2) if (board[m-1][n]->getPlayerNum() == 2) return 3;
				}
				if ((i+1 == m && j == n) && board[m][n]->getPlayerNum() == 2) return 3;
				if ((i+1 == m)&&(j+1 == n || j-1 == n))
				{
					if (board[m][n]->getPlayerNum() != 2 && board[m][n]->getPlayerNum()!= player)
						return 3;
				}
			}
			return -1;
		}
//I need to fix the bug where pawns can jump over another piece if they haven't moved
//also pawn a7 can't move to a5
		virtual vector<int> getThreats(int i, int j, Piece *board[8][8])
		{
			vector<int> threats;

			if (getPlayerNum() == 1)
			{	

				if (moved == false)
				{
					if ((board[i-2][j]->getPlayerNum() == 2) && (board[i-1][j]->getPlayerNum() == 2))
					{
						//threats.push_back(loc1d(i-2,j));
						addThreat(i-2,j,&threats);
					}
				}

				if (board[i-1][j]->getPlayerNum() == 2) addThreat(i-1,j,&threats);//threats.push_back(loc1d(i-1,j));
				
				if (j > 0)
					if (board[i-1][j-1]->getPlayerNum() == 0) addThreat(i-1,j-1,&threats);//threats.push_back(loc1d(i-1,j-1));

				if (j < 7)
					if (board[i-1][j+1]->getPlayerNum() == 0) addThreat(i-1,j+1,&threats);//threats.push_back(loc1d(i-1,j+1));
			}

			else if (getPlayerNum() == 0)
			{	

				if (moved == false)
				{
					if ((board[i+2][j]->getPlayerNum() == 2) && (board[i+1][j]->getPlayerNum() == 2))
					{
					//	threats.push_back(loc1d(i+2,j));
						addThreat(i+2,j,&threats);
					}
				}

				if (board[i+1][j]->getPlayerNum() == 2) addThreat(i+1,j,&threats);//threats.push_back(loc1d(i+1,j));
				
				if (j < 7)
					if (board[i+1][j+1]->getPlayerNum() == 1) addThreat(i+1,j+1,&threats);//threats.push_back(loc1d(i+1,j+1));

				if (j > 0)
					if (board[i+1][j-1]->getPlayerNum() == 1) addThreat(i+1,j-1,&threats);//threats.push_back(loc1d(i+1,j-1));
			}

			return threats;
		}
};

class Rook : public Piece
{

	//moving a2/a4 and a1/a3 doesn't work

	//0 0   3 0
	public:
		Rook(int p) : Piece(p)
		{
			name = "Ro";
			iKing = false;
			setN();
		}

		virtual int canMove(int i, int j, int m, int n, Piece *board[8][8])
		{

			//this is pretty bad, find better method (theres a way to use temp vars)
			// if (i == m)
			// 	if (j > n)
			// 		//moving left
			// 	else
			// 		//moving right
			// else if (j == n)
			// 	if (i > m)
			// 		//moving down
			// 	else
			// 		//moving right
			// else
			// 	return -1;

			if (i == m)
			{
				//row is the same
				if (j > n)
				{
					//rook is moving to the left
					while (1)
					{
						j--;
						if (j < 0) return -1;
						if (j == n) return 1;
						if (board[i][j]->getPlayerNum() != 2)
							return -1;
					}

				}
				else if (j < n)
				{
					//rook is moving to the right
					while (1)
					{
						j++;
						if (j > 7) return -1;
						if (j == n) return 1;
						if (board[i][j]->getPlayerNum() != 2)
							return -1;
					}
				}

			}
			if (j == n)
			{
				//horizontal axis is the same
				if (i > m)
				{
					//rook is moving down
					while (1)
					{
						i--;
						if (i < 0) return -1;
						if (i == m) return 1;
						if (board[i][j]->getPlayerNum() !=2)
							return -1;
					}
				}
				else if (i < m)
				{
					//rook is moving up
					while(1)
					{
					i++;
					if (i > 7) return -1;
					if (j == m) return 1;
					if (i == m) return 1;
					if (board[i][j]->getPlayerNum() != 2)
						return -1;
					}
				}
			}

			return -1;
		}		

		virtual vector<int> getThreats(int i, int j, Piece *board[8][8])
		{
			vector<int> threats;

			bool trick = false;
			//rook moves up, increases i
			for (int m = i+1; m < 8; ++m)
			{
				if (!trick)
				{
					if (board[m][j]->getPlayerNum() == 2)
					{
						//threats.push_back(loc1d(m,j));
						addThreat(m,j,&threats);
					}
					else if (board[m][j]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,j));
						addThreat(m,j,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}
			//rook moves down decreases i
			trick = false;
			for (int m = i-1; m >= 0; --m)
			{
				if (!trick)
				{
					if (board[m][j]->getPlayerNum() == 2)
						//threats.push_back(loc1d(m,j));
						addThreat(m,j,&threats);
					else if (board[m][j]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,j));
						addThreat(m,j,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}
			//rook moves left decreases j
			trick = false;
			for (int n = j-1; n >= 0; n--)
			{
				if (!trick)
				{
					if (board[i][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(i,n));
						addThreat(i,n,&threats);
					else if (board[i][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
					//	threats.push_back(loc1d(i,n));
						addThreat(i,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}
			//rook moves right increases j
			trick = false;
			for (int n = j+1; n < 8 ; n++)
			{
				if (!trick)
				{
					if (board[i][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(i,n));
						addThreat(i,n,&threats);
					else if (board[i][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(i,n));
						addThreat(i,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}
			return threats;
		}
};

class Knight : public Piece
{
	public: 
		Knight(int p) : Piece(p)
		{
			name = "Kn";
			iKing = false;
			setN();
		}

		virtual int canMove(int i, int j, int m, int n, Piece *board[8][8])
		{

			//it can move 1 right 2 up
			if (i+2 == m && j+1 == n) return 1;
			//it can move 2 right and 1 up
			if (j+2 == n && i+1 == m) return 1;
			//it can move 1 left 2 up
			if (j-1 == n && i+2 == m) return 1;
			//it can move 2 left and 1 up
			if (j-2 == n && i+1 == m) return 1;
			//it can move 2 left and 1 down
			if (j-2 == n && i-1 == m) return 1;
			//it can move 1 left and 2 down
			if (j-1 == n && i-2 == m) return 1;
			//it can move 1 right and 2 down
			if (j+1 == n && i-2 == m) return 1;
			//it can move 2 right and 1 down
			if (j+2 == n && i-1 == m) return 1;
			return -1;
		}

		virtual vector<int> getThreats(int i, int j, Piece *board[8][8])
		{
			vector<int> threats;

			if (i+2 < 8 && j+1 < 8)
			if (board[i+2][j+1]->getPlayerNum() != board[i][j]->getPlayerNum())
				addThreat(i+2,j+1,&threats);

			if (i+1 < 8 && j+2 < 8)
			if (board[i+1][j+2]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i+1,j+2));
				addThreat(i+1,j+2,&threats);

			if (i+2 < 8 && j-1 >= 0)
			if (board[i+2][j-1]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i+2,j-1));
				addThreat(i+2,j-1,&threats);

			if (i+1 < 8 && j-2 >= 0)
			if (board[i+1][j-2]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i+1,j-2));
				addThreat(i+1,j-2,&threats);

			if (i-1 >= 0 && j-2 >= 0)
			if (board[i-1][j-2]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i-1,j-2));
				addThreat(i-1,j-2,&threats);

			if (i-2 >= 0 && j-1 >= 0)
			if (board[i-2][j-1]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i-2,j-1));
				addThreat(i-2,j-1,&threats);

			if (i-2 >= 0 && j+1 < 8)
			if (board[i-2][j+1]->getPlayerNum() != board[i][j]->getPlayerNum())	
				//threats.push_back(loc1d(i-2,j+1));
				addThreat(i-2,j+1,&threats);

			if (i-1>=0 && j+2 < 8)
			if (board[i-1][j+2]->getPlayerNum() != board[i][j]->getPlayerNum())
			//	threats.push_back(loc1d(i-1,j+2));
				addThreat(i-1,j+2,&threats);

			return threats;
		}
};

class Bishop : public Piece
{
	public:
		Bishop(int p) : Piece(p)
		{
			name = "Bi";
			iKing = false;
			setN();
		}

		virtual int canMove(int i, int j, int m, int n, Piece *board[8][8])
		{//0 2   1 3
			std::cerr<<"FCK "<<i<<","<<j<<" "<<m<<","<<n<<"\n";
			if (i > m){
							if (j > n)
							{
								while (1)
								{
									i--;
									j--;
									if (i < 0 || i < 0) return -1;
									if (i == m && j == n) return 1;
									if (board[i][j]->getPlayerNum() != 2) return -1;
								}
								//moving down/left
							}
							else if (j < n)
								//moving down/right
								while(1)
								{
									i--;
									j++;
									if (i < 0 || j > 7) return -1;
									if (i == m && j == n) return 1;
									if (board[i][j]->getPlayerNum() != 2) return -1;
								}}
			else if (i < m){
							if (j > n)
								//moving up/left
								while(1)
								{
									i++;
									j--;
									if (i > 7 || j < 0) return -1;
									if (i == m && j == n) return 1;
									if (board[i][j]->getPlayerNum() != 2) return -1;
								}
							else if (j < n)
								//moving up/right
								while (1)
								{
									std::cerr<<"WEE ";
									i++;
									j++;
									std::cerr<<i<<","<<j;
									if (i > 7 || j > 7) return -1;
									if (i == m && j == n) return 1;
									if (board[i][j]->getPlayerNum() != 2) return -1;
								}}

			return -1;
		}

		virtual vector<int> getThreats(int i, int j, Piece *board[8][8])
		{

			//6 3    7|1 1 2 1 1 1 1 1
			//       6|2 2 2 1 2 2 2 2 
			//       5|2 2 2 2 2 2 2 2
			//       4|2 2 2 2 2 2 2 2
			//       3|2 2 2 2 2 2 2 2
			//       2|2 2 2 2 2 2 2 2
			//       1|2 2 2 0 2 2 2 2
			//       0|0 0 2 0 0 0 0 0
			//        |________________
			//        |0 1 2 3 4 5 6 7

			//std::cerr<<"THE SEG FAULT HAPPENS HERE";
			vector<int> threats;

			bool trick = false;
			int m = i;  //m = 6
			int n = j;  //n = 3
			//moving up/left
			while (m < 7 && n > 0)
			{
				m++; //m = 7
				n--; //n = 2
				if (!trick)
				{
					if (board[m][n]->getPlayerNum() == 2)
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
					}
					else if (board[m][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
						trick = true;
					}
					else
					{
						trick = true;
					}
				}
			}

			//moving down/left
			trick = false;
			m = i;
			n = j;

			while (m > 0 && n > 0)
			{
				m--;
				n--;
				if (!trick)
				{
					if (board[m][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
					else if (board[m][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}			

			//moving up/right
			trick = false;
			m = i;
			n = j;

			while ((m < 7) && (n <7) )
			{
				m++;
				n++;
				if (!trick)
				{
					if (board[m][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
					else if (board[m][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}		
			//moving down/right

			trick = false;
			m = i;
			n = j;

			while (m > 0 && n < 7)
			{
				m--;
				n++;
				if (!trick)
				{
					if (board[m][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
					else if (board[m][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}		

			return threats;
		}
};

class Queen : public Piece 
{
	public:
		Queen(int p) : Piece(p)
		{
			name = "Qu";
			iKing = false;
			setN();
		}

		virtual int canMove(int i, int j, int m, int n, Piece *board[8][8])
		{
			
if (i == m)
			{
				//vertical axis is the same
				if (j > n)
				{
					//rook is moving to the left
					while (1)
					{
						j--;
						if (j < 0) return -1;
						if (j == n) return 1;
						if (board[i][j]->getPlayerNum() != 2)
							return -1;
					}

				}
				else if (j < n)
				{
					//rook is moving to the right
					while (1)
					{
						j++;
						if (j > 7) return -1;
						if (j == n) return 1;
						if (board[i][j]->getPlayerNum() != 2)
							return -1;
					}
				}

			}
			if (j == n)
			{
				//horizontal axis is the same
				if (i > m)
				{
					//rook is moving down
					while (1)
					{
						i--;
						if (i < 0) return -1;
						if (i == m) return 1;
						if (board[i][j]->getPlayerNum() !=2)
							return -1;
					}
				}
				else if (i < m)
				{
					//rook is moving up
					while(1)
					{
					i++;
					if (i > 7) return -1;
					if (j == m) return 1;
					if (i == m) return 1;
					if (board[i][j]->getPlayerNum() != 2)
						return -1;
					}
				}
			}

			if (i > m){
							if (j > n)
							{
								while (1)
								{
									i--;
									j--;
									if (i < 0 || i < 0) return -1;
									if (i == m && j == n) return 1;
									if (board[i][j]->getPlayerNum() != 2) return -1;
								}
								//moving down/left
							}
							else if (j < n)
								//moving down/right
								while(1)
								{
									i--;
									j++;
									if (i < 0 || j > 7) return -1;
									if (i == m && j == n) return 1;
									if (board[i][j]->getPlayerNum() != 2) return -1;
								}}
			else if (i < m){
							if (j > n)
								//moving up/left
								while(1)
								{
									i++;
									j--;
									if (i > 7 || j < 0) return -1;
									if (i == m && j == n) return 1;
									if (board[i][j]->getPlayerNum() != 2) return -1;
								}
							else if (j < n)
								//moving up/right
								while (1)
								{
									std::cerr<<"WEE ";
									i++;
									j++;
									std::cerr<<i<<","<<j;
									if (i > 7 || j > 7) return -1;
									if (i == m && j == n) return 1;
									if (board[i][j]->getPlayerNum() != 2) return -1;
								}}

			return -1;
		}		

		virtual vector<int> getThreats(int i, int j, Piece *board[8][8])
		{
			vector<int> threats;


			bool trick = false;
			//rook moves up, increases i
			for (int m = i+1; m < 8; ++m)
			{
				if (!trick)
				{
					if (board[m][j]->getPlayerNum() == 2)
					{
						//threats.push_back(loc1d(m,j));
						addThreat(m,j,&threats);
					}
					else if (board[m][j]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,j));
						addThreat(m,j,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}
			//rook moves down decreases i
			trick = false;
			for (int m = i-1; m >= 0; --m)
			{
				if (!trick)
				{
					if (board[m][j]->getPlayerNum() == 2)
						//threats.push_back(loc1d(m,j));
						addThreat(m,j,&threats);
					else if (board[m][j]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,j));
						addThreat(m,j,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}
			//rook moves left decreases j
			trick = false;
			for (int n = j-1; n >= 0; n--)
			{
				if (!trick)
				{
					if (board[i][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(i,n));
						addThreat(i,n,&threats);
					else if (board[i][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
					//	threats.push_back(loc1d(i,n));
						addThreat(i,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}
			//rook moves right increases j
			trick = false;
			for (int n = j+1; n < 8 ; n++)
			{
				if (!trick)
				{
					if (board[i][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(i,n));
						addThreat(i,n,&threats);
					else if (board[i][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(i,n));
						addThreat(i,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}


			trick = false;
			int m = i;  //m = 6
			int n = j;  //n = 3
			//moving up/left
			while (m < 7 && n > 0)
			{
				m++; //m = 7
				n--; //n = 2
				if (!trick)
				{
					if (board[m][n]->getPlayerNum() == 2)
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
					}
					else if (board[m][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
						trick = true;
					}
					else
					{
						trick = true;
					}
				}
			}

			//moving down/left
			trick = false;
			m = i;
			n = j;

			while (m > 0 && n > 0)
			{
				m--;
				n--;
				if (!trick)
				{
					if (board[m][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
					else if (board[m][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}			

			//moving up/right
			trick = false;
			m = i;
			n = j;

			while ((m < 7) && (n <7) )
			{
				m++;
				n++;
				if (!trick)
				{
					if (board[m][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
					else if (board[m][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}		
			//moving down/right

			trick = false;
			m = i;
			n = j;

			while (m > 0 && n < 7)
			{
				m--;
				n++;
				if (!trick)
				{
					if (board[m][n]->getPlayerNum() == 2)
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
					else if (board[m][n]->getPlayerNum() != board[i][j]->getPlayerNum())
					{
						//threats.push_back(loc1d(m,n));
						addThreat(m,n,&threats);
						trick = true;
					}
					else
						trick = true;
				}
			}		


			return threats;
		}
};

class King : public Piece
{
	public:
		King(int p) : Piece(p)
		{
			name = "Ki";
			setN();
			iKing = true;
		}

		virtual int canMove(int i, int j, int m, int n, Piece *board[8][8])
		{
			if (moved == false)
			{//WARNING I THINK CASTLING IS BROKEN IF A PAWN GETS PROMOTED TO A SPOT WHERE IT COULD CASTLE IS THIS ALLOWED?
				if (board[i][7]->moved == false) //is kings side rook
					if (board[i][6]->getPlayerNum() == 2)
						if (board[i][5]->getPlayerNum() == 2)
							if (m == i && n == 6)
								return 2;//2 is a special castling number
				if (board[i][0]->moved == false) //queens side rook
					if (board[i][3]->getPlayerNum() == 2)
						if (board[i][2]->getPlayerNum() == 2)
							if (board[i][1]->getPlayerNum() == 2)
								if (m == i && n == 2)
									return 2;
			}

			if (i+1 == m && j+1 == n) return 1;
			if (i == m && j+1 == n) return 1;
			if (i-1 == m && j+1 == n) return 1;
			if (i-1 == m && j == n) return 1;
			if (i-1 == m && j-1 == n) return 1;
			if (i == m && j-1 == n) return 1;
			if (i+1 == m && j-1 == n) return 1;
			if (i+1 == m && j == n) return 1;
			return -1;
		}

		virtual vector<int> getThreats(int i, int j, Piece *board[8][8])
		{
			vector<int> threats;

			//I don't think the threat of a castle is something I need to draw, at least not right now, in prep for graphics yes

			if (i+1 < 8 && j+1 < 8)
			if (board[i+1][j+1]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i+1,j+1));
				addThreat(i+1,j+1,&threats);
			if (j+1 < 8)
			if (board[i][j+1]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i,j+1));
				addThreat(i,j+1,&threats);
			if (i-1 >= 0 && j+1 < 8)
			if (board[i-1][j+1]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i-1,j+1));
				addThreat(i-1,j+1,&threats);
			if (i-1 >= 0)
			if (board[i-1][j]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i-1,j));
				addThreat(i-1,j,&threats);
			if (i-1 >= 0 && j-1 >= 0)
			if (board[i-1][j-1]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i-1,j-1));
				addThreat(i-1,j-1,&threats);
			if (j-1 >= 0)
			if (board[i][j-1]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i,j-1));
				addThreat(i,j-1,&threats);
			if (i+1 < 8 && j-1 >= 0)
			if (board[i+1][j-1]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i+1,j-1));
				addThreat(i+1,j-1,&threats);
			if (i+1 < 8)
			if (board[i+1][j]->getPlayerNum() != board[i][j]->getPlayerNum())
				//threats.push_back(loc1d(i+1,j));
				addThreat(i+1,j,&threats);

			return threats;
		}
};

class Empty : public Piece
{
	public:
		Empty(int p) : Piece(p)
		{
			name = "  ";
			setN();
			iKing = false;
			moved = true; //to sidestep issues with trying to castle with empty spaces
		}

		virtual int canMove(int i, int j, int m, int n, Piece *board[8][8])
		{
			std::cerr<<"EMPTY PIECE SHOULD NEVER BE MOVED ERROR ERROR"<<std::endl;
			return -1;
		}

		virtual vector<int> getThreats(int i, int j, Piece *board[8][8])
		{
			vector<int> threats;

			return threats;
		}
};

#endif