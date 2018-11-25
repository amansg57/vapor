//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198
#pragma once

#include <string>

class Game
{
public:
	using GameId = int;

	Game(const GameId& gameId, const std::string& title, const std::string& desc, const double& price, const int& age_rating)
		: m_gameId(gameId)
		, m_title(title)
		, m_description(desc)
		, m_price(price)
		, m_age_rating(age_rating)
	{}

	virtual ~Game() {}

	const Game::GameId get_game_id() const { return m_gameId; }

	const std::string get_title() const { return m_title; }

	const std::string get_desc() const { return m_description; }

	void set_title(const std::string& val) { m_title = val; }

	const double get_price() const { return m_price; }

	const int get_age_rating() const { return m_age_rating; }

private:
	const GameId m_gameId;	// Numerical identifier for a game  (unique key).
	std::string m_title; // The game title
	std::string m_description; // Some information about the game.
	double m_price;
	int m_age_rating;
};
