#pragma once
#include <iostream>
#include "DateTime.h"
#include "StatisticData.h"

class Purchase : public StatisticData
{
public:
	Purchase(const std::string& name, const int& gameid, const double& price)
		: StatisticData(name, gameid)
		, _price(price)
	{}
	Purchase(const std::string& name, const int& gameid, const double& price, const std::string& dateTimeString)
		: StatisticData(name, gameid, dateTimeString)
		, _price(price)
	{}

	const double& get_price() const { return _price; }

private:
	const double _price;
};