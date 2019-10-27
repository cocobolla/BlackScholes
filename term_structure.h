#pragma once
#include "date.h"
#include <vector>

class TermStructure
{
public:
	TermStructure() {};
	TermStructure(Date start_d, double start_r) {
		dates_.push_back(start_d);
		rates_.push_back(start_r);
	}
	void pushPoint(Date d, double rate);
	std::vector<Date> getDates();
	double value(Date d);
	void operator+= (const double d);
	void operator*= (const double d);


private:
	std::vector<Date> dates_;
	std::vector<double> rates_;
};

class YieldTermStructure : public TermStructure
{
public:
	YieldTermStructure() {};
	YieldTermStructure(Date start_d, double start_r)
		:TermStructure(start_d, start_r) {}
	double discount(Date d);
	double forward(Date d1, Date d2);
};

class VolatilityTermStructure : public TermStructure
{
public:
	VolatilityTermStructure() {};
	VolatilityTermStructure(Date start_d, double start_r)
		:TermStructure(start_d, start_r) {}
	double variance(Date d);
};

class DividendTermStructure : public TermStructure
{
public:
	DividendTermStructure() {};
	DividendTermStructure(Date start_d, double start_r)
		:TermStructure(start_d, start_r) {}
};

struct MarketTermStructure {
	Date date;
	double yield, div, vol;
};
