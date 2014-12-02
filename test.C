#include <iostream>



		void loc2d(int *i, int *j, int m)
		{
			//convert m into an i and a j
			//have to modulo m which is i, then do some multiplication and subtraction for j
			*i = m / 8;
			*j = m - (*i*8);
		}
		

int main(void){
	int i;
	int j;
	loc2d(&i,&j,42);
	std::cerr<<"5 2 "<<i<<" "<<j<<std::endl;
	loc2d(&i,&j,2);
	std::cerr<<"0 2 "<<i<<" "<<j<<std::endl;

}