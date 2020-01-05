#include <iostream>
#include <random>
#include "desc.hpp"

const size_t inputs_n = 2;
const size_t overwrite_offs = 2;
const size_t cells_n = 2;
const size_t trans_n = 0;
const size_t weights_n = cells_n*3*(cells_n+inputs_n+2);

float weights[weights_n] = {	0.75, 0.25, 0.0, 0.0, 0.0, 0.0,
				0.05, 0.9,  0.0, 0.05, 0.0, 0.0,
				0.05, 0.05, 0.0, 0.9, 0.0, 0.0, 
				0.75, 0.25, 0.0, 0.0, 0.0, 0.0,
				0.05, 0.9,  0.0, 0.05, 0.0, 0.0,
				0.05, 0.05, 0.0, 0.9, 0.0, 0.0 };

diffbl weights_diffbl[weights_n]; //they should be entered here
float weights_der[weights_n];
diffbl inputs[inputs_n+cells_n-trans_n+2]={ diffbl{0,0}, diffbl{0,0}, diffbl{0,0}, diffbl{0,0}, diffbl{1,0}, diffbl{0,0} };
diffbl outputs[3*cells_n];

void evolve()
{
	size_t len[2]={3*cells_n,inputs_n+cells_n-trans_n+2};
	scalar_prod<diffbl>(outputs, weights_diffbl, inputs, diffbl{0,0}, len );
	for(size_t k=0, l=0; k < cells_n*3; l++,k+=3 )
		inputs[overwrite_offs+l] = (1-outputs[k+2])*outputs[k]+outputs[k+2]*outputs[k+1];
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> rand_test(0.0f, 1.0f );

template<class lambda0, class lambda1, class lambda2 > void train(float target, lambda0 cost, float step, float decay, size_t iterations, size_t show_progress_freq, bool show_weights, lambda1 get_sample, lambda2 restore_sample, size_t evolution_cycles )
{
	double avg_err=0;
	for(size_t test=0; test < iterations; test++ )
	{
		diffbl current_cost;
		auto sample = get_sample();
		size_t len[2] = {3*cells_n,inputs_n+cells_n-trans_n+2};
		for(size_t i=0,k=0; i < len[0]*len[1]; k++,i+=len[1] )
			for(size_t j=0; j < len[1]; j++ )
			{
				restore_sample(sample);
				size_t temp[2] = {k,j};
				gen_diffbl_vec(weights_diffbl, weights, temp, len );
				for(size_t ev_cycle_i=0; ev_cycle_i < evolution_cycles; ev_cycle_i++ )
					evolve();
				current_cost = cost(sample);
				weights_der[i+j] = current_cost.der;
			}
		
		descent(target, current_cost.val, step, weights, weights_der, len[0]*len[1] );
		avg_err += abs(current_cost.val);
		normalize(weights, len );
		if(test%show_progress_freq == 0)
		{
			avg_err/=show_progress_freq;
			std::cout << "step: " << step << " avg_err: " << avg_err << std::endl;
			if(show_weights)
			{
				std::cout << "weights_der: ";
				for(size_t i=0; i < len[0]*len[1]; i++ )
					std::cout << weights_der[i] << ' ';
				std::cout << std::endl;
				std::cout << "weights:\t";
				for(size_t i=0; i < len[0]*len[1]; i+=len[1] )
				{
					for(size_t j=0; j < len[1]; j++ )
						std::cout << weights[i+j] << ' ';
					std::cout << std::endl << "\t\t" ;
				}
				std::cout << std::endl << "-----------------" << std::endl;
			}
			avg_err=0;
		}
		step*=decay;
	}
}

struct mysample_t
{ float inputs[2]; };

auto mycost = [](mysample_t sample)->diffbl
{
	float expected = sample.inputs[0] + sample.inputs[1] - 2*sample.inputs[0]*sample.inputs[1];
	return expected-inputs[overwrite_offs];
};

auto my_get_sample = []()->mysample_t
{
	mysample_t retval;
	float test_value = rand_test(gen);
	float test_value1 = rand_test(gen);
	retval.inputs[0] = test_value < 0.5 ? 0.5*test_value : 1-0.5*(1-test_value);
	retval.inputs[1] = test_value1 < 0.5 ? 0.5*test_value1 : 1-0.5*(1-test_value1);
	return retval;
};

auto my_restore_sample = [](mysample_t sample)
{
	inputs[0].val = sample.inputs[0];
	inputs[1].val = sample.inputs[1];
	inputs[2].val = 0;
	inputs[3].val = 0;
};

int main()
{
	train(0, mycost, 0.3, 0.99999, 500000, 5000, false, my_get_sample, my_restore_sample, 2 );
	while(true)
	{
		std::cin >> inputs[0].val;
		std::cin  >> inputs[1].val;
		inputs[2].val=0;
		inputs[3].val=0;
		size_t temp[2] = {0,0}, len[2] = {3*cells_n,inputs_n+cells_n-trans_n+2};
		gen_diffbl_vec(weights_diffbl, weights, temp, len );
		evolve();
		std::cout << inputs[overwrite_offs].val << ' '
		<< inputs[overwrite_offs+1].val << std::endl;
		evolve();
		std::cout << inputs[overwrite_offs].val << std::endl;
	}
}
