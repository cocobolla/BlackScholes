#include "option_portfolio.h"

void OptionPortfolio::pushOption(OptionProduct op, Position p, OptionType type, 
	double strike, Date expiration, int q)
{
	Option* option = factory_.CreateOption(op, type, strike, expiration);
	double rf = y_term_.value(expiration);
	double vol = v_term_.value(expiration);
	double div = d_term_.value(expiration);
	GBMProcess process(spot_, rf, div, vol);

	option->setEvalDate(eval_date_);
	option->setProcess(process);

	option_arr_.push_back(option);
	position_arr_.push_back(p);
	quantity_arr_.push_back(q);
}

void OptionPortfolio::printOption(Option* x) {
	std::cout << "price = " << x->price() << std::endl;
	std::cout << "mc = " << x->mcprice(10000) << std::endl;
	std::cout << "bnt = " << x->bntprice(100) << std::endl;
	std::cout << std::string(30, '-') << std::endl;
}

void OptionPortfolio::calculate() {
	//TODO Calculate from where the size is different
	//if (option_arr_.size() != price_arr_.size()) {
	double total_price = 0;
	double total_delta = 0;
	double total_gamma = 0;
	double total_vega = 0;
	double total_rho = 0;
	double total_theta = 0;
	for (unsigned int i = 0; i < option_arr_.size(); i++) {
		double price = option_arr_[i]->price();
		double mc = option_arr_[i]->mcprice(10000);
		double bnt = option_arr_[i]->bntprice(100);
		double delta = option_arr_[i]->delta();
		double gamma = option_arr_[i]->gamma();
		double vega = option_arr_[i]->vega();
		double rho = option_arr_[i]->rho();
		double theta = option_arr_[i]->theta();

		price_arr_.push_back(price);
		mc_arr_.push_back(mc);
		bnt_arr_.push_back(bnt);
		delta_arr_.push_back(delta);
		gamma_arr_.push_back(gamma);
		vega_arr_.push_back(vega);
		rho_arr_.push_back(rho);
		theta_arr_.push_back(theta);

		Position pos = position_arr_[i];
		int quantity = quantity_arr_[i];
		int constant = pos * quantity;

		total_price += (price * constant);
		total_delta += (delta * constant);
		total_gamma += (gamma * constant);
		total_vega += (vega * constant);
		total_rho += (rho * constant);
		total_theta += (theta * constant);
	}
	total_price_ = total_price;
	total_delta_ = total_delta;
	total_gamma_ = total_gamma;
	total_vega_ = total_vega;
	total_rho_ = total_rho;
	total_theta_ = total_theta;
	//}
}

void OptionPortfolio::printPortfolio() {
	//if (option_arr_.size() != price_arr_.size())
	calculate();

	for (unsigned int i = 0; i < option_arr_.size(); i++) {
		double price = price_arr_[i];
		double mc = mc_arr_[i];
		double bnt = bnt_arr_[i];
		double delta = delta_arr_[i];
		double gamma = gamma_arr_[i];
		double vega = vega_arr_[i];
		double rho = rho_arr_[i];
		double theta = theta_arr_[i];
		
		std::cout << "Option " << i + 1 << std::endl;
		option_arr_[i]->printInfo();
		std::cout << std::endl;
		std::cout << "price = " << price << "\t";// << std::endl;
		std::cout << "mc = " << mc << "\t";
		std::cout << "bnt = " << bnt << std::endl;
		std::cout << std::endl;
		std::cout << "Delta = " << delta << "\t";
		std::cout << "Gamma = " << gamma << "\t";
		std::cout << "Vega = " << vega << "\t";
		std::cout << "Rho = " << rho << "\t";
		std::cout << "Theta = " << theta << "\t" << std::endl;
		std::cout << std::string(30, '-') << std::endl;
	}
	std::cout << std::string(30, '-') << std::endl;
	std::cout << "Option Portfolio" << std::endl;
	std::cout << "Price = " << total_price_ << std::endl;
	std::cout << "Delta = " << total_delta_ << std::endl;
	std::cout << "Gamma = " << total_gamma_ << std::endl;
	std::cout << "Vega = " << total_vega_ << std::endl;
	std::cout << "Rho = " << total_rho_ << std::endl;
	std::cout << "Theta= " << total_theta_ << std::endl;

}

OptionPortfolio::~OptionPortfolio() {
	for (unsigned int i = 0; i < option_arr_.size(); ++i)
		delete option_arr_[i];
}
