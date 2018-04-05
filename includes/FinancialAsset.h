#ifndef FINANCIALASSET_H
#define FINANCIALASSET_H

#include "auxiliary.h"
#include <ctime>
#include <sys/time.h>

struct arguments_FA {
	double r, sigma, T, L, K, D;
	unsigned int	Nb;
};

class FinancialAsset
{
private:
	double _r, _sigma, _T, _L, _K, _D, _delta_t;
	unsigned _M;

	struct timeval s_time;
	std::vector<double> tictoc_stack;
public:
	FinancialAsset(arguments_FA* a_FA);
	~FinancialAsset();
	void simulateMultipleAssets(unsigned N,	 double s_0 , double ** res, bool antithetic_variates);
	double * estimateFinalValue(unsigned N, double s_0, bool antithetic_variates);
	double exactSolution(double s_0, double t);

	void tic();
	double toc(bool test);
};

#endif
