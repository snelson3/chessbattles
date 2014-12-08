#include <iostream>
#include "Gamemaster.C"

using std::endl;

void linebreak(){
	std::cout<<"########################################################" << endl;
};

void newgame(int display){
	Gamemaster gm = Gamemaster(display);

	//gm.board.displayBoard();
	//gm.display->update();

	while (gm.checkmate == false && gm.stalemate == false)
	{
		while (gm.makeMove() == -1) ;
		gm.changeTurn(); //have the player turns be in a length 2 array so you can just -1
		gm.isCheckmate();
	}
	if (gm.checkmate == true)
		std::cout<<"Checkmate\n"<<endl;
	else if (gm.stalemate == true)
		std::cout<<"Stalemate\n"<<endl;
	else 
		std::cout<<"The game broke\n";
};

int main() {
	int i;
	linebreak();
	std::cout<<"Hello Welcome to Chess Battles/n" << endl;
	std::cout<<"Press 1 to start a new game in the command line" << endl;
	std::cout<<"Press 2 to start a new game with the VTK Gui" << endl;
	std::cout<<"Press 0 to exit" << endl;
	linebreak();
	std::cout<<"Value ";
	//std::cin>>i;
	i = 2;//remove this at some point
	if (i == 1)
		newgame(1);
	if (i == 2)
		newgame(2);
	linebreak();
	std::cout<<"Thank you so much for a playing my game"<<endl;
};