//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198
#pragma once

#include <iostream>

#include "DatabaseManager.h"

class UserBase;

class MenuSystem
{
public:

	// Singleton instance definition.
	static MenuSystem& instance();

	// run the system 
	int run();

private:
	void list_all_games() const;
	void list_all_users() const;
	int run_login_screen();
	int run_admin_user_menu();
	int run_player_user_menu();
	int run_unknown_user_menu();
	void add_game();
	void add_user();
	void modify_game();
	void list_user_games(PlayerUser*);
	void buy_game(PlayerUser*);
	void add_funds(PlayerUser*);
	void play_game(PlayerUser*);
	void gift_game(PlayerUser*);
	void stats_menu();
	void list_all_purchases();
	void list_all_plays();
	void rank_games_total();
	void rank_players_total();
	void rank_games_avg();
	void rank_players_avg();
	void get_avg_price();
	void rank_games_price();
	void rank_games_age();

private:
	UserBase* m_pUser = nullptr;  // the currently logged in user.
};

