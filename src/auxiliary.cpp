#include "../includes/auxiliary.h"

inline double kappa(double x) {
	const double gamma = 0.2316419;
	return 1.0/(1.0+gamma*x);
}

double normalRepartition(double x) {
	const double a_1 = 0.319381530;
	const double a_2 = -0.356563782;
	const double a_3 = 1.781477937;
	const double a_4 = -1.821255978;
	const double a_5 = 1.330274429;
double k=kappa(x);
	if (x >= 0) {
		return 1 - exp(-x*x/2)/sqrt(2*M_PI)*(a_1*k+a_2*pow(k,2)+a_3*pow(k,3)+a_4*pow(k,4)+a_5*pow(k,5));
	}
	else {
			// correction ici pour la formule de la loi Normale
		return 1.0-normalRepartition(-x);
	}
}

void brownianMotion(double T, double delta_t, unsigned M,double* res) {
	std::random_device rd{};
	std::mt19937 generator{rd()};
	std::normal_distribution<> distribution(0, 1);

	double sdelta_t = sqrt(delta_t);

	res[0] = 0;
	for (unsigned i = 0; i < M-1; i++) {
		res[i+1] = res[i] + distribution(generator)*sdelta_t;
	}

}

void simulateMultipleBrownianMotions(double T,	double delta_t, unsigned M,	unsigned N, double ** res) {

	for (unsigned i = 0; i < N; i++) {
		brownianMotion(T, delta_t, M, res[i]);
	}

}
