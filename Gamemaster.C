#include "Board.C"
#include <iostream>
#include <errno.h>
#include "Display.C"
#include "Display_cmd.C"
#include "Display_gui.C"

class Gamemaster
{
	public:
		Board *board;
		Display *display;
		bool checkmate;
		int p[2];
		int t;

		Gamemaster(int di)
		{
			checkmate = false;
			p[0] = 0;
			p[1] = 1;
			t = 0;
			board = new Board();
			if (di == 1)
				display = new Display_cmd(board);
			else if (di == 2)
				display = new Display_gui(board);
		}

		const char* getPlayer(void) {if (p[t] == 0) return "White"; if (p[t] == 1) return "Black";
									return "Err";}

		int getTurn(void){//this is weird
			if(p[t] == 0) return 0; if (p[t] == 1) return 1; perror("getTurn err");return 0;}

		

		int checkBounds(int i)
		{
			//Checks that number is withim 0 and 7
			if (i > 7 || i < 0) return 1;
			else return 0;
		}

		int makeMove(void)
		{
			//board.displayBoard();
			display->update();
			std::cout<<"It is currently "<<getPlayer()<<"'s turn"<<endl;

			std::cout<<endl<<"What move do you want to make ";
			int i, j, m, n;

			/****************
			***BEgin something that should go in it's own function
			*****************/

			//cin a letter and then a number corresponding to a chessboard
			//convert to i/j (to access a spot in a 1d array but its a 2d array)

			display->getMove(&i,&j,&m,&n);

			//will need to figure out some tests for valid input
			// char x, s; int y, t;
			// std::cout<<endl<<"Starting X ";
			// std::cin>>x;
			// std::cout<<"   Starting Y ";
			// std::cin>>y;
			// std::cout<<endl<<"Ending X ";
			// std::cin>>s;
			// std::cout<<" Ending Y ";
			// std::cin>>t;

			// j = parseNum(x);
			// i = y-1;
			// n = parseNum(s);
			// m = t-1;

			/******************
			***End something that should go in it's own function
			*******************/

			if (checkBounds(j) || checkBounds(i) || checkBounds(n) || checkBounds(m) )
			{
				std::cout<<"INPUT OUT OF RANGE OF CHESSBOARD"<<endl;
				return -1;
			}

			std::cout<<endl<<"Fetching piece at "<<i<<","<<j;
			std::cout<<endl<<board.getPiece(i,j)->getPlayer()<<" "<<board.getPiece(i,j)->getName();
			std::cout<<endl<<"Moving to piece at "<<m<<","<<n;
			std::cout<<endl<<board.getPiece(m,n)->getPlayer()<<" "<<board.getPiece(m,n)->getName();
			//probably gonna need a way to garbage collect the pieces




			if (board.getPiece(i,j)->getPlayerNum() != getTurn())
			{

				std::cout<<"THATS NOT YOUR PIECE!!!"<<endl;
				// std::cout<<i<<","<<j<<endl;
				// std::cout<<board.getPiece(i,j)->getName();
				// std::cout<<endl<<board.getPiece(i,j)->getPlayerNum();
				// std::cout<<endl<<p[t];
				// std::cout<<endl<<t<<endl;
				// std::cout<<getPlayer();
				// std::cout<<endl<<getTurn();
				return -1;
			}

			if (board.getPiece(i,j) == board.getPiece(m,n))
			{
				std::cout<<"MUST MOVE A PIECE!!!!"<<endl;
				return -1;
			}

			int mval = board.getPiece(i,j)->canMove(i,j,m,n,board.board);


			if (board.getPiece(m,n)->getPlayerNum() == getTurn() && mval != 2)
			{
				std::cout<<"CANT TAKE YOUR OWN PIECE!!!"<<endl;
				return -1;
			}


			if (mval == -1)
			{
				std::cout<<"THAT PIECE CANT MOVE THERE!!!"<<endl;
				return -1;
			}

			else if (mval == 2)
			{
				std::cerr<<"MVAL IS 2";
				//do a check for if theres check, maybe undo it if so


				if (j > n) //queens side rook
				{
					board.board[i][j-2] = board.board[i][j];
					board.board[i][j-1] = board.board[m][n];
					board.board[i][j] = new Empty(2);
					board.board[m][n] = new Empty(2);
					if(board.isCheck(getTurn()))
						{
						board.board[i][j] = board.board[i][j-2];
						board.board[m][n] = board.board[i][j-1];
						board.board[i][j-2] = new Empty(2);
						board.board[i][j-1] = new Empty(2);
						std::cerr<<"Your king would be in check!\n";
						return -1;
						}	
				}
				else //kings side rook
				{
					board.board[i][j+2] = board.board[i][j];
					board.board[i][j+1] = board.board[m][n];
					board.board[i][j] = new Empty(2);
					board.board[i][j] = new Empty(2);
					if(board.isCheck(getTurn()))
						{
						board.board[i][j] = board.board[i][j+2];
						board.board[m][n] = board.board[i][j+1];
						board.board[i][j-2] = new Empty(2);
						board.board[i][j-1] = new Empty(2);
						std::cerr<<"Your king would be in check!"<<endl;
						return -1;
						}	
				}
				std::cout<<"CASTLED";
			}
			else if (mval == 3)
			{
				std::cerr<<"MVAL is 3";
				//pawn leveling up
				if (m == 7 || m == 0)//this works since pawns only go one way
				{
					//function pointers would clean this up a lot i think

					//do a check for if theres check now

					board.board[i][j] = new Empty(2);
					board.board[m][n] = new Pawn(t);

					if (board.isCheck(t))
					{
						std::cerr<<"Your king would be in check!";
						board.board[i][j] = new Pawn(t);
						board.board[m][n] = new Empty(2);
						return -1;
					}

					int i = 0;
					std::cout<<"CONGRATS, your pawn is ready to evolve!!";
					std::cout<<"\ndo you want your digimon to divivolve into...\n";
					std::cout<<"a queen? (press 1) ";
					std::cin>>i;
					if (i != 1)
					{
						std::cout<<"\na rook? ";
						std::cin>>i;
						if (i != 1)
						{
							std::cout<<"\n 1 for Knight or else bishop ";
							std::cin>>i;
							if (i == 1)
								board.board[m][n] = new Knight(t);
							else
								board.board[m][n] = new Bishop(t);
						}
						else
						{
							board.board[m][n] = new Rook(t);
						}
					}
					else
					{
						board.board[m][n] = new Queen(t);
					}

					// board.board[i][j] = new Empty(2);
					// board.board[m][n] = temp;
				}
				else mval = 1; //just like a normal piece
				std::cerr<<"THIS IS BORING MAMA";
			}
			if (mval == 1)
			{

				//do a check for check
				
				//normal movement
				std::cerr<<"I guess not";
				board.board[m][n] = board.board[i][j];
				board.board[i][j] = new Empty(2);
				std::cerr<<"Gonna check for check";
				if (board.isCheck(getTurn()))
				{
					board.board[i][j] = board.board[m][n];
					board.board[m][n] = new Empty(2);
					std::cerr<<"Your king would be in check!";
					return -1;
				}
			}
			else
			{
				std::cerr<<"SOMETHING WENT WRONG";
				return -1;
			}

			//implement checks/mate

			std::cout<<endl<<"SUCCESS!";

			board.checkThreats(m,n);

			return 1;
		}

