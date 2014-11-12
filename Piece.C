#ifndef PIECE
#define PIECE
#include <stdio.h>
#include <iostream>

class Piece
{
	public:
		int player;
		char n[3];
		const char *name;

		Piece(int p)
		{
			player = p;
			
		}

		void setN()
		{
			if (player == 0)
				sprintf(n,"w%s",name);
			else if (player == 1)
				sprintf(n,"b%s",name);
			else sprintf(n," %s",name);
		}

		const char *getPlayer(){if (player == 0) return "White"; if (player == 1) return "Black"; return "NA";}
};

class Pawn : public Piece
{
	public:
		Pawn(int p) : Piece(p)
		{
			name = "Pa";
			setN();
		}
};

class Rook : public Piece
{
	public:
		Rook(int p) : Piece(p)
		{
			name = "Ro";
			setN();
		}
};

class Knight : public Piece
{
	public: 
		Knight(int p) : Piece(p)
		{
			name = "Kn";
			setN();
		}
};

class Bishop : public Piece
{
	public:
		Bishop(int p) : Piece(p)
		{
			name = "Bi";
			setN();
		}
};

class Queen : public Piece 
{
	public:
		Queen(int p) : Piece(p)
		{
			name = "Qu";
			setN();
		}
};

class King : public Piece
{
	public:
		King(int p) : Piece(p)
		{
			name = "Ki";
			setN();
		}
};

class Empty : public Piece
{
	public:
		Empty(int p) : Piece(p)
		{
			name = "  ";
			setN();
		}
};

#endif