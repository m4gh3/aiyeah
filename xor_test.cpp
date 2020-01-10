#include "cells.hpp"

const size_t inputs_n = 2;
const size_t overwrite_offs = 2;
const size_t cells_n = 2;
const size_t trans_n = 0;

fnn_network_t net{inputs_n, overwrite_offs, cells_n, trans_n, 2 };

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> rand_test(0.0f, 1.0f );

struct mysample_t
{ float inputs[2]; };

auto mycost = [](mysample_t sample)->diffbl
{
	float expected = sample.inputs[0] + sample.inputs[1] - 2*sample.inputs[0]*sample.inputs[1];
	return expected-net.inputs[overwrite_offs];
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
	net.inputs[0].val = sample.inputs[0];
	net.inputs[1].val = sample.inputs[1];
	net.inputs[2].val = 0;
	net.inputs[3].val = 0;
};

int main()
{
	net.init();
	net.gen_random_weights();
	net.train(0, mycost, 0.3, 0.99999, 1000000, 5000, true, my_get_sample, my_restore_sample );
	while(true)
	{
		std::cin >> net.inputs[0].val;
		std::cin  >> net.inputs[1].val;
		net.inputs[2].val=0;
		net.inputs[3].val=0;
		size_t temp[2] = {0,0}, len[2] = {3*cells_n,inputs_n+cells_n-trans_n+2};
		gen_diffbl_vec(net.weights_diffbl, net.weights, temp, len );
		net.evolve();
		std::cout << net.inputs[overwrite_offs].val << ' '
		<< net.inputs[overwrite_offs+1].val << std::endl;
		net.evolve();
		std::cout << net.inputs[overwrite_offs].val << std::endl;
	}
}
