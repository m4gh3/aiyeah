#ifndef _DIFFBL_
#define _DIFFBL_

struct diffbl
{
	float val;
	float der;

	diffbl operator+(diffbl d);	
	diffbl operator+(float f);	
	diffbl operator-(float f);	
	diffbl operator*(float f);
	diffbl operator*(diffbl d);
	diffbl operator-();
	diffbl operator-(diffbl d);
	diffbl operator/(float f);
	diffbl operator/(diffbl d);

	diffbl &operator+=(diffbl d);

};

diffbl operator+(float f, diffbl d );
diffbl operator*(float f, diffbl d );
diffbl operator-(float f, diffbl d );
diffbl operator/(float f, diffbl d );

#endif
