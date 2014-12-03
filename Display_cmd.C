#include "Display.C"
#include "Board.C"

class Display_cmd : public Display
{
	public:
		Board *b;
		Display_cmd(Board *board) : Display(){b = board;}
	
		int parseNum(char a)
		{
			int i;
			if (a == 'a') i = 0;
			else if (a == 'b') i = 1;
			else if (a == 'c') i = 2;
			else if (a == 'd') i = 3;
			else if (a == 'e') i = 4;
			else if (a == 'f') i = 5;
			else if (a == 'g') i = 6;
			else if (a == 'h') i = 7;
			else i = -1;

			return i;
		}

		virtual void update(){
			std::cerr<<b->getPiece(0,0)->n<<"DONE";
			std::cout<<"########################################################" << endl;
			std::cout<<"########################################################" << endl;
			std::cout<<"########################################################" << endl;
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
					std::cout<<"# "<<b->board[7-i][j]->n<<" #";
				std::cout<<endl<<"########################################################" << endl;
			}

		}

		virtual void getMove(int *i, int *j, int *m, int *n)
		{
		//will need to figure out some tests for valid input
			char x, s; int y, t;
			std::cout<<endl<<"Starting X ";
			std::cin>>x;
			std::cout<<"   Starting Y ";
			std::cin>>y;
			std::cout<<endl<<"Ending X ";
			std::cin>>s;
			std::cout<<" Ending Y ";
			std::cin>>t;

			*j = parseNum(x);
			*i = y-1;
			*n = parseNum(s);
			*m = t-1;
		}
};