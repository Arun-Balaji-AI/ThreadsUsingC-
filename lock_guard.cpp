/*
1. Lightweight wrapper for owning mutex lock on scope basis.
2. It acquires the mutex lock the moment lock_guard object is created.
3. It automatically removes the lock when it goes out of the scope.
4. Cannot explicitly unlock lock_guard
5. cannot copy lock_guard
*/

#include <iostream>
#include <mutex>
#include <thread>

int variable = 0;
std::mutex mt;

void recursiveAddition(int parameter) {
	if (parameter <= 0) return;
	std::cout << "Mutex is Locked : " << parameter << std::endl;
	std::lock_guard<std::mutex> lock(mt);
	for (int i = 1; i <= parameter; i++) variable += parameter;
	std::cout << "Mutex is unlocked Automatically : " << parameter << " " << variable << std::endl;
}

int main() {
	std::thread t1(recursiveAddition, 5);
	std::thread t2(recursiveAddition, 6);

	t1.join();
	t2.join();

	return 0;
}