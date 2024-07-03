#include <iostream>
#include <atomic>
#include <thread>


class LockThreads {
private:
	volatile bool lock_value{ false };
public:
	void lock() {
		while (__sync) {

			std::this_thread::yield();
		}
	}

	void unlock() {
		lock_value = false;
	}
};


int val = 0;

void increment(LockThreads& locker , int count) {
	for (int i = 0; i < count; i++) {
		locker.lock();
		val++;
		locker.unlock();
	}
}

int main() {
	LockThreads locker;
	std::thread t1(increment, std::ref(locker), 1000000);
	std::thread t2(increment, std::ref(locker), 1000000);

	t1.join();
	t2.join();

	std::cout << val << std::endl;

	return 0;
}