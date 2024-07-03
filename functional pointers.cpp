#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;
typedef unsigned long long int ull;


ull oddSum = 0;
ull evenSum = 0;

void findOddSum(ull start, ull end) {
	for (ull i = start; i <= end; i++) {
		if ((i & 1) == 1) {
			oddSum += i;
		}
	}
}

void findEvenSum(ull start, ull end) {
	for (ull i = start; i <= end; i++) {
		if ((i & 1) != 1) {
			evenSum += i;
		}
	}
}


int main() {
	ull start = 0, end = 1900000000;

	auto start_time = high_resolution_clock::now();

	/*std::thread t1(findOddSum, start, end);
	std::thread t2(findEvenSum, start, end);

	t1.join();
	t2.join();*/

	findOddSum(start, end);
	findEvenSum(start, end);
	auto end_time = high_resolution_clock::now();

	auto time_taken = duration_cast <microseconds> (end_time - start_time);
	cout << oddSum << endl;
	cout << evenSum << endl;
	cout << "Time Taken" << time_taken.count() / 1000000 << endl;
}