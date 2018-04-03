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
	}

	FinancialAsset* asset = new FinancialAsset(&a_FA);

	cout << "Estimated initial value : " << asset->estimateFinalValue(N, s_0) << endl;
	cout << "Exact solution : " << asset->exactSolution(s_0, 0) << endl;
	return 0;
}
