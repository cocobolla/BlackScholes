#include "binary_option.h"
#include "normal.h"
#include <cmath>

double BinaryOption::price() {
	double d2 = getd2();
	double nd2 = normcdf(type_*d2);
	return exp(-r_*t_)*nd2;
}

void BinaryOption::printInfo() {
	std::cout << "Product:\tBinary(Digital) Option" << std::endl;
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