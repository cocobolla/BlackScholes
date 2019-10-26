#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include "plainvanilla_option.h"
#include "binary_option.h"
#include "term_structure.h"
#include "option_portfolio.h"


void print_option(Option* x) {
	std::cout << "price = " << x->price() << std::endl;
	std::cout << "mc = " << x->mcprice(10000) << std::endl;
	std::cout << "bnt = " << x->bntprice(100) << std::endl;
	std::cout << std::string(30, '-') << std::endl;
}

int main2() {
	Date evalDate(2019, 9, 1);
	Date mat("20191031");
	double spot = 100, stk = 100, rf = 0.01, div = 0.01, vol = 0.2;
	OptionType type = Call;
	GBMProcess process(spot, rf, div, vol);
	std::vector<double> mktprice = { 3.2, 4.2, 1.4 };
	std::vector<Option*> inst;

	inst.push_back(new BinaryOption(mat, stk, type));
	inst.push_back(new PlainVanillaOption(mat, stk, type));
	for (unsigned int i = 0; i < inst.size(); ++i) {
		inst[i]->setProcess(process);
		inst[i]->setEvalDate(evalDate);
		print_option(inst[i]);
	}
	for (unsigned int i = 0; i < inst.size(); ++i)
		delete inst[i];
	return 0;
}
int main1() {
	Date evalDate(2019, 9, 1);
	Date mat1("20190930");
	Date mat2("20190802");
	Date mat3("20191010");
	
	Date tar("20190902");

	bool a = mat1 > evalDate;
	std::cout << a << std::endl;
	std::vector<Date> dates;
	dates.push_back(evalDate);
	dates.push_back(mat1);
	dates.push_back(mat2);
	dates.push_back(mat3);
	sort(dates.begin(), dates.end());

	std::vector<Date>::iterator iter = dates.begin();
	std::vector<Date>::iterator iter_end = dates.end();

	while (tar > * iter) {
		if (iter == iter_end) {
			std::cout << "Cannot interpolation" << std::endl;
			throw tar;
		}
		iter++;
	}
	Date d2 = *iter;
	Date d1 = *(--iter);

	double x1 = daysBetween(d1, tar);
	double x2 = daysBetween(tar, d2);

	std::cout << x1 << std::endl;
	std::cout << x2 << std::endl;

	return 0;
}

int main3() {
	Date mat1("20190930");
	Date mat2("20190901");
	std::cout << (mat1 < mat2) << std::endl;
	return 0;
}

int main() {
	double spot = 200;
	Date eval_date(2019, 9, 30);

	// Set Term Structure
	YieldTermStructure yield_term;
	VolatilityTermStructure vol_term;
	DividendTermStructure div_term;

	MarketTermStructure market_term[8] = {
		{Date(2019, 9, 30), 0.015, 0, 0.1},
		{Date(2019, 10, 30), 0.015, 0, 0.11},
		{Date(2019, 11, 30), 0.017, 0, 0.12},
		{Date(2019, 12, 30), 0.0185, 0.03, 0.125},
		{Date(2020, 1, 30), 0.0195, 0.03, 0.13},
		{Date(2020, 2, 28), 0.0205, 0.03, 0.135},
		{Date(2020, 3, 30), 0.0213, 0.04, 0.14},
		{Date(2020, 4, 30), 0.0220, 0.04, 0.145},
	};

	for (int i = 0; i < sizeof(market_term) / sizeof(MarketTermStructure); i++) {
		yield_term.pushPoint(market_term[i].date, market_term[i].yield);
		div_term.pushPoint(market_term[i].date, market_term[i].div);
		vol_term.pushPoint(market_term[i].date, market_term[i].vol);
	}

	// Set Option
	OptionPortfolio portfolio(yield_term, vol_term, div_term, spot, eval_date);
	portfolio.pushOption(Vanila, Long, Call, 200, Date(2020,1,10), 2);
	portfolio.pushOption(Vanila, Short, Call, 205, Date(2019,12,12), 1);
	portfolio.pushOption(Vanila, Long, Call, 195, Date(2020,3,15), 3);
	portfolio.pushOption(Vanila, Short, Put, 200, Date(2019,12,12), 2);
	portfolio.pushOption(Vanila, Short, Put, 210, Date(2020,3,15), 1);
	portfolio.pushOption(Vanila, Long, Put, 190, Date(2020,1,10), 2);

	portfolio.pushOption(Binary, Short, Call, 200, Date(2019,11,25), 10);
	portfolio.pushOption(Binary, Long, Call, 220, Date(2020,3,20), 25);
	portfolio.pushOption(Binary, Short, Put, 200, Date(2020,2,18), 10);
	portfolio.pushOption(Binary, Long, Put, 210, Date(2019,12,19), 10);
	portfolio.pushOption(Binary, Long, Put, 190, Date(2020,1,15), 20);
	portfolio.pushOption(Binary, Short, Call, 205, Date(2020,2,15), 20);


	return 0;
}
