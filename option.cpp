#include "option.h"
#include <cmath>
#include <random>

void Option::setEvalDate(Date d) {
	evalDate_ = d;
	t_ = daysBetween(evalDate_, expiration_) / 365.0; 
}

void Option::setProcess(GBMProcess p) {
	p_ = p;
	s_ = p_.getSpot();
	r_ = p_.getRf();
	q_ = p_.getDiv();
	sigma_ = p_.getVol();
}

double Option::getd1() {
	return (log(s_ / strike_) + (r_ - q_ + 0.5*sigma_*sigma_)*t_) / (sigma_*sqrt(t_));
}
double Option::getd2() {
	return getd1() - sigma_*sqrt(t_);
}

double Option::mcprice(unsigned int numOfSimulation) {
	double sumOfPayoff = 0;
	double df = exp(-r_*t_);
	std::mt19937_64 gen;
	std::normal_distribution<double> engine(0.0, 1.0);
	gen.seed(std::random_device{}());
	double es = s_*exp((r_ - q_ - 0.5*sigma_*sigma_)*t_);
	double diffution = sigma_*sqrt(t_);
	for (unsigned int i = 0; i < numOfSimulation; ++i) {
		double e = engine(gen);
		for (int j = 0; j < 2; ++j) {
			double st = es * exp(diffution*(1 - j * 2)*e);
			double p = (*payoff_)(st);
			sumOfPayoff += df * p;
		}
	}
	return sumOfPayoff / numOfSimulation / 2.0;
}

double Option::bntprice(unsigned int nsteps) {
	double dt = t_ / nsteps;
	double u = exp(sigma_*sqrt(dt));
	double d = 1 / u;
	double p = (exp((r_ - q_)*dt) - d) / (u - d);
	double df = exp(-r_*dt);

	std::vector<double> v(nsteps + 1, 0.0);
	for (int j = 0; j <= nsteps; ++j) {
		double st = s_*pow(u, nsteps - j)*pow(d, j);
		v[j] = (*payoff_)(st);
	}

	for (int i = nsteps - 1; i >= 0; --i) {
		for (int j = 0; j <= i; ++j)
			v[j] = df*(v[j] * p + v[j + 1] * (1 - p));
	}
	return v[0];
}

// Assignment.2
double Option::vega() {
	GBMProcess original_gbm = p_;
	double v = original_gbm.getVol();
	double v1 = v + 0.01;
	double v2 = v - 0.01;

	//TODO: it is better to Encapsulate Change process and Reset process
	// Get P(sigma + 0.01)
	GBMProcess temp_gbm = original_gbm;
	temp_gbm.setVol(v1);
	setProcess(temp_gbm);
	double p1 = price();

	// Get P(sigma - 0.01)
	temp_gbm.setVol(v2);
	setProcess(temp_gbm);
	double p2 = price();

	// Reset original GBM
	setProcess(original_gbm);

	double vega = (p1 - p2) / 0.02;
	return vega;
}

double Option::delta() {
	GBMProcess original_gbm = p_;
	double s = original_gbm.getSpot();
	double s1 = s * 1.01;
	double s2 = s * 0.99;

	// Get P(1.01 * S)
	GBMProcess temp_gbm = original_gbm;
	temp_gbm.setSpot(s1);
	setProcess(temp_gbm);
	double p1 = price();

	// Get P(0.99 * S)
	temp_gbm.setSpot(s2);
	setProcess(temp_gbm);
	double p2 = price();

	// Return to original GBM
	setProcess(original_gbm);

	double delta = (p1 - p2) / (0.02 * s);

	return delta;
}

double Option::gamma() {
	GBMProcess original_gbm = p_;
	double s = original_gbm.getSpot();
	double s1 = s * 1.01;
	double s2 = s * 0.99;

	// Get P(1.01 * S)
	GBMProcess temp_gbm = original_gbm;
	temp_gbm.setSpot(s1);
	setProcess(temp_gbm);
	double p1 = price();

	// Get P(S)
	temp_gbm.setSpot(s);
	setProcess(temp_gbm);
	double p = price();

	// Get P(0.99 * S)
	temp_gbm.setSpot(s2);
	setProcess(temp_gbm);
	double p2 = price();

	// Return to original GBM
	setProcess(original_gbm);

	double gamma = (p1- 2 * p + p2) / pow(0.01 * s, 2);
	return gamma;
}

double Option::rho() {
	GBMProcess original_gbm = p_;
	double r = original_gbm.getRf();
	double r1 = r + 0.0001;
	double r2 = r - 0.0001;

	// Get P(r + 0.0001)
	GBMProcess temp_gbm = original_gbm;
	temp_gbm.setRf(r1);
	setProcess(temp_gbm);
	double p1 = price();

	// Get P(r - 0.0001)
	temp_gbm.setRf(r2);
	setProcess(temp_gbm);
	double p2 = price();

	// Reset original GBM
	setProcess(original_gbm);

	double rho = (p1 - p2) / 0.0002;
	return rho;
}

double Option::theta() {
	double t = getT();
	double t1 = t - 0.0001;
	setT(t1);
	double p1 = price();

	// Reset original t
	setT(t);
	double p = price();

	double theta = (p1 - p) / 0.0001;
	return theta;
}