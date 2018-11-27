#pragma once
#include <iostream>
#include "DateTime.h"
#include "StatisticData.h"

class Play : public StatisticData
{
public:
	Play(const std::string& name, const int& gameid, const int& price)
		: StatisticData(name, gameid)
		, _length(price)
	{}
	Play(const std::string& name, const int& gameid, const int& price, const std::string& dateTimeString)
		: StatisticData(name, gameid, dateTimeString)
		, _length(price)
	{}

	const double& get_length() const { return _length; }

private:
	const int _length; // Length of time the user played in minutes
};