//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198
#pragma once

#include <string>
#include <list>
#include "Game.h"
#include "UserTypeId.h"
#include <vector>

//--
// UserBase represents a user base class for the system.
//--
class UserBase
{
public:
	using Username = std::string;

	UserBase(const Username& username, const std::string& password, const std::string& email)
		: m_username(username)
		, m_password(password)
		, m_email(email)
	{}

	virtual ~UserBase() {}

	// mechanism for identifying the user type at runtime.
	virtual const UserTypeId get_user_type() const = 0;

	const std::string get_username() const { return m_username; }

	const std::string get_password() const { return m_password; }
	void set_password(const std::string& val) { m_password = val; }

	const std::string get_email() const { return m_email; }
	void set_email(const std::string& val) { m_email = val; }

private:
	const Username m_username; // The users username (unique key)
	std::string m_password; // Users password.
	std::string m_email; // Users email address.
};

//--
// PlayerUser represents a system user who owns games
//--
class PlayerUser : public UserBase
{
public:
	using GameList = std::list<Game::GameId>;

	PlayerUser(const Username& username, const std::string& password, const std::string& email, const int& age)
		: UserBase(username, password, email)
		, m_age(age)
	{}
	
	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override { return UserTypeId::kPlayerUser; }

	const PlayerUser::GameList& get_game_list() const { return m_ownedGames; }
	
	const int get_age() const { return m_age; }

	const double get_available_funds() const { return m_accountFunds; }

	void set_funds(double funds) { m_accountFunds = funds; }

	void add_game(const Game::GameId&);

	void remove_game(const Game::GameId&);

	bool does_user_own_game(const Game::GameId&);

private:
	GameList m_ownedGames; // List of owned games.
	double m_accountFunds; // The players available funds.
	int m_age; // Age of the player
};

//--
// AdminUser represents a system user who has privileges to modify the system.
//--
class AdminUser : public UserBase
{
public:
	// inherit the constructor.
	using UserBase::UserBase;

	// define the specific user type.
	virtual const UserTypeId get_user_type() const override { return UserTypeId::kAdminUser; }
};

//--
// GameStudio represents a user who manages their own games
//--
class GameStudio : public UserBase
{
public:
	using GameList = std::list<Game::GameId>;

	// inherit the constructor.
	using UserBase::UserBase;

	const PlayerUser::GameList& get_game_list() const { return m_ownedGames; }

	void add_game(const Game::GameId&);

	void remove_game(const Game::GameId&);

	bool does_user_own_game(const Game::GameId&);

	// define the specific user type.
	virtual const UserTypeId get_user_type() const override { return UserTypeId::kGameStudio; }

private:
	GameList m_ownedGames; // List of owned games.
};


//--
// Guest represents a user who is not in the system
//--
class Guest
{
public:
	// Singleton instance definition.
	static Guest& instance();

	void add_email(std::string);

	bool does_email_exist(std::string);

	const std::vector<std::string>& get_emails() { return m_emails; }

private:
	std::vector<std::string> m_emails;
};

