#include <iostream>



class A //A is a piece
{
public:
	int a;

	A(void){a = 1;}
};

class B //B is a board holding a pointer of A
{
public:
	A *x;
	B(void){x = new A();}
};

class D //D is a display holding a pointer to B trying to read a
{
public:
	B *s;
	D(B *b){s = b;}
};

class C //C is a gamemaster holding an instance of B
{
public:
	B b;
	D *d;
	C(void){d = new D(&b);}
};

int main(void){
	C c;
	std::cerr<<c.b.x->a<<" should be 1\n";
	std::cerr<<c.d->s->x->a<<" should also be 1\n";
	c.b.x->a = 5;
	std::cerr<<c.b.x->a<<" should now be 5\n";
	std::cerr<<c.d->s->x->a<<" should now also be 5\n";

}