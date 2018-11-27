#pragma once
#include <iostream>
#include "DateTime.h"
#include "StatisticData.h"

class Play : public StatisticData
{
public:
	Play(const std::string& name, const Game::GameId& gameid, const int& length)
		: StatisticData(name, gameid)
		, _length(length)
	{}
	Play(const std::string& name, const Game::GameId& gameid, const int& length, const std::string& dateTimeString)
		: StatisticData(name, gameid, dateTimeString)
		, _length(length)
	{}

	const double& get_length() const { return _length; }

private:
	const int _length; // Length of time the user played in minutes
};