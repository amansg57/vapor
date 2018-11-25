//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "DatabaseManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{
	// We must delete users since we have take ownership.
	for (auto it : m_users)
	{
		if (it.second) delete it.second;
	}
}

DatabaseManager& DatabaseManager::instance()
{
	static DatabaseManager s_instance;
	return s_instance;
}

void DatabaseManager::load_data()
{
	std::ifstream fin("data\\users.txt");
	std::string line;
	while (std::getline(fin, line)) {
		std::stringstream lineS(line);
		std::string typeS, un, pw, email;
		std::getline(lineS, typeS, ',');
		UserTypeId type(static_cast<UserTypeId>(std::stoi(typeS)));
		std::getline(lineS, un, ',');
		std::getline(lineS, pw, ',');
		std::getline(lineS, email, ',');
		switch (type) {
		case UserTypeId::kAdminUser :
			add_user(new AdminUser(un, pw, email));
			break;
		case UserTypeId::kPlayerUser :
			add_user(new PlayerUser(un, pw, email));
			break;
		}
	}
}

void DatabaseManager::store_data()
{
	std::ofstream fout("data\\users.txt");
	auto userVisitorLambda = [&fout](const UserBase& rUser) {
		fout << static_cast<int>(rUser.get_user_type()) << "," << rUser.get_username() << "," 
			<< rUser.get_password() << "," << rUser.get_email() << "\n";
	};
	visit_users(userVisitorLambda);
	fout.close();
}

void DatabaseManager::add_user(UserBase* pUser)
{
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
	if (pUser)
	{
		m_users.insert(std::make_pair(pUser->get_username(), pUser));
	}
}

UserBase* DatabaseManager::find_user(const UserBase::Username& username)
{
	auto it = m_users.find(username);
	if (it != m_users.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void DatabaseManager::add_game(Game& rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame.get_game_id(), rGame));
}

Game* DatabaseManager::find_game(const Game::GameId gameid)
{
	auto it = m_games.find(gameid);
	if (it != m_games.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}
