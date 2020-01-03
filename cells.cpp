#include <iostream>
#include <random>
#include "desc.hpp"

float weights[12] = { 	0.75, 0.25, 0.0, 0.0,
			0.05, 0.9,  0.0, 0.05,
			0.05, 0.05, 0.0, 0.9 };
diffbl weights_diffbl[12]; //they should be entered here
float weights_der[12];
diffbl inputs[4]={ diffbl{0,0}, diffbl{0,0}, diffbl{1,0}, diffbl{0,0} };
diffbl outputs[3];

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> rand_test(0.0f, 1.0f );

int main()
{
	//float guess;
	float step = 0.1;
	for(size_t test=0; test < 30000; test++ )
	{
		float test_value = rand_test(gen);
		float test_value1 = rand_test(gen);
		diffbl output;
		size_t len[2] = {3,4};
		for(size_t i=0,k=0; i < len[0]*len[1]; k++,i+=len[1] )
			for(size_t j=0; j < len[1]; j++ )
			{
				inputs[0].val = test_value;
				inputs[1].val = test_value1;
				size_t temp[2] = {k,j};
				gen_diffbl_vec(weights_diffbl, weights, temp, len );
				scalar_prod<diffbl>(outputs, weights_diffbl, inputs, diffbl{0,0}, len );
				output = (1.0-outputs[2])*outputs[0]+outputs[2]*outputs[1];
				weights_der[i+j] = output.der;
			}
		/*for(size_t i=0; i < 3; i++ )
			std::cout << outputs[i].val << ' ' << outputs[i].der << std::endl;
		std::cout << "---" << std::endl;*/
		float target=(1-test_value)*test_value1; //1-test_value;
		descent(target, output.val, step, weights, weights_der, len[0]*len[1] );
		normalize(weights, len );
		std::cout << "guess: " << output.val << " target: " << target << "step: " << step << std::endl << "weights_der: ";
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
