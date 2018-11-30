//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "DatabaseManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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
	// Load Users
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
		{
			PlayerUser* pUser = new PlayerUser(elements.at(1), elements.at(2), elements.at(3), std::stoi(elements.at(6)));
			std::string game;
			std::stringstream gamesS(elements.at(4));
			while (std::getline(gamesS, game, '|')) {
				pUser->add_game(std::stoi(game));
			}
			pUser->set_funds(std::stod(elements.at(5)));
			add_user(pUser);
			break;
		}
		case UserTypeId::kGameStudio:
		{
			GameStudio* pStudio = new GameStudio(elements.at(1), elements.at(2), elements.at(3));
			std::string game;
			std::stringstream gamesS(elements.at(4));
			while (std::getline(gamesS, game, '|')) {
				pStudio->add_game(std::stoi(game));
			}
			add_user(pStudio);
		}
		}
	}
	fin_users.close();

	// Load Games
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

	// Load Plays
	std::ifstream fin_plays("data\\plays.txt");
	while (std::getline(fin_plays, line)) {
		std::string element;
		std::stringstream lineS(line);
		std::vector<std::string> elements;
		while (std::getline(lineS, element, ',')) {
			elements.push_back(element);
		}
		add_play(elements.at(0), std::stoi(elements.at(1)), std::stoi(elements.at(2)), elements.at(3));
	}
	fin_plays.close();

	// Load Purchases
	std::ifstream fin_purchases("data\\purchases.txt");
	while (std::getline(fin_purchases, line)) {
		std::string element;
		std::stringstream lineS(line);
		std::vector<std::string> elements;
		while (std::getline(lineS, element, ',')) {
			elements.push_back(element);
		}
		add_purchase(elements.at(0), std::stoi(elements.at(1)), std::stod(elements.at(2)), elements.at(3));
	}
	fin_purchases.close();

	// Load Guests
	std::ifstream fin_guests("data\\guests.txt");
	std::string email;
	while (std::getline(fin_guests, email, ',')) {
		Guest::instance().add_email(email);
	}
	fin_guests.close();
}

