#include "PU.h"
#include <cmath>
#include<vector>
#include <iostream>

using namespace std;

void PU::set_probability_allocate_PU_active_and_extract_vector(const float prob, vector<bool>& PU_STATE, const unsigned short i, unsigned short& PU_active)
{
	Probability_PU_ON = prob;

	double PURandom = (double(rand()) / double(RAND_MAX));
	if (!Probability_PU_ON) {
		PU_state = 0;
		PU_STATE[i] = 0;
	}
	else if (PURandom < Probability_PU_ON) {                //compare
		PU_state = 1;
		PU_STATE[i] = 1;
		PU_active++;
	}
	else {
		PU_state = 0;
		PU_STATE[i] = 0;
	}
}


void PU::get_throughput_utilization_interference(const bool PU_STATE, const unsigned short freq_of_band) {
	//calculating throughput
	if (!PU_STATE && freq_of_band == 1)
	{
		throughput++;
	}
	
	//calculating utilization
	if (freq_of_band >= 1)
	{
		utilization++;
	}

	//calculating interference
	if (PU_STATE) {
		number_of_active++;
		if (freq_of_band > 0) {
			interferance++;
		}
	}
}

PU::PU()
{
}


PU::~PU()
{
}

