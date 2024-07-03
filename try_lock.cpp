/*
std :: try_lock() tries to lock all the lockable objects passed it to it one by one.

std :: try_lock(m1 , m2 , m3 , ...)
If it succeds it will return -1 else it will return 0-based mutex index which it couldn't lock.
If it fails to lock any mutex,  it releases all the mutex locks it locked before.
If a call for try_lock is throwing the exception, then unlock() is called for any locked objects before rethrowing.
*/

#include <iostream>
#include <thread>
#include <algorithm>
#include <mutex>
#include <chrono>

using namespace std;
using namespace std::chrono;

std::mutex m1, m2;
int X = 0, Y = 0;
void timerFunc(int seconds) {
	std::this_thread::sleep_for(chrono::seconds(seconds));
}

void increment(int& XorY , std :: mutex& m , const char* ch) {
	for (int i = 0; i < 5; ++i) {
		m.lock();
		++XorY;
		cout << ch << " " << XorY << endl;
		m.unlock();
		timerFunc(1);
	}
}

void consume() {
	int count = 5, XplusY = 0;

	while (1) {
		int lock_variable = std::try_lock(m1, m2);
		if (lock_variable == -1) {
			if (X != 0 && Y != 0) {
				XplusY += (X + Y);
				--count;
				X = 0;
				Y = 0;
				cout << "XplusY : " << XplusY << endl;
			}
			/*XplusY += (X + Y);
			--count;
			X = 0;
			Y = 0;
			cout << "XplusY : " << XplusY << endl;*/
			m1.unlock();
			m2.unlock();

			if (count <= 0) break;
		}

		
	}
}

int main() {
	std::thread t1(increment, std::ref(X), std::ref(m1) , "X");
	std::thread t2(increment, std::ref(Y), std::ref(m2) , "Y");
	std::thread t3(consume);

	t1.detach();
	t2.detach();
	t3.detach();

	std::this_thread::sleep_for(chrono::seconds(5));

	return 0;
}