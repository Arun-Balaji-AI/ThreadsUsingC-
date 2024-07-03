/*
Async ==> It is used to do the job of promise and future without using the promise. We just need to use future. 
It runs a function asynchronously and returns a std::future value that will hold the result.

There are three launch policies(How the async function should work)
1. std::launch::asyn ==> creates a thread.
2. std::launch::deferred ==> doesnt creates a thread.
3. std::launch::async | std::launch::deferred ==> System chooses the launch policy
*/


#include <iostream>
#include <thread>
#include <future>
#include <chrono>

int balance = 0;

int addMoney(int money) {
	int prev_balance = balance;
	balance += money;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return prev_balance;
}

int main() {
	std::future<int> PrevBalance = std::async(std::launch::async, addMoney , 1520);

	std::cout << "Waiting to finish the execution!!!" << std::endl;
	std::cout << "Previous Balance : " << PrevBalance.get() << std::endl;
	std::cout << "Current Balance : " << balance << std::endl;

	std::future<int> PrevBalance1 = std::async(std::launch::deferred, addMoney, 1520);
	std::cout << "Waiting to finish the execution!!!" << std::endl;
	std::cout << "Previous Balance : " << PrevBalance1.get() << std::endl;
	std::cout << "Current Balance : " << balance << std::endl;

	return 0;
}