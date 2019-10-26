#include "term_structure.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

void TermStructure::pushPoint(Date d, double rate) {
	//TODO: Should check whether Date is sorted.
	dates_.push_back(d);
	rates_.push_back(rate);
}

std::vector<Date> TermStructure::getDates() {
	return dates_;
}

// Find Where the d is between, and get value with Linear Interpolation
double TermStructure::value(Date d) {
	double inter_rate = 0;
	if (d < dates_.at(0)) {
		std::cout << "Cannot interpolate. Out of range." << std::endl;
		throw d;
	}

	else if (d == dates_.at(0))
		inter_rate = rates_.at(0);

	else {
		int i = 0;
		while (d > dates_.at(i)) {
			i++;
			if (i == dates_.size()) {
				std::cout << "Cannot interpolate. Out of range." << std::endl;
				throw d;
			}
		}

		Date d2 = dates_.at(i);
		Date d1 = dates_.at(i-1);
		double r2 = rates_.at(i);
		double r1 = rates_.at(i - 1);

		double x1 = daysBetween(d1, d);
		double x2 = daysBetween(d, d2);

		inter_rate = x2 / (x1 + x2) * r1 + x1 / (x1 + x2) * r2;
	}

	return inter_rate;
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

//TODO: What is the Time volatility
double VolatilityTermStructure::variance(Date d) {
	return 1.2;
}