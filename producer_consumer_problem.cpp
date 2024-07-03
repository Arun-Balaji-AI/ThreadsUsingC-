#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
//#define maxBufferSize 20;
std::priority_queue<int, std::vector<int>, std::greater<int>>  pq;
std::condition_variable cv;
std::mutex mt;
const int maxBufferSize = 40;

void producer(int item_count) {
	while (item_count) {
		std::unique_lock<std::mutex> lock(mt);
		cv.wait(lock, []() {return pq.size() <= maxBufferSize; });

		pq.push(item_count);
		std::cout << "Produced Value : " << item_count << std::endl;
		item_count--;
		lock.unlock();
		cv.notify_one();
	}
}

void consumer() {
	while (true) {
		std::unique_lock<std::mutex> lock(mt);
		cv.wait(lock, []() { return pq.size() > 0; });
		int val = pq.top();
		pq.pop();
		std::cout << "Consumed Value : " << val << std::endl;
		lock.unlock();
		
		cv.notify_one();	
		//if (pq.size() == 0) break;
	}
}

int main() {
	std::thread t1(producer, 100);
	std::thread t2(consumer);

	t1.join();
	t2.join();
}