//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#pragma once

#include <iostream>
#include <ctime>

// Class to store date and time, with functions to convert to string and back for easy storage and readability
class DateTime
{
public:
	DateTime();
	DateTime(const std::string&);

	const int& get_year() const { return timeinfo.tm_year + 1900; }
	int get_month() { return timeinfo.tm_mon + 1; }
	const int& get_day() const { return timeinfo.tm_mday; }
	const int& get_hours() const { return timeinfo.tm_hour; }
	const int& get_minutes() const { return timeinfo.tm_min; }
	const int& get_seconds() const { return timeinfo.tm_sec; }
	const std::string to_string();

private:
	tm timeinfo;
	std::string addLeadingZero(const int&);
};