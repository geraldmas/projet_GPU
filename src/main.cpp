#include <iostream>
#include "../includes/FinancialAsset.h"

using namespace std;

int main (int argc, char *argv[]) {

	arguments_FA a_FA;
	{
		a_FA.L = 20;
		a_FA.T = 0.5;
		a_FA.K = 10;
		a_FA.r = 0.1;
		a_FA.sigma = 0.5;
		a_FA.D = 0.04;
		a_FA.Nb = 100;
	}

	unsigned N = 100;
	double s_0 = 2;
	bool testing_mode = false;
	unsigned nb_test = 1;
	bool antithetic_variates = false;
	unsigned numthreads = 1;

	for (int i = 1; i < argc; ++i) {
		if (string(argv[i]) == "-N") {
			if (i+1 < argc) {
				i++;
				N = atoi(argv[i]);
			}
		}
		if (string(argv[i]) == "--s0" || string(argv[i]) == "-s0") {
			if (i+1 < argc) {
				i++;
				s_0 = stod(argv[i]);
			}
		}
		if (string(argv[i]) == "--antithetic" || string(argv[i]) == "-a") {
			antithetic_variates = true;
		}
		if (string(argv[i]) == "--numthreads" || string(argv[i]) == "-n") {
			if (i+1 < argc) {
				i++;
				numthreads = atoi(argv[i]);
			}
		}
	}

	FinancialAsset* asset = new FinancialAsset(&a_FA);

	asset->tic();

	double * res = asset->estimateFinalValue(N, s_0, antithetic_variates, numthreads);

	cout << endl << " ****** ****** ****** ****** ****** ****** *****" << endl << endl;

	cout << "Computing asset value with Monte-Carlo method using following parameters : " << endl;
	if (numthreads > 1) {
		cout << " - Using OpenMP with " << numthreads << " threads" << endl;
	}
	else {
		cout << " - Not using OpenMP" << endl;
	}
	cout << " - Number of experiments : " << N << endl;
	cout << " - Initial price : " << s_0 << endl;
	cout << " - Spatial length : " << a_FA.L << endl; 
	cout << " - Temporal length : " << a_FA.T << endl;
	cout << " - Ulterior price : " << a_FA.K << endl;
	cout << " - Interest rate : " << a_FA.r << endl;
	cout << " - Volatility : " << a_FA.sigma << endl;
	cout << " - Dividend : " << a_FA.D << endl;
	cout << " - Temporal points for discretization : " << a_FA.Nb << endl << endl;

	if (antithetic_variates) {
		cout << "Using antithetic variates method for reduicing the estimated standard deviation. " << endl << endl;
	}

	cout << " ****** ****** ****** ****** ****** ****** *****" << endl << endl;

	cout << "Exact solution : " << asset->exactSolution(s_0, 0) << endl << endl;
	cout << "Estimated expected value : " << res[0] << endl;

	cout << "Estimated standard deviation : " << res[1] << endl;
	cout << "Confidence interval (0.95%) : [" << res[2] << ", " << res[3] << "]" << endl << endl;
	
	cout << "Price error (absolute) : " << res[0]-asset->exactSolution(s_0, 0) << endl;
	cout << "Price error (relative) : " << 100*(res[0]/asset->exactSolution(s_0, 0)-1) << "%" << endl;
	cout << "Total error (L_infty norm) : " << res[4] << endl << endl;

	cout << " ****** ****** ****** ****** ****** ****** *****" << endl << endl;

	double elapsed_time = asset->toc(1);
	cout << "Elapsed time : " << elapsed_time << endl << endl;

	cout << " ****** ****** ****** ****** ****** ****** *****" << endl << endl;

	return 0;
}
