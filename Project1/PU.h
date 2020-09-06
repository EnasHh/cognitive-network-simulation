#include <vector>
#include <iostream>
#pragma once

using namespace std;

class PU
{
public:
	
	//PU_allocating
	float Probability_PU_ON;	                 
	bool PU_state;         //state PU (on/off)
	void set_probability_allocate_PU_active_and_extract_vector(const float prob, vector<bool>& PU_STATE, const unsigned short i, unsigned short& PU_active);       //PU=1 means PU is on
 		                     		//PU=0 means PU is off

	//parameter calculations
	double throughput = 0.0;
	double utilization = 0.0;
	unsigned short number_of_active = 0;
	double interferance = 0.0;
	//void print_PU_active_vector();
	void get_throughput_utilization_interference(const bool PU_STATE, const unsigned short freq_of_band);
	
	PU();
	~PU();
	
};


