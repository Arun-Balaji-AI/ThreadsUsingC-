/*
unique_lock class is a mutex ownership wrapper.
It allows to ,
1. have different locking strategies
2. time constrained attempts at locking
3. recursive locking
4. transfer of lock ownership -> move not copy
5. condition variables

Locking Strategies:
1. defer_lock -> do not acquire the ownership of the mutex.
2. try_to_lock -> try to acquire the ownership of the lock without blocking.
3. adopt_lock -> assume the calling thread already has the ownership of the mutex.
*/


#include <iostream>
#include <thread>
#include <mutex>

int variable = 0;
std::mutex mt;

static void incrementor(int parameter) {

	std::unique_lock<std::mutex> lock(mt , std :: try_to_lock);
	std::cout << "Acquired Lock : " << parameter << std::endl;
	//lock.lock();
	variable += parameter;
	//mt.unlock();
	std::cout << "Released Lock : " << parameter << " " << variable << std::endl;
}

int main() {
	std::thread t1(incrementor, 5);
	std::thread t2(incrementor, 6);
	t1.join();
	t2.join();

	return 0;
}