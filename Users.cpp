//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "Users.h"
#include <algorithm>

void PlayerUser::add_game(Game::GameId gameId) {
	m_ownedGames.push_back(gameId);
}

bool PlayerUser::does_user_own_game(Game::GameId gameid) {
	bool found = (std::find(m_ownedGames.begin(), m_ownedGames.end(), gameid) != m_ownedGames.end());
	return found;
}