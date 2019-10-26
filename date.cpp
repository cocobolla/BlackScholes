#include "date.h"
#include <ctime>
#include <iostream>
#include <sstream>

Date::Date(std::string ymd) {
	y_ = std::atoi(ymd.substr(0, 4).c_str());
	m_ = std::atoi(ymd.substr(4, 2).c_str());
	d_ = std::atoi(ymd.substr(6, 2).c_str());
}

int Date::daysFrom(Date d) {
	std::tm a = { 0, 0, 0, d_, m_ - 1, y_ - 1900 };
	std::tm b = { 0, 0, 0, d.day(), d.month() - 1, d.year() - 1900 };
	std::time_t x = std::mktime(&a);
	std::time_t y = std::mktime(&b);
	int difference = std::difftime(y, x) / (60 * 60 * 24);
	return difference;
}

void Date::print() {
	std::cout << y_ << "/" << m_ << "/" << d_ << std::endl;
}

std::string Date::to_str() {
	std::stringstream ss;
	ss << y_ << "/" << m_ << "/" << d_;
	return ss.str();
}

bool Date::operator>(Date d) {
	int diff = (*this).daysFrom(d);
	if (diff < 0)
		return true;
	else
		return false;
}

bool Date::operator<(Date d) {
	int diff = (*this).daysFrom(d);
	if (diff > 0)
		return true;
	else
		return false;
}

bool Date::operator==(Date d) {
	bool a = !(*this > d);
	bool b = !(*this < d);
	if ( !(*this > d) && !(*this < d) )
		return true;
	else
		return false;
}

int daysBetween(Date d1, Date d2) {
	return d1.daysFrom(d2);
}