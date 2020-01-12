#include <iostream>
#include <random>
#include <cstdlib>
#include "desc.hpp"

struct fnn_network_t
{
	size_t inputs_n;
	size_t overwrite_offs;
	size_t cells_n;
	size_t trans_n;
	size_t evolution_cycles;
	size_t weights_n;
	float *weights;
	diffbl *weights_diffbl;
	float *weights_der;
	diffbl *inputs;
	diffbl *outputs;
	void init();
	void gen_random_weights();
	void evolve();
	template<class lambda0, class lambda1, class lambda2 > void train(float target, lambda0 cost, float step, float decay, size_t iterations, size_t show_progress_freq, bool show_weights, lambda1 get_sample, lambda2 restore_sample );
};

template<class lambda0, class lambda1, class lambda2 > void fnn_network_t::train(float target, lambda0 cost, float step, float decay, size_t iterations, size_t show_progress_freq, bool show_weights, lambda1 get_sample, lambda2 restore_sample )
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
				if(std::isnan(inputs[overwrite_offs].val))
				{
					std::cout << "Got nan around iteration " << test << '/' << iterations << "\nTraining aborted. :( " << std::endl;
					std::exit(EXIT_FAILURE);
				}
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

