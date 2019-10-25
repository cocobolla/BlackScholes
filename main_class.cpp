#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include "plainvanilla_option.h"
#include "binary_option.h"

void print_option(Option* x) {
	std::cout << "price = " << x->price() << std::endl;
	std::cout << "mc = " << x->mcprice(10000) << std::endl;
	std::cout << "bnt = " << x->bntprice(100) << std::endl;
	std::cout << std::string(30, '-') << std::endl;
}

int main() {
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
