#pragma once
#include "term_structure.h"

class YieldTermStructure : public TermStructure
{
public:
	double discount(Date d);
	double forward(Date d1, Date d2);
};

