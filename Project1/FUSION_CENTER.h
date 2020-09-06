#pragma once
#include <vector>
#include "SU.h"
using namespace std;

class SU;

class FUSION_CENTER
{
public:

	vector<int> collected_data_from_SUs;
	vector<bool> PU_FINAL_STATE;
	double misdetection_FS;
	double false_alarm_FS;

	void FC_operation_calculate_fa_md(const unsigned short Num_of_bands, const unsigned short Num_of_SU, vector<SU>& SU_user, vector<bool>& PU_STATE);

	FUSION_CENTER();
	~FUSION_CENTER();
};

