/*
Useful for synchronizing threads.
-> Notify other threads
-> Wait for conditions

Conditional variables waits for some certain condition, once the condition is fulfilled, it notifies the waiting thread using,
1. notify_one()
2. notify_all()

We need mutex to use conditional variable
If some thread want to wait on some condition, it has to do,
1. Acquire mutex lock using unique_lock
2. Execute wait,wait_for, wait_until. These operations automatically release the mutex and suspend the execution of the thread.
3. When the conditional variable is notified, the thread is awakened to perform the task, meaning the lock is acquired.
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mt;
std::condition_variable cv;
int balance = 0;

void withdraw(int amount) {
	std::unique_lock<std::mutex> lock(mt);

	cv.wait(lock, [] {return balance > 0 ? true : false; });

	if (balance >= amount) {
		balance -= amount;
		std :: cout << "Amount is deduced : " << amount << std::endl;
	}
	else {
		std::cout << "Balance low please add more balance withdraw the required amount" << std::endl;
	}

	std::cout << "Balance Amount is : " << balance << std::endl;
}

void addBalance(int amount) {
	std::unique_lock<std::mutex> lock(mt);
	balance += amount;
	cv.notify_all();
}


int main() {
	std::thread t1(withdraw, 998);
	std::thread t2(addBalance, 999);
	std::thread t3(withdraw, 100);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}