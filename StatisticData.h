#pragma once
#include <iostream>
#include "DateTime.h"
#include "Game.h"

// Base class for Purchase and Play
class StatisticData
{
public:
	StatisticData(const std::string& name, const Game::GameId& gameid)
		: _playerUsername(name)
		, _gameid(gameid)
		, _pdateTime(new DateTime())
	{}
	StatisticData(const std::string& name, const Game::GameId& gameid, const std::string& dateTimeString)
		: _playerUsername(name)
		, _gameid(gameid)
		, _pdateTime(new DateTime(dateTimeString))
	{}
	virtual ~StatisticData()
	{}

	const std::string& get_player() const { return _playerUsername; }
	const Game::GameId& get_gameid() const { return _gameid; }
	DateTime* get_dateTime() const { return _pdateTime; }

private:
	const std::string _playerUsername;
	const Game::GameId _gameid;
	DateTime* _pdateTime;
};