#include "option_portfolio.h"

void OptionPortfolio::pushOption(OptionProduct op, Position p, OptionType type, 
	double strike, Date expiration, int q)
{
	Option* option = factory_.CreateOption(op, type, strike, expiration);
	double rf = y_term_.value(expiration);
	double vol = v_term_.value(expiration);
	double div = d_term_.value(expiration);
	GBMProcess process(spot_, rf, div, vol);

	option->setEvalDate(expiration);
	option->setProcess(process);

	option_arr.push_back(option);
	position_arr.push_back(p);
	quantity_arr.push_back(q);
}

void OptionPortfolio::printOption(Option* x) {
	std::cout << "price = " << x->price() << std::endl;
	std::cout << "mc = " << x->mcprice(10000) << std::endl;
	std::cout << "bnt = " << x->bntprice(100) << std::endl;
	std::cout << std::string(30, '-') << std::endl;
}

void OptionPortfolio::printPortfolio() {}


OptionPortfolio::~OptionPortfolio() {
	for (unsigned int i = 0; i < option_arr.size(); ++i)
		delete option_arr[i];
}
