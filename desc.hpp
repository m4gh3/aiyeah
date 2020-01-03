#include <cmath>
#include <cstddef>
#include "diffbl.hpp"	//auto differentiable type
			//calculate automatically derivative with respect to a variable
			//used for the gradient descent

//generate automatically derivative
inline diffbl gen_diffbl(float val, size_t i, size_t j )
{
	return i==j ? ( val > 0 ? diffbl{val,1-val} : ( val == 0 ? diffbl{val,1} : diffbl{val,val-1} ) )
		: ( val > 0 ? diffbl{val,-val} : diffbl{val,val} );
}

inline diffbl gen_diffbl(float val, size_t i[2], size_t j[2] )
{ return i[0]!=j[0] ? diffbl{val,0} : gen_diffbl(val, i[1], j[1] ); }

inline void gen_diffbl_vec(diffbl *dest, float *src, size_t j, size_t len )
{
	for(size_t i=0; i < len; i++ )
		dest[i] = gen_diffbl(src[i], i, j );
}

inline void gen_diffbl_vec(diffbl *dest, float *src, size_t j[2], size_t len[2] )
{
	for(size_t i=0,l=0; i < len[0]*len[1]; l++,i+=len[1] )
		for(size_t k=0; k < len[1]; k++ )
		{
			size_t temp[2] = {l,k};
			dest[i+k] = gen_diffbl(src[i+k], temp, j );
		}
}	


template<typename scalar_tt> scalar_tt scalar_prod(scalar_tt *v0, scalar_tt *v1, scalar_tt zero, size_t len )
{
	scalar_tt retval = zero;
	for(size_t i=0; i < len; i++ )
	{ retval += v1[i]*v0[i]; }
	return retval;
}

template<typename scalar_tt> void scalar_prod( scalar_tt *dest, scalar_tt *v0, scalar_tt *v1, scalar_tt zero, size_t len[2] )
{
	for(size_t i=0; i < len[0]; i++ )
	{ dest[i] = scalar_prod<scalar_tt>( v0+(len[1]*i), v1, zero, len[1]  ); }
}

float vec_dist(float *src, size_t dest );
float vec_sum(float *src, float f, size_t len );
void vec_mul(float *src, float f, size_t len );
void descent(float target, float current, float step, float *dest, float *src, size_t len );
void normalize(float *dest, size_t len );
void normalize(float *dest, size_t len[2] );
