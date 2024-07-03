#include <iostream>
#include <vector>
#include <thread>
#include <semaphore>
#include <string.h>
#include <set>
#include <chrono>
#include <mutex>

std::binary_semaphore sem(1);
std::set<std::string> database;
std::mutex mt;

void add_details_to_database(std::vector<std::string> names) {

	//std::lock_guard<std::mutex> lock(mt);
	sem.acquire();
	//critical Section
	std::cout << "Entered Critical Section : " << std::this_thread::get_id() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	for (std::string name : names) {
		std::cout << name << " " << std::this_thread::get_id() << std::endl;
		database.insert(name);
	}
	//mt.unlock();
	sem.release();
}

int main() {
	std::vector<std::string> names1 = { "Arun" , "Balaji" , "Rithick" , "Gojo" , "Paagal" };
	std::vector <std::string> names2 = { "Dharaneesh" , "Mridul" , "NaveenR" , "Thilak" };

	std::thread t1(add_details_to_database, names1);
	std::thread t2(add_details_to_database, names2);
	//sem.release();

	t1.join();
	t2.join();

	for (std::string it : database) {
		std::cout << it << std::endl;
	}

	return 0;
}