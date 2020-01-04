#include <iostream>
#include <random>
#include "desc.hpp"

const size_t inputs_n = 2;
const size_t overwrite_offs = 0;
const size_t cells_n = 1;
const size_t trans_n = 0;
const size_t weights_n = cells_n*3*(cells_n+inputs_n+2);

float weights[weights_n] = {	0.75, 0.25, 0.0, 0.0, 0.0,
				0.05, 0.9,  0.0, 0.05, 0.0,
				0.05, 0.05, 0.0, 0.9, 0.0 };

diffbl weights_diffbl[weights_n]; //they should be entered here
float weights_der[weights_n];
diffbl inputs[inputs_n+cells_n-trans_n+2]={ diffbl{0,0}, diffbl{0,0}, diffbl{0,0}, diffbl{1,0}, diffbl{0,0} };
diffbl outputs[3*cells_n];

void evolve()
{
	size_t len[2]={3*cells_n,inputs_n+cells_n-trans_n+2};
	scalar_prod<diffbl>(outputs, weights_diffbl, inputs, diffbl{0,0}, len );
	for(size_t k=0; k < cells_n*3; k+=3 )
		inputs[overwrite_offs+k] = (1-outputs[k+2])*outputs[k]+outputs[k+2]*outputs[k+1];
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> rand_test(0.0f, 1.0f );

int main()
{
	//float guess;
	float step = 0.1;
	for(size_t test=0; test < 40000; test++ )
	{
		float test_value = rand_test(gen);
		float test_value1 = rand_test(gen);
		diffbl output;
		size_t len[2] = {3,inputs_n+cells_n-trans_n+2};
		for(size_t i=0,k=0; i < len[0]*len[1]; k++,i+=len[1] )
			for(size_t j=0; j < len[1]; j++ )
			{
				inputs[0].val = test_value;
				inputs[1].val = test_value1;
				size_t temp[2] = {k,j};
				gen_diffbl_vec(weights_diffbl, weights, temp, len );
				evolve();
				weights_der[i+j] = inputs[0].der;
			}
		float target=(1-test_value)*test_value1;
		descent(target, inputs[0].val, step, weights, weights_der, len[0]*len[1] );
		normalize(weights, len );
		std::cout << "guess: " << inputs[0].val << " target: " << target << " step: " << step << std::endl << "weights_der: ";
		for(size_t i=0; i < len[0]*len[1]; i++ )
			std::cout << weights_der[i] << ' ';
		std::cout << std::endl << "weights:\t";
		for(size_t i=0; i < len[0]*len[1]; i+=len[1] )
		{
			for(size_t j=0; j < len[1]; j++ )
				std::cout << weights[i+j] << ' ';
			std::cout << std::endl << "\t\t" ;
		}
		std::cout << std::endl << "-----------------" << std::endl;
		step*=0.99999;
	}

}