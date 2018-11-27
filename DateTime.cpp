//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "DateTime.h"
#include <sstream>

DateTime::DateTime() {
	time_t t(time(NULL));
	tm timeinfo;
	localtime_s(&timeinfo, &t);
	year = timeinfo.tm_year + 1900;
	month = timeinfo.tm_mon + 1;
	day = timeinfo.tm_mday;
	hours = timeinfo.tm_hour;
	minutes = timeinfo.tm_min;
	seconds = timeinfo.tm_sec;
}

DateTime::DateTime(const std::string& str) {
	sscanf_s(str.c_str(), "%d/%d/%d %d:%d:%d", &year, &month, &day, &hours, &minutes, &seconds);
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