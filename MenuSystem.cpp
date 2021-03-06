//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "MenuSystem.h"
#include "UserTypeId.h"
#include <map>
#include <cmath>

MenuSystem& MenuSystem::instance()
{
	static MenuSystem s_instance;
	return s_instance;
}

void MenuSystem::list_all_games() const
{
	auto gameVisitorLambda = [](const Game& rGame) {
		std::cout << rGame.get_title() << " (" << rGame.get_game_id() << ")" << "\n";
	};

	DatabaseManager::instance().visit_games(gameVisitorLambda);
}

void MenuSystem::list_all_users() const
{
	auto userVisitorLambda = [](const UserBase& rUser) {
		std::cout << rUser.get_username() << " " << rUser.get_email() << "\n";
	};

	DatabaseManager::instance().visit_users(userVisitorLambda);
}

int MenuSystem::run_login_screen()
{
	m_pUser = nullptr;

	// in this menu we get the username and password.
	std::string username;
	std::string password;

	std::cout << "Login Menu\n";
	std::cout << "Use login name \"guest\" to login as a guest.\n";
	std::cout << "Username: ";
	std::cin >> username;

	if (username == "guest") {
		std::string email;
		std::cout << "Please enter your email address.";
		std::cin >> email;
		if (Guest::instance().does_email_exist(email)) {
			std::cout << "Welcome back! We invite you to sign up to our service.\n";
		}
		else Guest::instance().add_email(email);
	}
	else {
		std::cout << "Password: ";
		std::cin >> password;

		// find the user and check password
		auto pUser = DatabaseManager::instance().find_user(username);
		if (pUser && pUser->get_password() == password)
		{
			m_pUser = pUser;
			std::cout << "Hi " << m_pUser->get_username() << "\n";
		}
		else
		{
			std::cout << "No such username or password!\n";
		}
	}
	
	return 0;
}

