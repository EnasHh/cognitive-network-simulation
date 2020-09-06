#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

#include "SU.h"
#include "PU.h"
#include "FUSION_CENTER.h"
#include <time.h>

using namespace std;

int main()
{
	clock_t tStart = clock();		//execution time calculator

	//parameter declaration and reading
	unsigned short Num_of_bands = 100;			//Number of bands	
	unsigned short Num_of_SU = 10;				//Number of SU
	unsigned short T = 20000;					//Number of time slots
	float pfa = 0.1;				//Probability of False alarm
	float pmd = 0.1;				//Probability of Misdetection
	unsigned short user = 3;					//which user to draw vs. time
	bool cooperative = 0;			//determine the state of the fusion center
	float Probabilty_PU_ON;	//determine the PU_active_probability
	vector<float> PU_ACTIVE{ 0.0,0.15,0.25 };
	unsigned short Sn;							//number of bands per SU
	unsigned short LTDDF = 10;					//Load To Draw Data For
	unsigned short PU_active = 0;

	//read parameters

	//vectors to know who is on in which band
	vector<bool> PU_STATE(Num_of_bands);
	vector<int> freq_array(Num_of_bands);

	//vectors to hold loads to find success for
	vector<unsigned short> load_change{ 5,8,10,12,15 };

	//creating and resizing database to store data
	vector<vector<double>> performance_file(11);
	performance_file[0].resize(5);
	performance_file[0][0] = Num_of_bands;
	performance_file[0][1] = Num_of_SU;
	performance_file[0][2] = T;
	performance_file[0][3] = LTDDF;
	performance_file[0][4] = load_change.size();
	performance_file[1].resize(T);
	for (unsigned short i = 2; i < 5; i++) {
		performance_file[i].resize(Num_of_bands);
	}
	for (unsigned short i = 5; i < 9; i++) {
		performance_file[i].resize(Num_of_SU);
	}
	performance_file[9].resize(load_change.size());
	performance_file[10].resize(load_change.size());

	//users vectors
	vector<PU> PU_user(Num_of_bands);
	vector<SU> SU_user(Num_of_SU);
	vector<FUSION_CENTER> fusion_center(1);
	for (unsigned short i = 0; i < Num_of_SU; i++) {
		SU_user[i].result_from_FC.resize(Num_of_bands);
		SU_user[i].sensing_vector.resize(Num_of_bands);
	}
	fusion_center[0].collected_data_from_SUs.resize(Num_of_bands, 0);
	fusion_center[0].PU_FINAL_STATE.resize(Num_of_bands, 0);

	//begining of code
	for (unsigned short PU_ACTIVE_CHANGE = 0; PU_ACTIVE_CHANGE < PU_ACTIVE.size(); PU_ACTIVE_CHANGE++) {
		
		//change PU_ACTIVE probability
		Probabilty_PU_ON = PU_ACTIVE[PU_ACTIVE_CHANGE];
		
		for (unsigned short coop_state = 0; coop_state < 2; coop_state++) {
			
			//change cooperative state
			cooperative = coop_state;
			
			//keep the same random results every time
			srand(73);
	
			//restarting database for new data to come
			for (unsigned short j = 7; j < 9; j++) {
				for (unsigned short i = 0; i < Num_of_SU; i++) {
					performance_file[j][i] = 0;
				}
			}
			for (unsigned short i = 0; i < load_change.size(); i++) {
				performance_file[10][i] = 0;
			}


			for (int Sn_range = 0; Sn_range < load_change.size(); Sn_range++) {
				//begining of load changing loop

				//extract loades to draw data for
				performance_file[9][Sn_range] = load_change[Sn_range];

				//select number of pands per SU
				Sn = load_change[Sn_range];


				//restarting some vectors and classes members
				if (Sn == LTDDF) {
					for (unsigned short i = 0; i < Num_of_bands; i++) {
						PU_user[i].throughput = 0;
						PU_user[i].utilization = 0;
						PU_user[i].number_of_active = 0;
						PU_user[i].interferance = 0;
					}
				}
				for (unsigned short i = 0; i < Num_of_SU; i++) {
					SU_user[i].SU_band_vector.resize(Sn, 0);
					SU_user[i].collision = 0;
				}

				for (unsigned short t = 0; t < T; t++) {
					//start of time loop

					//variable to count PU_active every time slot
					PU_active = 0;

					//PU alocation
					for (unsigned short i = 0; i < Num_of_bands; i++) {
						PU_user[i].set_probability_allocate_PU_active_and_extract_vector(Probabilty_PU_ON, PU_STATE, i, PU_active);

						//using same loop for some other stuff
						//resetting vector
						freq_array[i] = 0;
						//reseting vector
						fusion_center[0].collected_data_from_SUs[i] = 0;
					}

					//sensing
					for (unsigned short i = 0; i < Num_of_SU; i++) {
						SU_user[i].set_pfa_pmd_and_sensing_PU_data_and_sending_to_FC(PU_STATE, Num_of_SU, Num_of_bands, pfa, pmd, fusion_center[0]);
					}

					//FC proccessing
					if (cooperative) {
						//data sharing and FC decision
						fusion_center[0].FC_operation_calculate_fa_md(Num_of_bands, Num_of_SU, SU_user, PU_STATE);

						//receiving results from FC
						for (int i = 0; i < Num_of_SU; i++)
						{
							SU_user[i].get_result_from_FC(Num_of_bands, fusion_center[0]);
						}
					}
					//get the sum of fa and md over all time
					if (Sn == LTDDF) {
						if (PU_active != Num_of_bands && PU_active != 0) {
							for (int i = 0; i < Num_of_SU; i++) {
								SU_user[i].false_alarm_SU = SU_user[i].false_alarm_SU / double(Num_of_bands - PU_active);
								SU_user[i].misdetection_SU = SU_user[i].misdetection_SU / double(PU_active);
								performance_file[7][i] = performance_file[7][i] + SU_user[i].false_alarm_SU;
								performance_file[8][i] = performance_file[8][i] + SU_user[i].misdetection_SU;
							}
						}
						else if (PU_active == 0) {
							for (int i = 0; i < Num_of_SU; i++) {
								SU_user[i].false_alarm_SU = SU_user[i].false_alarm_SU / double(Num_of_bands - PU_active);
								SU_user[i].misdetection_SU = 0;
								performance_file[7][i] = performance_file[7][i] + SU_user[i].false_alarm_SU;
								performance_file[8][i] = performance_file[8][i] + SU_user[i].misdetection_SU;
							}
						}
						else if (PU_active == Num_of_bands) {
							for (int i = 0; i < Num_of_SU; i++) {
								SU_user[i].false_alarm_SU = 0;
								SU_user[i].misdetection_SU = SU_user[i].misdetection_SU / double(PU_active);
								performance_file[7][i] = performance_file[7][i] + SU_user[i].false_alarm_SU;
								performance_file[8][i] = performance_file[8][i] + SU_user[i].misdetection_SU;
							}
						}
					}

					//SU_allocation
					//allocate bands and get a vector of how many SU on every band!
					for (int i = 0; i < Num_of_SU; i++) {
						SU_user[i].band_selection(Num_of_bands, Sn, freq_array);
					}

					//calculate performance data
					for (int s = 0; s < Num_of_SU; s++) {
						SU_user[s].get_collision(freq_array, PU_STATE, Sn);
					}
					if (Sn == LTDDF) {
						for (int v = 0; v < Num_of_bands; v++)
						{
							PU_user[v].get_throughput_utilization_interference(PU_STATE[v], freq_array[v]);
						}
					}

					//find SU user vs time slots
					if (Sn == LTDDF) {
						//reassign the spectrum as required
						if (t <= T / 2) {
							for (int i = 0; i < 100; i++) {
								PU_STATE[i] = 0;
							}
						}
						else {
							for (int i = 0; i < 50; i++) {
								PU_STATE[i] = 1;
							}
							for (int i = 50; i < 100; i++) {
								PU_STATE[i] = 0;
							}
						}

						//calculate success and extracting to database
						performance_file[1][t] = 0;
						for (int i = 0; i < Sn; i++) {
							bool collision = freq_array[SU_user[user].SU_band_vector[i]] > 1;
							bool interference = PU_STATE[SU_user[user].SU_band_vector[i]];
							if (!collision && !interference) {
								performance_file[1][t] = performance_file[1][t] + 1;
							}
						}
					}
					//end of time loop
				}

				//extracting average data to database
				if (Sn == LTDDF) {
					//data vs. num of bands
					for (int i = 0; i < Num_of_bands; i++) {
						//throughput
						performance_file[2][i] = PU_user[i].throughput / double(T);
						//utilization
						performance_file[3][i] = PU_user[i].utilization / double(T);
						//interference ratio
						if (PU_user[i].number_of_active == 0) {
							performance_file[4][i] = 0;
						}
						else {
							performance_file[4][i] = PU_user[i].interferance / PU_user[i].number_of_active;
						}
						//performance_file[4][i] = interferance_ratio_vector[i];
					}

					//data vs. num of SU
					for (int i = 0; i < Num_of_SU; i++) {
						//successful transmission
						performance_file[5][i] = Sn - SU_user[i].collision / double(T);
						//collision
						performance_file[6][i] = SU_user[i].collision / double(T);
						//false alarm
						performance_file[7][i] = performance_file[7][i] / double(T);
						//misdetection
						performance_file[8][i] = performance_file[8][i] / double(T);
					}
				}

				//calculating success vs. load and adding to database
				for (int f = 0; f < Num_of_SU; f++)
				{
					performance_file[10][Sn_range] = performance_file[10][Sn_range] + T*Sn - SU_user[f].collision;//add time average of every user success
				}
				performance_file[10][Sn_range] = performance_file[10][Sn_range] / double(Num_of_SU*T);
				//end of load loop
			}

			//extract data to output *.csv file
			ofstream output_file_to_matlab;
			output_file_to_matlab.open("test_results PU " + to_string(Probabilty_PU_ON) + " coop " + to_string(cooperative) + ".csv");
			for (int i = 0; i < 5; i++) {
				output_file_to_matlab << performance_file[0][i] << ",";
			}
			output_file_to_matlab << endl;

			for (int i = 0; i < T; i++) {
				output_file_to_matlab << performance_file[1][i] << ",";
			}
			output_file_to_matlab << endl;

			for (int i = 2; i < 5; i++) {
				for (int j = 0; j < Num_of_bands; j++) {
					output_file_to_matlab << performance_file[i][j] << ",";
				}
				output_file_to_matlab << endl;
			}
			for (int i = 5; i < 9; i++) {
				for (int j = 0; j < Num_of_SU; j++) {
					output_file_to_matlab << performance_file[i][j] << ",";
				}
				output_file_to_matlab << endl;
			}
			for (int i = 9; i < 11; i++) {
				for (int j = 0; j < load_change.size(); j++) {
					output_file_to_matlab << performance_file[i][j] << ",";
				}
				output_file_to_matlab << endl;
			}
			output_file_to_matlab.close();
			//end of control loop
		}
	}
	printf("time taken: %.2f ms\n", ((double)(clock() - tStart) / CLOCKS_PER_SEC)*1000);
	printf("End of Code\n");
	system("pause");
	return 0;
}