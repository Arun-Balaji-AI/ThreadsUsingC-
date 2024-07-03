// timed_mutex() locks the mutex only for a particular amount of time. Meaning, a waiting thread only has to wait only for a
//particular amount of time, then it will acquire the lock.
/*
Available member functions : 
1. lock
2. try_lock_for
3. try_lock_until
4. try_lock_until
5. unlock
6. try_lock
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>


using namespace std;
using namespace std::chrono;

std::timed_mutex tm;
int accessible = 10;

void accessor(int number) {
	if (tm.try_lock_for(chrono::seconds(2))) {
		//accessible++;
		

		number += accessible++;
		cout << "Entered the thread " << number << endl;
		std::this_thread::sleep_for(chrono::seconds(5));
		tm.unlock();
	}
	else {
		cout << "Couldnt enter the thread" << number << endl;
	}
}

int main() {
	std::thread t1(accessor, 10);
	std::thread t2(accessor, 11);

	t1.join();
	t2.join();

	return 0;
}