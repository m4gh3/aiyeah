#include <iostream>
#include "diffbl.hpp"

diffbl diffbl::operator+(diffbl d)
{
	diffbl retval;
	retval.val=val+d.val;
	retval.der=der+d.der;
	return retval;
}

diffbl diffbl::operator+(float f)
{
	diffbl retval{val+f, der };
	return retval;
}

diffbl diffbl::operator-(float f)
{
	diffbl retval{val-f, der };
	return retval;
}

diffbl diffbl::operator*(float f)
{
	diffbl retval;
	retval.val=val*f;
	retval.der=der*f;
	return retval;
}

diffbl diffbl::operator*(diffbl d)
{
	diffbl retval;
	retval.val=val*d.val;
	retval.der=val*d.der+der*d.val;
	return retval;
}

diffbl diffbl::operator-()
{ return (*this)*(-1); }

diffbl diffbl::operator-(diffbl d)
{ return (-d)+*this; }

diffbl diffbl::operator/(float f)
{ return (*this)*(1/f); }

diffbl operator+(float f, diffbl d )
{ return d+f; }

diffbl operator*(float f, diffbl d )
{ return d*f; }

diffbl operator-(float f, diffbl d )
{ return -(d-f); }

diffbl operator/(float f, diffbl d )
{ return diffbl{f/d.val, f*(-d.der/(d.val*d.val)) }; }

diffbl diffbl::operator/(diffbl d)
{ return (*this)*(1/d); }

diffbl &diffbl::operator+=(diffbl d)
{ return (*this)=(*this)+d; }

/*int main()
{
	diffbl a{10,1}, b=a;
	for(int i=0; i < 50; i++ )
		b = 0.5*(b+a/b);
	std::cout << b.val << ' ' <<b.der << std::endl;
}*/