int MenuSystem::run_admin_user_menu()
{
	AdminUser* pAdminUser = static_cast<AdminUser*>(m_pUser);
	int result = 0;
	do
	{
		std::cout << "Admin Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) List All Users\n";
		std::cout << "(3) Add Game\n";
		std::cout << "(4) Add User\n";
		std::cout << "(5) Modify Game\n";
		std::cout << "(6) Access Statistics\n";
		std::cout << "(7) View Game Details\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': list_all_users(); break;
		case '3': add_game(); break;
		case '4': add_user(); break;
		case '5': modify_game(); break;
		case '6': stats_menu(); break;
		case '7': view_game_details(); break;
		case 'q': result = -1; break;
		default:  std::cout << "INVALID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;

	return 0;
}

int MenuSystem::run_gamestudio_user_menu()
{
	GameStudio* pGameStudio = static_cast<GameStudio*>(m_pUser);
	int result = 0;
	do
	{
		std::cout << "Game Studio Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) Add Game\n";
		std::cout << "(3) Modify Game\n";
		std::cout << "(4) View Game Details\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': add_game(); break;
		case '3': modify_game(); break;
		case '4': view_game_details(); break;
		case 'q': result = -1; break;
		default:  std::cout << "INVALID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;

	return 0;
}

void MenuSystem::add_game()
{
	int id, age_rating;
	double price;
	std::string name, desc;
	std::cout << "Enter a unique numeric id for the new game: ";
	std::cin >> id;
	std::cin.ignore();
	std::cout << "Enter a name for the new game: ";
	std::getline(std::cin, name);
	std::cout << "Enter a short description for the game: ";
	std::getline(std::cin, desc);
	std::cout << "Enter a price for the game: ";
	std::cin >> price;
	std::cout << "Enter an age rating for the game: ";
	std::cin >> age_rating;
	if (DatabaseManager::instance().find_game(id) == nullptr) {
		Game newGame = Game(id, name, desc, price, age_rating);
		DatabaseManager::instance().add_game(newGame);
		if (m_pUser->get_user_type() == UserTypeId::kGameStudio) {
			GameStudio* pGameStudio = static_cast<GameStudio*>(m_pUser);
			pGameStudio->add_game(id);
		}
		std::cout << "Game added successfully\n";
	}
	else std::cout << "That game id already exists\n";
}

void MenuSystem::add_user()
{
	std::string un, pw, email;
	int userType;
	std::cout << "What type of user are you creating? 1 for player, 2 for admin, 3 for gamestudio: ";
	std::cin >> userType;
	std::cout << "Username: ";
	std::cin >> un;
	std::cout << "Email: ";
	std::cin >> email;
	std::cout << "Password: ";
	std::cin >> pw;
	switch (static_cast<UserTypeId>(userType)) {
	case UserTypeId::kAdminUser :
		DatabaseManager::instance().add_user(new AdminUser(un, pw, email));
		break;
	case UserTypeId::kPlayerUser:
		int age;
		std::cout << "Age: ";
		std::cin >> age;
		DatabaseManager::instance().add_user(new PlayerUser(un, pw, email, age));
		break;
	case UserTypeId::kGameStudio:
		DatabaseManager::instance().add_user(new GameStudio(un, pw, email));
		break;
	}
}

void MenuSystem::modify_game()
{
	int id;
	std::cout << "Please enter the id of the game you wish to modify: ";
	std::cin >> id;
	Game* pg = DatabaseManager::instance().find_game(id);
	if (pg == nullptr) {
		std::cout << "That id does not exist.";
	}
	else {
		bool canModify = false;
		if (m_pUser->get_user_type() == UserTypeId::kAdminUser) canModify = true;
		else {
			GameStudio* pGameStudio = static_cast<GameStudio*>(m_pUser);
			canModify = pGameStudio->does_user_own_game(pg->get_game_id());
		}
		if (canModify) {
			int result = 0;
			do {
				std::cout << "Modify " << pg->get_title() << "\n";
				std::cout << "(1) Change Title\n";
				std::cout << "(2) Change Description\n";
				std::cout << "(3) Change Price\n";
				std::cout << "(4) Change Age Rating\n";
				std::cout << "(5) Remove Game\n";
				std::cout << "(q) Go Back\n";

				char option;
				std::cin >> option;

				switch (option)
				{
				case '1':
				{
					std::string title;
					std::cin.ignore();
					std::cout << "Enter new title: ";
					std::getline(std::cin, title);
					pg->set_title(title);
					std::cout << "New title set to " << title << "\n";
					break;
				}
				case '2':
				{
					std::string desc;
					std::cin.ignore();
					std::cout << "Enter new description: ";
					std::getline(std::cin, desc);
					pg->set_desc(desc);
					std::cout << "New description set to \"" << desc << "\"" << "\n";
					break;
				}
				case '3':
				{
					double price;
					std::cout << "Enter new price: ";
					std::cin >> price;
					pg->set_price(price);
					std::cout << "New price set to \x9C" << price << "\n";
					break;
				}
				case '4':
				{
					int age_rating;
					std::cout << "Enter new age rating: ";
					std::cin >> age_rating;
					pg->set_age_rating(age_rating);
					std::cout << "New age rating set to " << age_rating << "\n";
					break;
				}
				case '5':
					std::cout << pg->get_title() << " has been removed. \n";
					DatabaseManager::instance().remove_game(id);
					result = -1;
					break;
				case 'q': result = -1; break;
				default:  std::cout << "INVALID OPTION\n"; break;
				}
			} while (result == 0);
		}
		else std::cout << "You are not the owner of that game.";
	}
}

int MenuSystem::run_player_user_menu()
{
	PlayerUser* pPlayerUser = static_cast<PlayerUser*>(m_pUser);
	int result = 0;
	do
	{
		std::cout << "Player Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "Wallet \x9C" << pPlayerUser->get_available_funds() << "\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) List My Games\n";
		std::cout << "(3) Buy Game\n";
		std::cout << "(4) Add Funds\n";
		std::cout << "(5) Play Game\n";
		std::cout << "(6) Gift Game\n";
		std::cout << "(7) View Game Details\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': list_user_games(pPlayerUser); break;
		case '3': buy_game(pPlayerUser); break;
		case '4': add_funds(pPlayerUser); break;
		case '5': play_game(pPlayerUser); break;
		case '6': gift_game(pPlayerUser); break;
		case 'q': result = -1; break;
		default:  std::cout << "INVALID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;

	return 0;
}

void MenuSystem::list_user_games(PlayerUser* ppu) {
	for (auto const& gameid : ppu->get_game_list()) {
		Game* pg = DatabaseManager::instance().find_game(gameid);
		std::cout << pg->get_title() << " (" << pg->get_game_id() << ")\n";
	}
}

void MenuSystem::buy_game(PlayerUser* ppu) {
	Game::GameId gameid;
	std::cout << "Enter the id of the game you wish to buy: ";
	std::cin >> gameid;
	Game* pg = DatabaseManager::instance().find_game(gameid);
	if (pg != nullptr) {
		if (!ppu->does_user_own_game(gameid)) {
			if (ppu->get_age() >= pg->get_age_rating()) {
				if (ppu->get_available_funds() >= pg->get_price()) {
					ppu->add_game(gameid);
					ppu->set_funds(ppu->get_available_funds() - pg->get_price());
					DatabaseManager::instance().add_purchase(ppu->get_username(), gameid, pg->get_price());
					std::cout << "You have purchased " << pg->get_title() << "!\n";
				}
				else std::cout << "You do not have enough funds.\n";
			}
			else std::cout << "You must be " << pg->get_age_rating() << " years old to buy that game.\n";
		}
		else std::cout << "You already own that game.\n";
	}
	else std::cout << "That is not a valid game id.\n";
}

void MenuSystem::add_funds(PlayerUser* ppu) {
	double addAmount, currentFunds(ppu->get_available_funds());
	std::cout << "Current Balance: \x9C" << currentFunds << "\n";
	std::cout << "How much would you like to top up by? ";
	std::cin >> addAmount;
	ppu->set_funds(currentFunds + addAmount);
	std::cout << "\x9C" << addAmount << " has been added to your account. Your new total is \x9C" << currentFunds + addAmount << "\n";
}

void MenuSystem::play_game(PlayerUser* ppu) {
	Game::GameId gameid;
	std::cout << "Enter the id of the game you wish to play: ";
	std::cin >> gameid;
	Game* pg = DatabaseManager::instance().find_game(gameid);
	if (pg != nullptr) {
		if (ppu->does_user_own_game(gameid)) {
			int duration;
			std::cout << "How long are you playing for? (in minutes): ";
			std::cin >> duration;
			std::cout << "I hope you enjoyed your fun and interactive gaming experience.\n";
			DatabaseManager::instance().add_play(ppu->get_username(), gameid, duration);
		}
		else std::cout << "You don't own that game.\n";
	}
	else std::cout << "That is not a valid game id.\n";
}

void MenuSystem::gift_game(PlayerUser* ppu) {
	Game::GameId gameid;
	std::cout << "Enter the id of the game you wish to gift: ";
	std::cin >> gameid;
	Game* pg = DatabaseManager::instance().find_game(gameid);
	if (pg != nullptr) {
		if (ppu->does_user_own_game(gameid)) {
			std::string username;
			std::cout << "Please enter the name of the user you wish to gift " << pg->get_title() << " to: ";
			std::cin >> username;
			UserBase* pRecipient = DatabaseManager::instance().find_user(username);
			if (pRecipient != nullptr) {
				if (pRecipient->get_user_type() == UserTypeId::kPlayerUser) {
					PlayerUser* pRecipientPlayer = dynamic_cast<PlayerUser*>(pRecipient);
					if (pRecipientPlayer->get_age() >= pg->get_age_rating()) {
						ppu->remove_game(gameid);
						pRecipientPlayer->add_game(gameid);
					}
					else std::cout << "That user is not the appropriate age for this game.\n";
				}
				else std::cout << "That user is not a player user, and cannot own games.\n";
			}
			else std::cout << "That user does not exist.\n";
		}
		else std::cout << "You don't own that game.\n";
	}
	else std::cout << "That is not a valid game id.\n";
}

void MenuSystem::stats_menu() {
	int result = 0;
	do {
		std::cout << "\nStatistics Menu\n";
		std::cout << "(1) List all purchases\n";
		std::cout << "(2) List all plays\n";
		std::cout << "(3) Rank games by total time played\n";
		std::cout << "(4) Rank players by total time played\n";
		std::cout << "(5) Rank games by average time played\n";
		std::cout << "(6) Rank players by average time played\n";
		std::cout << "(7) Get average game price\n";
		std::cout << "(8) Rank games by price\n";
		std::cout << "(9) Rank games by age rating\n";
		std::cout << "(a) Rank games by sales\n";
		std::cout << "(q) Go Back\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_purchases(); break;
		case '2': list_all_plays(); break;
		case '3': rank_games_total(); break;
		case '4': rank_players_total(); break;
		case '5': rank_games_avg(); break;
		case '6': rank_players_avg(); break;
		case '7': get_avg_price(); break;
		case '8': rank_games_price(); break;
		case '9': rank_games_age(); break;
		case 'a': rank_games_sales(); break;
		case 'q': result = -1; break;
		default:  std::cout << "INVALID OPTION\n"; break;
		}
	} while (result == 0);
}

void MenuSystem::list_all_purchases() {
	auto listPurchaseLambda = [](const Purchase& rPurchase) {
		std::cout << "\nUsername: " << rPurchase.get_player() << "\n";
		Game* pg = DatabaseManager::instance().find_game(rPurchase.get_gameid());
		std::cout << "Game: " << pg->get_title() << "\n";
		std::cout << "Date and time of Purchase: " << rPurchase.get_dateTime()->to_string() << "\n";
		std::cout << "Price at time of Purchase: " << rPurchase.get_price() << "\n";
	};
	DatabaseManager::instance().visit_purchases(listPurchaseLambda);
}

void MenuSystem::list_all_plays() { 
	auto listPlayLambda = [](const Play& rPlay) {
		std::cout << "\nUsername: " << rPlay.get_player() << "\n";
		Game* pg = DatabaseManager::instance().find_game(rPlay.get_gameid());
		std::cout << "Game: " << pg->get_title() << "\n";
		std::cout << "Date and time of Purchase: " << rPlay.get_dateTime()->to_string() << "\n";
		std::cout << "Playtime in minutes: " << rPlay.get_length() << "\n";
	};
	DatabaseManager::instance().visit_plays(listPlayLambda);
}

void MenuSystem::rank_games_total() {
	std::multimap<int, Game::GameId> playtimeMap;
	auto visitGameLambda = [&playtimeMap](const Game& rGame) {
		int totalPlaytime = DatabaseManager::instance().find_total_playtime_of_game(rGame.get_game_id());
		playtimeMap.insert(std::make_pair(totalPlaytime, rGame.get_game_id()));
	};
	DatabaseManager::instance().visit_games(visitGameLambda);
	int count(1);
	for (auto it = playtimeMap.rbegin(); it != playtimeMap.rend(); ++it)
	{
		Game* pg = DatabaseManager::instance().find_game(it->second);
		std::cout << count++ << ": " << pg->get_title() << " (" << it->first << ")\n";
	}
}

void MenuSystem::rank_players_total() {
	std::multimap<int, std::string> playtimeMap;
	auto visitPlayerLambda = [&playtimeMap](const UserBase& rUser) {
		int totalPlaytime = DatabaseManager::instance().find_total_playtime_of_user(rUser.get_username());
		if (totalPlaytime != -1) playtimeMap.insert(std::make_pair(totalPlaytime, rUser.get_username()));
	};
	DatabaseManager::instance().visit_users(visitPlayerLambda);
	int count(1);
	for (auto it = playtimeMap.rbegin(); it != playtimeMap.rend(); ++it)
	{
		std::cout << count++ << ": " << it->second << " (" << it->first << ")\n";
	}
}

void MenuSystem::rank_games_avg() {
	std::multimap<int, Game::GameId> playtimeMap;
	auto visitGameLambda = [&playtimeMap](const Game& rGame) {
		int avgPlaytime = DatabaseManager::instance().find_avg_playtime_of_game(rGame.get_game_id());
		playtimeMap.insert(std::make_pair(avgPlaytime, rGame.get_game_id()));
	};
	DatabaseManager::instance().visit_games(visitGameLambda);
	int count(1);
	for (auto it = playtimeMap.rbegin(); it != playtimeMap.rend(); ++it)
	{
		Game* pg = DatabaseManager::instance().find_game(it->second);
		std::cout << count++ << ": " << pg->get_title() << " (" << it->first << ")\n";
	}
}

void MenuSystem::rank_players_avg() {
	std::multimap<int, std::string> playtimeMap;
	auto visitPlayerLambda = [&playtimeMap](const UserBase& rUser) {
		int avgPlaytime = DatabaseManager::instance().find_avg_playtime_of_user(rUser.get_username());
		if (avgPlaytime != -1) playtimeMap.insert(std::make_pair(avgPlaytime, rUser.get_username()));
	};
	DatabaseManager::instance().visit_users(visitPlayerLambda);
	int count(1);
	for (auto it = playtimeMap.rbegin(); it != playtimeMap.rend(); ++it)
	{
		std::cout << count++ << ": " << it->second << " (" << it->first << ")\n";
	}
}

void MenuSystem::get_avg_price() {
	int count(0);
	double accum(0);
	auto visitGameLambda = [&accum, &count](const Game& rGame) {
		accum += rGame.get_price();
		++count;
	};
	DatabaseManager::instance().visit_games(visitGameLambda);
	double roundedAvg = round((accum / count) * 100) / 100;
	std::cout << "Average price of games: " << roundedAvg << "\n";
}

void MenuSystem::rank_games_price() {
	std::multimap<double, std::string> priceMap;
	auto visitGameLambda = [&priceMap](const Game& rGame) {
		priceMap.insert(std::make_pair(rGame.get_price(), rGame.get_title()));
	};
	DatabaseManager::instance().visit_games(visitGameLambda);
	int count(1);
	for (auto it = priceMap.rbegin(); it != priceMap.rend(); ++it)
	{
		std::cout << count++ << ": " << it->second << " (" << it->first << ")\n";
	}
}

void MenuSystem::rank_games_age() {
	std::multimap<int, std::string> ageMap;
	auto visitGameLambda = [&ageMap](const Game& rGame) {
		ageMap.insert(std::make_pair(rGame.get_age_rating(), rGame.get_title()));
	};
	DatabaseManager::instance().visit_games(visitGameLambda);
	int count(1);
	for (auto it = ageMap.rbegin(); it != ageMap.rend(); ++it)
	{
		std::cout << count++ << ": " << it->second << " (" << it->first << ")\n";
	}
}

void MenuSystem::rank_games_sales() {
	std::multimap<int, std::string> salesMap;
	auto visitGameLambda = [&salesMap](const Game& rGame) {
		int sales = DatabaseManager::instance().get_no_of_purchases_for_game(rGame.get_game_id());
		salesMap.insert(std::make_pair(sales, rGame.get_title()));
	};
	DatabaseManager::instance().visit_games(visitGameLambda);
	int count(1);
	for (auto it = salesMap.rbegin(); it != salesMap.rend(); ++it)
	{
		std::cout << count++ << ": " << it->second << " (" << it->first << ")\n";
	}
}

void MenuSystem::view_game_details() {
	Game::GameId gameid;
	std::cout << "Enter the id of the game you want to view: ";
	std::cin >> gameid;
	Game* pg = DatabaseManager::instance().find_game(gameid);
	if (pg != nullptr) {
		std::cout << "Name: " << pg->get_title() << "\n";
		std::cout << "Description: " << pg->get_desc() << "\n";
		std::cout << "Price: \x9C" << pg->get_price() << "\n";
		std::cout << "Age Rating" << pg->get_age_rating() << "\n";
	}
	else std::cout << "That is not a valid game id.\n";
}

int MenuSystem::run_unknown_user_menu()
{
	// in this menu we get the username and password.
	int result = 0;

	std::cout << "Main Menu\n";
	std::cout << "(1) List All Games\n";
	std::cout << "(2) Login\n";
	std::cout << "(q) Quit\n";

	char option;
	std::cin >> option;

	switch (option)
	{
	case '1': list_all_games(); break;
	case '2': run_login_screen(); break;
	case 'q': result = -1;  break;
	default:  std::cout << "INVALID OPTION\n"; break;
	}

	return result;
}

int MenuSystem::run()
{
	std::cout << " _   _____   ___  ____  ___ \n"
		<< "| | / / _ | / _ \\/ __ \\/ _ \\ \n"
		<< "| |/ / __ |/ ___/ /_/ / , _/\n"
		<< "|___/_/ |_/_/   \\____/_/|_| \n";

	int result = 0;
	do
	{
		if (!m_pUser)
		{
			result = run_unknown_user_menu();
		}
		else
		{
			switch (m_pUser->get_user_type())
			{
				case UserTypeId::kPlayerUser: result = run_player_user_menu(); break;
				case UserTypeId::kAdminUser: result = run_admin_user_menu(); break;
				case UserTypeId::kGameStudio: result = run_gamestudio_user_menu(); break;
				default: result = -1; break;
			}
		}
	} while (result == 0);

	DatabaseManager::instance().store_data();
	return 0;
}