void DatabaseManager::store_data()
{
	// Store Users
	std::ofstream fout_users("data\\users.txt");
	auto userVisitorLambda = [&fout_users](const UserBase& rUser) {
		fout_users << static_cast<int>(rUser.get_user_type()) << "," << rUser.get_username() << ","
			<< rUser.get_password() << "," << rUser.get_email();
		switch (rUser.get_user_type()) {
		case UserTypeId::kAdminUser :
			fout_users << "\n";
			break;
		case UserTypeId::kPlayerUser :
		{
			const PlayerUser* ppu(dynamic_cast<const PlayerUser*>(&rUser));
			fout_users << ",";
			for (auto const& gameid : ppu->get_game_list()) {
				fout_users << gameid << "|";
			}
			fout_users << "," << ppu->get_available_funds() << "," << ppu->get_age() << "\n";
			break;
		}
		case UserTypeId::kGameStudio :
		{
			const GameStudio* pgs(dynamic_cast<const GameStudio*>(&rUser));
			fout_users << ",";
			for (auto const& gameid : pgs->get_game_list()) {
				fout_users << gameid << "|";
			}
			fout_users << "\n";
			break;
		}
		}
	};
	visit_users(userVisitorLambda);
	fout_users.close();

	// Store Games
	std::ofstream fout_games("data\\games.txt");
	auto gameVisitorLambda = [&fout_games](const Game& rGame) {
		fout_games << rGame.get_game_id() << "," << rGame.get_title() << "," << rGame.get_desc() << ","
			<< rGame.get_price() << "," << rGame.get_age_rating() << "\n";
	};
	visit_games(gameVisitorLambda);
	fout_games.close();

	// Store Plays
	std::ofstream fout_plays("data\\plays.txt");
	auto playVisitorLambda = [&fout_plays](const Play& rPlay) {
		fout_plays << rPlay.get_player() << "," << rPlay.get_gameid() << "," << rPlay.get_length() << ","
			<< rPlay.get_dateTime()->to_string() << "\n";
	};
	visit_plays(playVisitorLambda);
	fout_plays.close();

	// Store Purchases
	std::ofstream fout_purchases("data\\purchases.txt");
	auto purchaseVisitorLambda = [&fout_purchases](const Purchase& rPurchase) {
		fout_purchases << rPurchase.get_player() << "," << rPurchase.get_gameid() << ","
			<< rPurchase.get_price() << "," << rPurchase.get_dateTime()->to_string() << "\n";
	};
	visit_purchases(purchaseVisitorLambda);
	fout_purchases.close();

	// Store Guests
	std::ofstream fout_guests("data\\guests.txt");
	for (auto i : Guest::instance().get_emails()) {
		fout_guests << i << ",";
	}

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

void DatabaseManager::add_play(const std::string& username, const Game::GameId& gameid, const int& length)
{
	m_plays.push_back(Play(username, gameid, length));
}

void DatabaseManager::add_play(const std::string& username, const Game::GameId& gameid, const int& length, const std::string& dateTimeStr)
{
	m_plays.push_back(Play(username, gameid, length, dateTimeStr));
}

int DatabaseManager::find_avg_playtime_of_game(const Game::GameId& gameid)
{
	auto getPlaysLambda = [gameid](const Play& rPlay) {
		return (rPlay.get_gameid() == gameid);
	};
	std::list<Play>::iterator it = std::find_if(m_plays.begin(), m_plays.end(), getPlaysLambda);
	int count(0), accum(0);
	while (it != m_plays.end()) {
		accum += it->get_length();
		++count;
		it = std::find_if(++it, m_plays.end(), getPlaysLambda);
	}
	if (count != 0) { return (accum / count); }
	else return 0;
}

int DatabaseManager::find_total_playtime_of_game(const Game::GameId& gameid)
{
	auto getPlaysLambda = [gameid](const Play& rPlay) {
		return (rPlay.get_gameid() == gameid);
	};
	std::list<Play>::iterator it = std::find_if(m_plays.begin(), m_plays.end(), getPlaysLambda);
	int accum(0);
	while (it != m_plays.end()) {
		accum += it->get_length();
		it = std::find_if(++it, m_plays.end(), getPlaysLambda);
	}
	return accum;
}

int DatabaseManager::find_avg_playtime_of_user(const std::string& username)
{
	if (find_user(username)->get_user_type() != UserTypeId::kPlayerUser) return -1;
	auto getPlaysLambda = [username](const Play& rPlay) {
		return (rPlay.get_player() == username);
	};
	std::list<Play>::iterator it = std::find_if(m_plays.begin(), m_plays.end(), getPlaysLambda);
	int count(0), accum(0);
	while (it != m_plays.end()) {
		accum += it->get_length();
		++count;
		it = std::find_if(++it, m_plays.end(), getPlaysLambda);
	}
	if (count != 0) { return (accum / count); }
	else return 0;
}

int DatabaseManager::find_total_playtime_of_user(const std::string& username)
{
	if (find_user(username)->get_user_type() != UserTypeId::kPlayerUser) return -1;
	auto getPlaysLambda = [username](const Play& rPlay) {
		return (rPlay.get_player() == username);
	};
	std::list<Play>::iterator it = std::find_if(m_plays.begin(), m_plays.end(), getPlaysLambda);
	int accum(0);
	while (it != m_plays.end()) {
		accum += it->get_length();
		it = std::find_if(++it, m_plays.end(), getPlaysLambda);
	}
	return accum;
}

void DatabaseManager::add_purchase(const std::string& username, const Game::GameId& gameid, const double& price)
{
	m_purchases.push_back(Purchase(username, gameid, price));
}

void DatabaseManager::add_purchase(const std::string& username, const Game::GameId& gameid, const double& price, const std::string& dateTimeStr)
{
	m_purchases.push_back(Purchase(username, gameid, price, dateTimeStr));
}

int DatabaseManager::get_no_of_purchases_for_game(const Game::GameId& gameid) {
	auto getPurchaseLambda = [gameid](const Purchase& rPurchase) {
		return (rPurchase.get_gameid() == gameid);
	};
	std::list<Purchase>::iterator it = std::find_if(m_purchases.begin(), m_purchases.end(), getPurchaseLambda);
	int count(0);
	while (it != m_purchases.end()) {
		++count;
		it = std::find_if(++it, m_purchases.end(), getPurchaseLambda);
	}
	return count;
}