//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "Users.h"
#include <algorithm>

void PlayerUser::add_game(const Game::GameId& gameId) {
	m_ownedGames.push_back(gameId);
}

bool PlayerUser::does_user_own_game(const Game::GameId& gameid) {
	bool found = (std::find(m_ownedGames.begin(), m_ownedGames.end(), gameid) != m_ownedGames.end());
	return found;
}

void PlayerUser::remove_game(const Game::GameId& gameid) {
	m_ownedGames.erase(remove(m_ownedGames.begin(), m_ownedGames.end(), gameid));
}

void GameStudio::add_game(const Game::GameId& gameId) {
	m_ownedGames.push_back(gameId);
}

bool GameStudio::does_user_own_game(const Game::GameId& gameid) {
	bool found = (std::find(m_ownedGames.begin(), m_ownedGames.end(), gameid) != m_ownedGames.end());
	return found;
}

void GameStudio::remove_game(const Game::GameId& gameid) {
	m_ownedGames.erase(remove(m_ownedGames.begin(), m_ownedGames.end(), gameid));
}

Guest& Guest::instance()
{
	static Guest s_instance;
	return s_instance;
}

void Guest::add_email(std::string email) {
	if (!does_email_exist(email)) {
		m_emails.push_back(email);
	}
}

bool Guest::does_email_exist(std::string email) {
	if (std::find(m_emails.begin(), m_emails.end(), email) != m_emails.end()) return true;
	else return false;
}
