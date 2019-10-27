#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include "plainvanilla_option.h"
#include "binary_option.h"
#include "term_structure.h"
#include "option_portfolio.h"

int main() {
	std::cout.precision(4);
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

	// Assignment.3
	portfolio.printPortfolio();


	// Assignment.4
	int plain_num = 6;	// No.1 ~ No.6 => Plain Vanilla Options
	portfolio.calculate(plain_num);
	double price_ori = portfolio.price();
	double delta_ori = portfolio.delta();
	double gamma_ori = portfolio.gamma();
	double vega_ori = portfolio.vega();
	double rho_ori = portfolio.rho();
	double theta_ori = portfolio.theta();

	double price = 0;
	double change_greek = 0;

	std::cout << std::endl << std::endl;
	std::cout << "시나리오\t" << "가격 변동\t" 
		<< "가격 변동(Greek)\t" << "오차\t" << std::endl;

	// Scenario for Spot
	// Spot +1%
	portfolio.changeSpot(1.01, Mul);
	portfolio.resetProcess();
	portfolio.calculate(plain_num);
	portfolio.changeSpot(1/1.01, Mul);

	price = portfolio.price();
	change_greek = delta_ori * 0.01 * spot;
	std::cout << "SPOT +1%\t" << price - price_ori << "\t\t"
		<< change_greek << "\t\t\t" << price - price_ori - change_greek << std::endl;

	// Spot -1%
	portfolio.changeSpot(0.99, Mul);
	portfolio.resetProcess();
	portfolio.calculate(6);
	portfolio.changeSpot(1/0.99, Mul);

	price = portfolio.price();
	change_greek = delta_ori * -0.01 * spot;
	std::cout << "SPOT -1%\t" << price - price_ori << "\t\t"
		<< change_greek << "\t\t\t" << price - price_ori - change_greek << std::endl;

	// Scenario for Vol
	// Vol +1%
	portfolio.changeVolatility(0.01, Add);
	portfolio.resetProcess();
	portfolio.calculate();
	portfolio.changeVolatility(-0.01, Add);

	price = portfolio.price();
	change_greek = vega_ori * 0.01;
	std::cout << "Vol +1%\t\t" << price - price_ori << "\t\t"
		<< change_greek << "\t\t\t" << price - price_ori - change_greek << std::endl;

	// Vol -1%
	portfolio.changeVolatility(-0.01, Add);
	portfolio.resetProcess();
	portfolio.calculate();
	portfolio.changeVolatility(0.01, Add);

	price = portfolio.price();
	change_greek = vega_ori * -0.01;
	std::cout << "Vol -1%\t\t" << price - price_ori << "\t\t"
		<< change_greek << "\t\t\t" << price - price_ori - change_greek << std::endl;

	// Scenario for Rf
	// Rho +10bp
	portfolio.changeYield(10/1000, Add);
	portfolio.resetProcess();
	portfolio.calculate();
	portfolio.changeYield(-10/1000, Add);

	price = portfolio.price();
	change_greek = rho_ori * 10/1000;
	std::cout << "Rf +10bp\t" << price - price_ori << "\t\t"
		<< change_greek << "\t\t\t" << price - price_ori - change_greek << std::endl;

	// Rho -10bp
	portfolio.changeYield(-10/1000, Mul);
	portfolio.resetProcess();
	portfolio.calculate();
	portfolio.changeYield(10/1000, Mul);

	price = portfolio.price();
	change_greek = rho_ori * -10/1000;
	std::cout << "Rf -10bp\t" << price - price_ori << "\t\t"
		<< change_greek << "\t\t\t" << price - price_ori - change_greek << std::endl;

	return 0;
}
