#include "FUSION_CENTER.h"


void FUSION_CENTER::FC_operation_calculate_fa_md(const unsigned short Num_of_bands, const unsigned short Num_of_SU, vector<SU>& SU_user, vector<bool>& PU_STATE)
{
	false_alarm_FS = 0;
	misdetection_FS = 0;
	for (int j = 0; j < Num_of_bands; j++)
	{
		if (collected_data_from_SUs[j] > Num_of_SU / 2)
		{
			PU_FINAL_STATE[j] = 1;
			if (!PU_STATE[j]) {
				false_alarm_FS++;
			}
		}
		else
		{
			PU_FINAL_STATE[j] = 0;
			if (PU_STATE[j]) {
				misdetection_FS++;
			}
		}

	}

}


FUSION_CENTER::FUSION_CENTER()
{
}

FUSION_CENTER::~FUSION_CENTER()
{
}
