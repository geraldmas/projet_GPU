#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <cmath>
#include <random>
#include <iostream>

inline double kappa(double x);

double normalRepartition(double x);

void  brownianMotion(double T,double, unsigned M, double *);

void simulateMultipleBrownianMotions(double T, double delta_t,  unsigned M, unsigned N, double ** res);

#endif
