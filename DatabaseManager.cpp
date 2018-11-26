//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "DatabaseManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

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
	std::ifstream fin_users("data\\users.txt");
	std::string line;
	while (std::getline(fin_users, line)) {
		std::string element;
		std::stringstream lineS(line);
		std::vector<std::string> elements;
		while (std::getline(lineS, element, ',')) {
			elements.push_back(element);
		}
		switch (static_cast<UserTypeId>(std::stoi(elements.at(0)))) {
		case UserTypeId::kAdminUser:
			add_user(new AdminUser(elements.at(1), elements.at(2), elements.at(3)));
			break;
		case UserTypeId::kPlayerUser:
			PlayerUser* pUser = new PlayerUser(elements.at(1), elements.at(2), elements.at(3), std::stoi(elements.at(6)));
			std::string game;
			std::stringstream gamesS(elements.at(4));
			while (std::getline(gamesS, game, '|')) {
				pUser->add_game(std::stoi(game));
			}
			pUser->set_funds(std::stoi(elements.at(5)));
			add_user(pUser);
			break;
		}
	}
	fin_users.close();

	std::ifstream fin_games("data\\games.txt");
	while (std::getline(fin_games, line)) {
		std::string element;
		std::stringstream lineS(line);
		std::vector<std::string> elements;
		while (std::getline(lineS, element, ',')) {
			elements.push_back(element);
		}
		add_game(Game(std::stoi(elements.at(0)), elements.at(1), elements.at(2), std::stod(elements.at(3)), std::stoi(elements.at(4))));
	}
	fin_games.close();
}

void DatabaseManager::store_data()
{
	std::ofstream fout_users("data\\users.txt");
	auto userVisitorLambda = [&fout_users](const UserBase& rUser) {
		fout_users << static_cast<int>(rUser.get_user_type()) << "," << rUser.get_username() << ","
			<< rUser.get_password() << "," << rUser.get_email();
		switch (rUser.get_user_type()) {
		case UserTypeId::kAdminUser :
			fout_users << "\n";
			break;
		case UserTypeId::kPlayerUser :
			const PlayerUser* ppu(dynamic_cast<const PlayerUser*>(&rUser));
			fout_users << ",";
			for (auto const& gameid : ppu->get_game_list()) {
				fout_users << gameid << "|";
			}
			fout_users << "," << ppu->get_available_funds() << "," << ppu->get_age() << "\n";
			break;
		}
	};
	visit_users(userVisitorLambda);
	fout_users.close();

	std::ofstream fout_games("data\\games.txt");
	auto gameVisitorLambda = [&fout_games](const Game& rGame) {
		fout_games << rGame.get_game_id() << "," << rGame.get_title() << "," << rGame.get_desc() << ","
			<< rGame.get_price() << "," << rGame.get_age_rating() << "\n";
	};
	visit_games(gameVisitorLambda);
	fout_games.close();
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

void DatabaseManager::remove_game(const Game::GameId gameid)
{
	m_games.erase(gameid);
}