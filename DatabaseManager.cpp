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
			PlayerUser* pUser = new PlayerUser(un, pw, email);
			std::string games, game, funds;
			std::getline(lineS, games, ',');
			std::stringstream gamesS(games);
			while (std::getline(gamesS, game, '|')) {
				pUser->add_game(std::stoi(game));
			}
			std::getline(lineS, funds, ',');
			pUser->set_funds(std::stoi(funds));
			add_user(pUser);
			break;
		}
	}

	add_game(Game(4789, "Bounceback", "A platform puzzle game for PSP", 4.99, 12));
	add_game(Game(5246, "Piecefall", "A tetris like 3d puzzle game for PS4", 9.99, 7));
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
			for (auto const& i : ppu->get_game_list()) {
				fout_users << i << "|";
			}
			fout_users << "," << ppu->get_available_funds() << "\n";
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
