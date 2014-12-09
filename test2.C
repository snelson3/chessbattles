#ifndef ONE
#define ONE

#include <iostream>
#include "test.C"

class B
{
public:
	B(A *a)
	{
		std::cerr<<"test "<<3+a->i;
	}
};

#endif