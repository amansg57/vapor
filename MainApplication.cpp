//C++ Boot Camp - Task 2 - 2018-19 
//Name: Aman Gill
//Student number: 28012198

#include <iostream>
#include "DatabaseManager.h"
#include "MenuSystem.h"
#include "DateTime.h"
#include "Purchase.h"
// Entry point for the system
int main(int argc, char** argv)
{
	DateTime dt = DateTime();
	std::cout << dt.to_string() << "\n";
	DateTime dt2 = DateTime(dt.to_string());
	std::cout << dt2.to_string() << "\n";

	Purchase p = Purchase("aman", 1234, 1.99);
	std::cout << p.get_dateTime()->to_string();

	// Let the database load all the data.
	DatabaseManager::instance().load_data();
	return MenuSystem::instance().run();
}
