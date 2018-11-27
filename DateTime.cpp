//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "DateTime.h"
#include <sstream>

DateTime::DateTime() {
	time_t t(time(NULL));
	localtime_s(&timeinfo, &t);
}

DateTime::DateTime(const std::string& str) {
	timeinfo = { 0 };
	int year, month, day, hours, minutes, seconds;
	sscanf_s(str.c_str(), "%d/%d/%d %d:%d:%d", &year, &month, &day, &hours, &minutes, &seconds);
	timeinfo.tm_year = year - 1900;
	timeinfo.tm_mon = month - 1;
	timeinfo.tm_mday = day;
	timeinfo.tm_hour = hours;
	timeinfo.tm_min = minutes;
	timeinfo.tm_sec = seconds;
}

const std::string DateTime::to_string() {
	std::stringstream ss;
	ss << get_year() << "/" << addLeadingZero(get_month()) << "/" << addLeadingZero(get_day()) << " ";
	ss << addLeadingZero(get_hours()) << ":" << addLeadingZero(get_minutes()) << ":" << addLeadingZero(get_seconds());
	return ss.str();
}

std::string DateTime::addLeadingZero(const int& i) {
	std::string str = std::to_string(i);
	if (str.length() == 1) {
		str = std::string(1, '0').append(str);
	}
	return str;
}