//#include <iostream>
#include "desc.hpp"

/*float weights[3] = {0.1, 0.6, 0.3 };
diffbl weights_diffbl[2];
diffbl values[3] = {diffbl{7,0}, diffbl{40,0}, diffbl{23,0} };
float d_weights[2];*/

float vec_dist(float *src, size_t len )
{
	float retval=0;
	for(size_t i=0; i < len; i++ )
	{ retval += src[i]*src[i]; }
	return sqrt(retval);
}

float vec_sum(float *src, size_t len )
{
	float retval=0;
	for(size_t i=0; i < len; i++ )
	{ retval += fabs(src[i]); }
	return retval;
}

void vec_mul(float *src, float f, size_t len )
{
	for(size_t i=0; i < len; i++ )
	{ src[i]=fabs(src[i]*f); }
}

void descent(float target, float current, float step, float *dest, float *src, size_t len )
{
	float dist = vec_dist(src, len ) * ( (target > current)-(target < current) );
	if(dist != 0) for(size_t i=0; i < len; i++ )
	{ dest[i] += step*src[i]/dist; }
	/*float sum = vec_sum(dest, len );
	vec_mul(dest, 1/sum, len );*/
}

void normalize(float *dest, size_t len )
{
	float sum = vec_sum(dest, len );
	vec_mul(dest, 1/sum, len );
}

void normalize(float *dest, size_t len[2] )
{
	for(size_t i=0; i < len[0]*len[1]; i+=len[1] )
		normalize(dest+i, len[1] );
}

/*int main()
{
	std::cout << weights[0] << ' ' << weights[1] << std::endl;
	std::cout << d_weights[0] << ' ' << d_weights[1] << std::endl;
	std::cout << "----------------------" << std::endl;
	for(int i=0; i < 50; i++ )
	{
		float guess;
		for(size_t i=0; i < 3; i++ )
		{
			gen_diffbl_vec(weights_diffbl, weights, i, 3 );
			diffbl res = scalar_prod<diffbl, diffbl{0,0} >(weights_diffbl, values, 3 );
			guess = res.val; d_weights[i] = res.der;
		}
		std::cout << "guess: " << guess << std::endl;
		descent(23.5, guess, 0.01, weights, d_weights, 3 );
	}
	std::cout << "----------------------" << std::endl;
	std::cout << d_weights[0] << ' ' << d_weights[1] << ' ' << d_weights[2] << std::endl;
	std::cout << weights[0] << ' ' << weights[1] << ' ' << weights[2] << std::endl;
	return 0;
}*/
