SRC=main.C Board.C Piece.C
OBJ=$(SRC:.C=.o)

prog: $(OBJ)
	g++ $(OBJ) -o chess

.C.o: $<
	g++ -I. -c $<

clean:
	rm *.o chess
