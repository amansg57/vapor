//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include "MenuSystem.h"
#include "UserTypeId.h"

MenuSystem& MenuSystem::instance()
{
	static MenuSystem s_instance;
	return s_instance;
}

void MenuSystem::list_all_games() const
{
	auto gameVisitorLambda = [](const Game& rGame) {
		std::cout << rGame.get_title() << "\n";
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
	std::cout << "Username: ";
	std::cin >> username;

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
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': list_all_users(); break;
		case '3': add_game(); break;
		case '4': add_user(); break;
		case 'q': result = -1; break;
		default:  std::cout << "INAVLID OPTION\n"; break;
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
	Game newGame = Game(id, name, desc, price, age_rating);
	DatabaseManager::instance().add_game(newGame);
	std::cout << "Game added successfully\n";
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
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': std::cout << "TODO\n"; break;
		case '3': std::cout << "TODO\n"; break;
		case '4': std::cout << "TODO\n"; break;
		case 'q': result = -1; break;
		default:  std::cout << "INAVLID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;

	return 0;
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
	default:  std::cout << "INAVLID OPTION\n"; break;
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
				default: result = -1; break;
			}
		}
	} while (result == 0);

	DatabaseManager::instance().store_data();
	return 0;
}
