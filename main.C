#include <iostream>
#include "Piece.C"
#include "Board.C"

using std::endl;

void linebreak(){
	std::cout<<"########################################################" << endl;
};

void newgame(){
	Board board = Board();
	//Gamemaster gm;

	board.displayBoard();



	// while (gm.isCheckmate == false)
	// {
	// 	Move move = acceptmove(gm.activeplayer);
	// 	if (gm.isValid(move))
	// 		{
	// 			gm.makemove(move);
	// 			gm.setTurn(); //have the player turns be in a length 2 array so you can just -1
	// 			gm.isCheckmate();
	// 		}
	// }
	std::cout<<"Checkmate"<<endl;
};

int main() {
	int i;
	linebreak();
	std::cout<<"Hello Welcome to Chess Battles/n" << endl;
	std::cout<<"Press 1 to start a new game" << endl;
	//std::cout<<"Press 0 to start a new game as black" << endl;
	std::cout<<"Press 0 to exit" << endl;
	linebreak();
	std::cout<<"Value ";
	std::cin>>i;
	if (i == 1)
		newgame();
	linebreak();
	std::cout<<"Thank you so much for a playing my game"<<endl;
};