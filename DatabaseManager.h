//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198
#pragma once

#include "Game.h"
#include "Users.h"
#include "Play.h"
#include "Purchase.h"

#include <map>

//--
// DatabaseManager is the one central database for the whole system
// In this example it has been designed using the singleton pattern.
//--
class DatabaseManager
{
public:
	// Singleton instance definition.
	static DatabaseManager& instance();

	// Initialize the database from storage.
	void load_data();

	// Write all the data to storage.
	void store_data();

	// Adds a user to the db.
	void add_user(UserBase* pUser);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const UserBase::Username& username);

	// iterating users using visitor pattern
	template<typename Visitor> void visit_users(Visitor& func)
	{
		for (auto it : m_users) { func(*it.second); }
	}

	// Adds a game to the db.
	void add_game(Game& rGame);

	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);

	// Removes a game from the container by id.
	void remove_game(const Game::GameId gameid);

	// iterating games using visitor pattern
	template<typename Visitor> void visit_games(Visitor& func)
	{
		for (auto it : m_games) { func(it.second); }
	}

	// Adds plays, new play and importing play from file
	void add_play(const std::string& username, const Game::GameId& gameid, const int& length);
	void add_play(const std::string& username, const Game::GameId& gameid, const int& length, const std::string& dateTimeStr);

	// iterating plays using visitor pattern
	template<typename Visitor> void visit_plays(Visitor& func)
	{
		for (auto it : m_plays) { func(it); }
	}

	int find_avg_playtime_of_game(const Game::GameId& gameid);

	int find_total_playtime_of_game(const Game::GameId& gameid);

	int find_avg_playtime_of_user(const std::string& username);

	int find_total_playtime_of_user(const std::string& username);

	// Adds purchases, new purchase and importing purchase from file
	void add_purchase(const std::string& username, const Game::GameId& gameid, const double& price);
	void add_purchase(const std::string& username, const Game::GameId& gameid, const double& price, const std::string& dateTimeStr);

	// iterating purchases using visitor pattern
	template<typename Visitor> void visit_purchases(Visitor& func)
	{
		for (auto it : m_purchases) { func(it); }
	}

	int get_no_of_purchases_for_game(const Game::GameId& gameid);

private:
	// Constructors are private for singleton pattern.
	DatabaseManager();
	~DatabaseManager();


private:
	// Types
	using UserContainer = std::map<UserBase::Username, UserBase*>;
	using GameContainer = std::map<Game::GameId, Game>;
	using PlayContainer = std::list<Play>;
	using PurchaseContainer = std::list<Purchase>;

	UserContainer m_users;
	GameContainer m_games;
	PlayContainer m_plays;
	PurchaseContainer m_purchases;
};

