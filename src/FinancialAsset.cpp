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

void FinancialAsset::simulateMultipleAssets(unsigned N, double s_0 , double ** res, bool antithetic_variates, unsigned numthreads) 
{
	unsigned n = antithetic_variates ? 2*N : N;
	double ** brownianMotions = new double *[n];
	double t;

	# pragma omp parallel for num_threads(numthreads) 
	for (unsigned i = 0; i < n; i ++) {
		brownianMotions[i] = new double[_M];
	}

	simulateMultipleBrownianMotions(_T, _delta_t, _M, N, brownianMotions, antithetic_variates, numthreads);

	# pragma omp parallel for num_threads(numthreads)  
	for (unsigned i = 0; i < n; i++) {
		for (unsigned j = 0; j < _M; j++) {
			t = j*_delta_t;
			res[i][j] = s_0 * exp((_r -_D-pow(_sigma, 2)/2.0)*t+_sigma*brownianMotions[i][j]);
		}
	}
}

double * FinancialAsset::estimateFinalValue(unsigned N, double s_0, bool antithetic_variates, unsigned numthreads) {
	unsigned n = antithetic_variates ? 2*N : N;
	double ** A = new double *[n];
	double * res = new double[5];

	# pragma omp parallel for num_threads(numthreads) 
	for (unsigned i = 0; i < n; i ++) {
		A[i] = new double[_M];
	}
	simulateMultipleAssets(N, s_0, A, antithetic_variates, numthreads);
	double * sum = new double[_M];
	double sum_squared = 0;
	double error = 0;
	double t = 0;


	for (unsigned j = 0; j < _M; j++) {
		# pragma omp parallel for num_threads(numthreads) reduction(+:sum[j], sum_squared)
		for (unsigned i = 0; i < n; i++) {
			sum[j] += fmax(A[i][j]-_K, 0.0);
		}
	}

	for (unsigned i = 0; i < n; i++) {
		sum_squared += pow(fmax(A[i][_M-1]-_K, 0.0), 2);
	}

	# pragma omp parallel for num_threads(numthreads) reduction(max: error) 
	for (unsigned j = 0; j < _M; j++) {
		t = j*_delta_t;
		error = max(error, abs(exactSolution(s_0, _T-t)-exp(-_r*(_T-t))*sum[j]/n));
	}

	res[0] = exp(-_r*_T)*sum[_M-1]/n;
	res[1] = exp(-_r*_T)*sqrt((sum_squared/n-pow(sum[_M-1]/n, 2))*(n/(n-1)));
	res[2] = res[0]-1.96*res[1]/sqrt(n);
	res[3] = res[0]+1.96*res[1]/sqrt(n);
	res[4] = error;
	return res;
}

double FinancialAsset::exactSolution(double s_0, double t) {
	double d_1 = (log(s_0/_K)+((_r-_D)+_sigma*_sigma/2)*(_T-t))/(_sigma*sqrt(_T-t));
	double d_2 = (log(s_0/_K)+((_r-_D)-_sigma*_sigma/2)*(_T-t))/(_sigma*sqrt(_T-t));
	return s_0*exp(-_D*(_T-t))*normalRepartition(d_1)-_K*exp(-_r*(_T-t))*normalRepartition(d_2);
}

void FinancialAsset::tic()
{
	double t1;
	gettimeofday(&s_time,NULL);
	t1=s_time.tv_sec+(s_time.tv_usec/1000000.0);
	tictoc_stack.push_back(t1);
	return;
}

double FinancialAsset::toc(bool test)
{
  // time counter since last tic() / not to use in a pragma command
	if(tictoc_stack.empty()){
		printf("Error! No previous tic function call recorded\n");
    	return -1.;
  	}
	else {
	    double t1;
		gettimeofday(&s_time,NULL);
		t1=s_time.tv_sec+(s_time.tv_usec/1000000.0);
		double timer = (double)((t1-tictoc_stack.back()));
		if(test==0)
			tictoc_stack.pop_back();
		return timer;
	}
}


FinancialAsset::~FinancialAsset(){}
