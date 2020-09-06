#pragma once
#include<vector> 
#include <iostream>
#include "FUSION_CENTER.h"

using namespace std;
class FUSION_CENTER;

class SU{

public:

	//sensing data
	float Pfa, Pmd;
	vector<bool> sensing_vector;
	double false_alarm_SU;
	double misdetection_SU;
	void set_pfa_pmd_and_sensing_PU_data_and_sending_to_FC(vector<bool>& PU_STATE, const unsigned short Num_of_SU, const unsigned short Num_of_bands, float pfa, float pmd, FUSION_CENTER& fusion_center);
	
	//cooperative sensing
	vector<bool> result_from_FC;
	void get_result_from_FC(const unsigned short Num_of_bands, FUSION_CENTER& fusion_centre);

	//allocating bands
	int collision = 0;
	vector<int>  SU_band_vector;
	void band_selection(const unsigned short Num_of_bands, const unsigned short band_per_SU, vector<int> &freq_array);
	void get_collision(vector<int>& freq_array, vector<bool>& PU_state, const unsigned short band_per_SU);



		
	SU();
	~SU();
};

