/*
Recursive mutex locks the mutex if the owner is same as the thread which is asking to lock. => Same thread can lock the mutex as much as want

*/

#include <iostream>
#include <thread>
#include <mutex>

int variable = 0;
std::recursive_mutex mt;
void recursiveAddition(int parameter) {
	if (parameter <= 0) return;
	std::cout << "Mutex is Locked : " << parameter << std::endl;
	mt.lock();
	variable += parameter;
	std::cout << parameter << " : " << variable << std :: endl;
	recursiveAddition(--parameter);
	std::cout << "Mutex is unlocked" << std::endl;
	mt.unlock();
}

int main() {
	std::thread t1(recursiveAddition, 5);
	std::thread t2(recursiveAddition, 6);

	t1.join();
	t2.join();

	return 0;
}