		void changeTurn(void)
		{
			//return;
			if (t == 0) t=1;
			else t=0;
			if (t > 1 || t < 0) perror("Wrong turn");
		}

		void isCheckmate()
		{
			//well I don't know how to test for checkmmate, I guess go through every 
			//pieces possible moves and test it and see if they are in checkmmate
			//(only do this if they are in check to start)
			
			std::cerr<<"IT BREAKS BEFORE CHECK RIGHT";
			if (!board.isCheck(t)) return;
			std::cerr<<"NO NOT REALLY";

			std::cerr<<"You are in check!!!!!\n";
			//for each piece owned by t
			  //get a vector of all possible moves
			  //for each of those moves
			    //perform the move on a copy of the board
			    //see if the king is still in check
			    //without more logic it's harder to do better i think

			//this bugs with castling, because the logic isn't coded in
			//Maybe I should make each piece have their own move function.

			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)   
					if (board.board[i][j]->getPlayerNum() == t)
					{
						vector<int> moves = board.board[i][j]->getThreats(i,j,board.board);
  						for(std::vector<int>::iterator mo = moves.begin(); mo != moves.end(); ++mo) 
  						{
  							Piece *t1 = board.board[i][j];
  							int g;
  							int h;
  							t1->loc2d(&g,&h,mo[0]);//might not be indexing right
  							Piece *t2 = board.board[g][h];
  							//make temp square that saves [i][j]
  							//make temp square that saves mo loc
  							board.board[g][h] = board.board[i][j];
  							board.board[i][j] = new Empty(2);
  							if (!board.isCheck(t))
  							{
  								//I have to reset it, then return
  								board.board[g][h] = t2;
  								board.board[i][j] = t1;
  								return;
  							}
  							board.board[g][h] = t2;
  							board.board[i][j] = t1;

//I didn't finish coding the checkmate function.

  							//move [i][j] to mo
  							//see if its check
  							  //if its not return
  							//else reset the board back
  						}
					}

			//if it hasn't returned yet it must be checkmate

			checkmate = true;
		
		}
};