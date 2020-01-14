#include "cells.hpp"

const size_t inputs_n = 1;
const size_t overwrite_offs = 1;
const size_t cells_n = 6;
const size_t trans_n = 0;

fnn_network_t net{inputs_n, overwrite_offs, cells_n, trans_n, 5 };

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> rand_test(0.0f, 1.0f );

auto mycost = [](float sample)->diffbl
{
	float expected = sqrt(sample);
	return expected-net.inputs[overwrite_offs];
};

auto my_get_sample = []()->float
{
	return rand_test(gen);
};

auto my_restore_sample = [](float sample)
{
	net.inputs[0].val = sample;
	for(size_t i=1; i < 6; i++ )
		net.inputs[i].val = 0;
};

int main()
{
	net.init();
	net.gen_random_weights();
	net.train(0, mycost, 0.3, 0.9999, 100000, 5000, true, my_get_sample, my_restore_sample );
	while(true)
	{
		float sample;
		std::cin >> sample;
		my_restore_sample(sample);
		size_t temp[2] = {0,0}, len[2] = {3*cells_n,inputs_n+cells_n-trans_n+2};
		gen_diffbl_vec(net.weights_diffbl, net.weights, temp, len );
		for(size_t i=0; i < 5; i++ )
		{
			net.evolve();
			std::cout << net.inputs[overwrite_offs].val << std::endl;
		}
		std::cout << "-------" << std::endl;
	}
}
