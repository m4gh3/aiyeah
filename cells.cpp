#include "cells.hpp"

void fnn_network_t::init()
{
	weights_n = cells_n*3*(cells_n+inputs_n+2);
	weights = new float[weights_n];
	weights_diffbl = new diffbl[weights_n];
	weights_der = new float[weights_n];
	inputs = new diffbl[inputs_n+cells_n-trans_n+2];
	for(size_t i=0; i < inputs_n+cells_n-trans_n; i++ )
		inputs[i] = diffbl{0,0};
	inputs[inputs_n+cells_n-trans_n] = diffbl{1,0};
	inputs[inputs_n+cells_n-trans_n+1] = diffbl{0,0};
	outputs = new diffbl[3*cells_n];
}

void fnn_network_t::gen_random_weights()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> rand_f(0.0f, 1.0f );

	size_t len[2] = {3*cells_n, inputs_n+cells_n-trans_n+2 };

	for(size_t i=0; i < weights_n; i+=cells_n+inputs_n+2 )
	{
		for(size_t j=0; j < cells_n+inputs_n+2; j++ )
		{
			weights[i+j] = rand_f(gen);
			weights[i+j] += 0.05;
		}
	}
	normalize(weights, len );
}

void fnn_network_t::evolve()
{
	size_t len[2]={3*cells_n,inputs_n+cells_n-trans_n+2};
	scalar_prod<diffbl>(outputs, weights_diffbl, inputs, diffbl{0,0}, len );
	for(size_t k=0, l=0; k < cells_n*3; l++,k+=3 )
		inputs[overwrite_offs+l] = (1-outputs[k+2])*outputs[k]+outputs[k+2]*outputs[k+1];
}
