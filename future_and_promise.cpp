/*
1.Promise => It is used to set some value that can be used in the main progrma which was actually used in a thread. It is basically 
used to set a value in a thread, so that it can be used later in the main function.

2.Future => It is used to get the setted value from the promise. It can also wait for the promise.
*/


#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>

int balance = 0;

void addMoney(std::promise<int>&& Balance , int add) {
	int prev = balance;
	balance += add;
	std::cout << balance << std::endl;
	Balance.set_value(prev);

}

int main() {
	std::promise<int> PrevBalance;
	std::future<int> BeforeBalance = PrevBalance.get_future();

	std::thread t(addMoney, std::move(PrevBalance), 1000);
	std::cout << "Waiting for Thread to finish execution : " << std::endl;
	int prev = BeforeBalance.get();
	std::cout << "Previous Balance : " << prev << std::endl;
	t.join();
	std::promise<int> PrevBalance2;
	std::future<int> BeforeBalance2 = PrevBalance2.get_future();
	std::thread t1(addMoney, std::move(PrevBalance2), 1520);
	std::cout << "Waiting for Thread to finish execution : " << std::endl;
	prev = BeforeBalance2.get();
	std::cout << "Previous Balance : " << prev << std::endl;
	t1.join();

	return 0;
}