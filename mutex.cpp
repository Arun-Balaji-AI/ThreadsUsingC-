/*
Mutex : Mutual Exclusion

Race Condition :
1. Race Condition is a condition where two or more threads happened to change a common data at the same time.
2. Ih there is Race Condition, we must protect it and that section is called critical section.

Mutex -> Used to avoid a race condition.
lock() and unlock() is used to execute mutex to avoid race condition.

std :: mutex :: try_lock() -> A member function of mutex. This tries to lock the mutex. On successful lock acquisition it returns true
else false. If if try_lock() is not able to lock mutex, then it doesn't get blocked. This is known as non-blocking. If try_lock() is called
againby the same thread, then the behavior is undefined.
It is a deadlock situation between the undefined behaviour (If the same thread is able to lock the same mutex more than one time,
then go for recursive_mutex .)


try_lock() functions:
1. std :: try_lock()
2. std :: mutex :: try_lock()
3. std :: shared_lock :: try_lock()
4. std :: timed_mutex :: try_lock()
5. std :: unique_lock :: try_lock()
6. std :: shared_mutex :: try_lock()
7. std :: recursive_mutex :: try_lock()
8. std :: shared_timed_mutex :: try_lock()
9. std :: recursive_timed_mutex :: try_lock()
*/


#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

using namespace std;
using namespace std::chrono;
vector<vector<int>> nums;
std::mutex m;
int counter = 0;
void insertValues(vector<int> temp) {
	if (m.try_lock()) {
		nums.push_back(temp);
		m.unlock();
	}
}

void countValues() {
	for (int i = 0; i < 10000; i++) {
		if (m.try_lock()) {
			++counter;
			m.unlock();
		}
	}
}

int main() {
	cout << std::thread::hardware_concurrency() << std::endl;
	std::thread t1(insertValues, vector<int>{ 1 , 2 , 3 });
	std::thread t2(countValues);
	std::thread t3(countValues);

	cout << "Before Joining : " << endl;

	t1.join();
	t2.join();
	t3.join();
	
	cout << "After Joining..." << endl;
	
	for (auto i : nums) {
		for (auto j : i) {
			cout << j << " ";
		}
		cout << endl;
	}

	cout << "Counted for : " << counter << endl;
	return 0;
}