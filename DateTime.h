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

	const int& get_year() const { return year; }
	const int& get_month() { return month; }
	const int& get_day() const { return day; }
	const int& get_hours() const { return hours; }
	const int& get_minutes() const { return minutes; }
	const int& get_seconds() const { return seconds; }
	const std::string to_string();

private:
	int year, month, day, hours, minutes, seconds;
	std::string addLeadingZero(const int&);
};