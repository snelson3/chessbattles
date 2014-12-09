#ifndef TWO
#define TWO


#include <iostream>
#include "test2.C"

class A
{
public:
	int i;
	A(void)
	{
		i=5;
		B b = new B(this);
	}
};

int main()
{
	A a;
	return 1;
}

#endif