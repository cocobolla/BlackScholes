#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include "plainvanilla_option.h"
#include "binary_option.h"

void print_option(Option* x) {
	std::cout << "price = " << x->price() << std::endl;
	std::cout << "mc = " << x->mcprice(10000) << std::endl;
	std::cout << "bnt = " << x->bntprice(100) << std::endl;
	std::cout << std::string(30, '-') << std::endl;
}

int main1() {
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
int main() {
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
