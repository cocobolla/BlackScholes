#include "plainvanilla_option.h"
#include "normal.h"
#include <cmath>
#include <iostream>


double PlainVanillaOption::price() {
	double d1 = getd1();
	double d2 = getd2();
	double nd1 = normcdf(type_*d1);
	double nd2 = normcdf(type_*d2);
	double price = type_*(s_*exp(-q_*t_)*nd1 - strike_*exp(-r_*t_)*nd2);
	return price;
}

double PlainVanillaOption::impliedVol(double mktPrice, double init, double tol) {
	double x = init;
	double e = 1;
	while (e > tol) {
		setVolatility(x);
		double diff = price() - mktPrice;
		e = abs(diff);
		x = x - diff / vega();
	}
	return x;
}

void PlainVanillaOption::printInfo() {
	std::cout << "Product:\tPlain Vanilla Option" << std::endl;
	if (type_ == Call)
		std::cout << "Type:\t\tCall" << std::endl;
	else
		std::cout << "Type:\t\tPut" << std::endl;
	std::cout << "Expiration:\t" << expiration_.year() << '.' <<
		expiration_.month() << '.' << expiration_.day() << std::endl;
	std::cout << "Rf: " << r_ << "\t";
	std::cout << "Div: " << q_ << "\t";
	std::cout << "Vol: " << sigma_ << "\t" << std::endl;
}

double PlainVanillaOption::vega() {
	double d1 = getd1();
	double npd1 = normpdf(d1);
	double vega = s_*exp(-q_*t_)*npd1*sqrt(t_);
	return vega;
}

double PlainVanillaOption::delta() {
	double d1 = getd1();
	double nd1 = normcdf(d1);
	double delta = exp(-q_ * t_)*(nd1 + (type_ - 1) / 2);
	return delta;
}

double PlainVanillaOption::gamma() {
	double d1 = getd1();
	double npd1 = normpdf(d1);
	double gamma = (npd1 * exp(-q_ * t_)) / (s_ * sigma_ * sqrt(t_));
	return gamma;
}

double PlainVanillaOption::rho() {
	double d2 = getd2();
	double nd2 = normcdf(type_ * d2);
	double rho = type_ * strike_ * t_ * exp(-r_ * t_) * nd2;
	return rho;
}
double PlainVanillaOption::theta() {
	double d1 = getd1();
	double d2 = getd2();
	double npd1 = normpdf(d1);
	double nd1 = normpdf(type_ * d1);
	double nd2 = normpdf(type_ * d2);
	double theta = -s_ * npd1 * sigma_ * exp(-q_ * t_) / (2 * sqrt(t_))
		+ type_ * q_ * s_ * nd1 * exp(-q_ * t_)
		- type_ * r_ * strike_ * exp(-r_ * t_) * nd2;
	return theta;
}
