/*
Deadlocks
*/

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>

std::mutex mt1, mt2 , mt3 , mt4;

void printNumber(int num) {
	
	
	int count = 0;
	while (true) {
		std::lock(mt1, mt2);
		std::lock(mt3, mt4);
		
		/*mt1.lock();
		mt2.lock();
		mt3.lock();
		mt4.lock();*/
		std::cout << "Thread 1 Execution : " << num << " " << count++ << std::endl;
		mt1.unlock();
		mt2.unlock();
		mt3.unlock();
		mt4.unlock();
	}

	
	
}

void printNumber2(int num) {

	
	
	int count = 0;
	while (true) {
		std::lock(mt3, mt4);
		std::lock(mt1, mt2);
		/*mt3.lock();
		mt4.lock();
		mt2.lock();
		mt1.lock();*/
		std::cout << "Thread 2 Execution : " << num << " " << count++ << std::endl;
		mt3.unlock();
		mt4.unlock();
		mt2.unlock();
		mt1.unlock();
	}
	
	
	
	
	
}

int main() {
	std::thread t1(printNumber, 10);
	std::thread t2(printNumber2, 12);

	t1.join();
	t2.join();

	return 0;
}