//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "Users.h"

void PlayerUser::add_game(Game::GameId gameId) {
	m_ownedGames.push_back(gameId);
}