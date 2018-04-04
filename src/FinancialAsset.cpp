#include "../includes/FinancialAsset.h"

using namespace std;

FinancialAsset::FinancialAsset(arguments_FA* a_FA)
{
	_L = a_FA->L;
	_T = a_FA->T;
	_K = a_FA->K;
	_r = a_FA->r;
	_sigma = a_FA->sigma;
	_D = a_FA->D;
	_M = a_FA->Nb;
	_delta_t = _T/(_M-1);
}

void FinancialAsset::simulateMultipleAssets(unsigned N, double s_0 , double ** res) 
{
	double ** brownianMotions = new double *[N];
	double t;
	for (unsigned i = 0; i < N; i ++) {
		brownianMotions[i]= new double[_M];
	}
	simulateMultipleBrownianMotions(_T, _delta_t, _M, N, brownianMotions);
	// paralléliser sur i 
	for (unsigned j = 0; j < _M; j ++) {
		t = j*_delta_t;
		for (unsigned i = 0; i < N; i ++) {
			res[i][j] = s_0 * exp((_r -_D- _sigma*_sigma/2.0)*t+_sigma*brownianMotions[i][j]);
		}
	}
}

double FinancialAsset::estimateFinalValue(unsigned N, double s_0) {
	double ** A =	new double *[N];
	for (unsigned i = 0; i < N; i ++) {
		A[i]= new double[_M];
	}
	simulateMultipleAssets(N, s_0, A);
	double res = 0;
	for (unsigned i = 0; i < N; i++) {
		res += max(A[i][_M-1]-_K, 0.0);
	}	
	return exp(-_r*_T)*res/N;
}

// calcul variance intervalle de confiance

double FinancialAsset::exactSolution(double s_0, double t) {
	// ici une correction de mon erreur de l'énoncé: permuter d1 et d2
	double d_1 = (log(s_0/_K)+((_r-_D)+_sigma*_sigma/2)*(_T-t))/(_sigma*sqrt(_T-t));
	double d_2 = (log(s_0/_K)+((_r-_D)-_sigma*_sigma/2)*(_T-t))/(_sigma*sqrt(_T-t));
	return s_0*exp(-_D*(_T-t))*normalRepartition(d_1)-_K*exp(-_r*(_T-t))*normalRepartition(d_2);
}



FinancialAsset::~FinancialAsset(){}
