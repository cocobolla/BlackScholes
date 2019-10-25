#pragma once
#include <string>

class Date {
public:
	Date() {}
	Date(int y, int m, int d) : y_(y), m_(m), d_(d) {}
	Date(std::string ymd);

	int year() { return y_; }
	int month() { return m_; }
	int day() { return d_; }
	int daysFrom(Date d);

	void print();
	std::string to_str();
private:
	int y_, m_, d_;
};

int daysBetween(Date d1, Date d2);