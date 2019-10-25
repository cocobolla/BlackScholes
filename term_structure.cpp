#include "term_structure.h"
#include <cmath>

void TermStructure::pushPoint(Date d, double rate) {
	dates_.push_back(d);
	rates_.push_back(rate);
}

std::vector<Date> TermStructure::getDates() {
	return dates_;
}

// TODO: Find Where the d is between, and get value with Linear Interpolation
double TermStructure::value(Date d) {
	return 1.2;
}

double YieldTermStructure::discount(Date d) {
	Date start_date = getDates().front();
	double t = daysBetween(start_date, d);
	double r = value(d);

	double df = std::exp(-r * t/365);
	return df;
}

double YieldTermStructure::forward(Date d1, Date d2) {
	double t = daysBetween(d1, d2);
	double forward_rate = 365 / t * 
		(discount(d1) / discount(d2) - 1);
	return forward_rate;
}

// TODO: What is the Time volatility
double VolatilityTermStructure::variance(Date d) {
	return 1.2;
}