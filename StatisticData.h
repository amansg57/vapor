#pragma once
#include <iostream>
#include "DateTime.h"

// Base class for Purchase and Play
class StatisticData
{
public:
	StatisticData(const std::string& name, const int& gameid)
		: _playerUsername(name)
		, _gameid(gameid)
		, _pdateTime(new DateTime())
	{}
	StatisticData(const std::string& name, const int& gameid, const std::string& dateTimeString)
		: _playerUsername(name)
		, _gameid(gameid)
		, _pdateTime(new DateTime(dateTimeString))
	{}
	virtual ~StatisticData()
	{
		delete _pdateTime;
	}

	const std::string& get_player() const { return _playerUsername; }
	const int& get_gameid() const { return _gameid; }
	DateTime* get_dateTime() const { return _pdateTime; }

private:
	const std::string _playerUsername;
	const int _gameid;
	DateTime* _pdateTime;
};