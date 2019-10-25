#pragma once
#include "date.h"
#include <vector>

class TermStructure
{
public:
	TermStructure(Date start_d, double start_r)
		:dates_({ start_d }), rates_(start_r) {}
	void pushPoint(Date d, double rate);
	std::vector<Date> getDates();
	double value(Date d);

private:
	std::vector<Date> dates_;
	std::vector<double> rates_;
};

class YieldTermStructure : public TermStructure
{
public:
	double discount(Date d);
	double forward(Date d1, Date d2);
};

class VolatilityTermStructure : public TermStructure
{
public:
	double variance(Date d);
};
