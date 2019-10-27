#pragma once
#include "term_structure.h"
#include "option.h"
#include "binary_option.h"
#include "plainvanilla_option.h"
#include "option_factory.h"

enum Position { Long = 1, Short = -1 };
enum Caltype { Add = 0, Mul = 1 };

class OptionPortfolio
{
public:
	OptionPortfolio(YieldTermStructure y_term,
		VolatilityTermStructure v_term,
		DividendTermStructure d_term,
		double spot, Date eval_date)
		:y_term_(y_term), v_term_(v_term), d_term_(d_term), 
			spot_(spot), eval_date_(eval_date) {}

	void pushOption(OptionProduct op, Position p, OptionType t,
		double strike, Date expiration, int q = 1);
	static void printOption(Option* x);
	void initialize();
	void calculate();
	void calculate(int num);
	void printPortfolio();
	void resetProcess();
	void changeSpot(double d, Caltype t);
	void changeYield(double d, Caltype t);
	void changeVolatility(double d, Caltype t);
	double price() { return total_price_; }
	double delta() { return total_delta_; }
	double gamma() { return total_gamma_; }
	double vega() { return total_vega_; }
	double rho() { return total_rho_; }
	double theta() { return total_theta_; }
	~OptionPortfolio();

private:
	YieldTermStructure y_term_;
	VolatilityTermStructure v_term_;
	DividendTermStructure d_term_;
	double spot_;
	Date eval_date_;

	std::vector<Option*> option_arr_;
	std::vector<Position> position_arr_;
	std::vector<int> quantity_arr_;
	OptionFactory factory_;

	std::vector<double> price_arr_;
	std::vector<double> mc_arr_;
	std::vector<double> bnt_arr_;
	std::vector<double> delta_arr_;
	std::vector<double> gamma_arr_;
	std::vector<double> vega_arr_;
	std::vector<double> rho_arr_;
	std::vector<double> theta_arr_;

	double total_price_;
	double total_delta_;
	double total_gamma_;
	double total_vega_;
	double total_rho_;
	double total_theta_;
};



