#pragma once
#include "option.h"
#include "binary_option.h"
#include "plainvanilla_option.h"


class OptionFactory
{
public:
	OptionFactory() {}
	Option* CreateOption(OptionProduct op, OptionType type, double strike, Date expiration) {
		Option* option = NULL;
		if (op == Vanila)
			option = new PlainVanillaOption(expiration, strike, type);
		else if (op == Binary)
			option = new BinaryOption(expiration, strike, type);

		return option;
	}
};
