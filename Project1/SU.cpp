#include <cmath>
#include "SU.h"
#include<vector>
#include <iostream>

using namespace std;



void SU::set_pfa_pmd_and_sensing_PU_data_and_sending_to_FC(vector<bool>& PU_STATE, const unsigned short Num_of_SU, const unsigned short Num_of_bands, float pfa, float pmd, FUSION_CENTER& fusion_center)
{
	Pfa = pfa;
	Pmd = pmd;
	false_alarm_SU = 0;
	misdetection_SU = 0;
	for (int j = 0; j < Num_of_bands; j++) {
		double value = double(rand()) / double(RAND_MAX);
		switch (PU_STATE[j]) {
		case 0:
			if (value >= Pfa) {
				sensing_vector[j] = 0;
			}
			else {
				sensing_vector[j] = 1;
				false_alarm_SU++;
			}
			break;
		case 1: 
			if (value >= Pmd) {
				sensing_vector[j] = 1;
			}
			else {
				sensing_vector[j] = 0;
				misdetection_SU++;
			}
		}
		fusion_center.collected_data_from_SUs[j] = fusion_center.collected_data_from_SUs[j] + sensing_vector[j];
	}
}

void SU::get_result_from_FC(const unsigned short Num_of_bands, FUSION_CENTER& fusion_center) {
	
	result_from_FC = fusion_center.PU_FINAL_STATE;
	false_alarm_SU = fusion_center.false_alarm_FS;
	misdetection_SU = fusion_center.misdetection_FS;
}

void SU::band_selection(const unsigned short Num_of_bands, const unsigned short Sn, vector<int> &freq_array) {
	
	vector<int> repeat(Num_of_bands, 0);
	for (int j = 0; j < Sn; j++) {
		unsigned short value = rand() % Num_of_bands;
		if (repeat[value] > 0)
		{
			j--;
			continue;
		}
		SU_band_vector[j] = value;
		repeat[value]++;
		freq_array[value]++;
	}
}

void SU::get_collision(vector<int>& freq_array, vector<bool>& PU_state, const unsigned short Sn)
{
	for (int i = 0; i < Sn; i++)
	{
		if (freq_array[SU_band_vector[i]] > 1 || PU_state[SU_band_vector[i]])
		{
			collision++;
		}

	}


}


SU::SU()
{
}


SU::~SU()
{
}


