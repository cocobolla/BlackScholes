#pragma once

#include "date.h"
#include "GBMProcess.h"
#include "payoff.h"

enum OptionType { Call = 1, Put = -1 };
enum OptionProduct { Binary = 0, Vanila = 1 };

class Option {
public:
	Option(Date expiration, double strike, OptionType type) :
		expiration_(expiration), strike_(strike), type_(type) {}
	virtual ~Option() { 
		delete payoff_; 
	}
	void setProcess(GBMProcess p);
	void setEvalDate(Date d);
	virtual double price() = 0;
	virtual double mcprice(unsigned int numOfSimulation);
	virtual double bntprice(unsigned int nsteps);
	virtual double impliedVol(double m) { return 10; }

	// Assignment.2
	virtual double vega();
	virtual double delta();
	virtual double gamma();
	virtual double rho();
	virtual double theta();

protected:
	double getd1();
	double getd2();

	Payoff* payoff_;
	Date evalDate_;
	Date expiration_;
	double strike_;
	OptionType type_;
	GBMProcess p_;
	double s_, r_, q_, t_, sigma_;
};

struct MarketOption{
	Option* option;
	GBMProcess gbm;
	Date eval_date;
	int quantity;
};
