#ifndef FINANCIALASSET_H
#define FINANCIALASSET_H

#include "auxiliary.h"

struct arguments_FA {
	double r, sigma, T, L, K, D;
	unsigned int  Nb;
};

class FinancialAsset
{
private:
	double _r, _sigma, _T, _L, _K, _D, _delta_t;
	unsigned _M;
public:
	FinancialAsset(arguments_FA* a_FA);
	~FinancialAsset();
	 void  simulateMultipleAssets(unsigned N,   double s_0 , double ** res);
	double estimateFinalValue(unsigned N, double s_0);
	double exactSolution(double s_0, double t);
};

#endif
