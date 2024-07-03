#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <vector>
#include <functional>

class ThreadPool {
private:
	int thread_count;
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> jobs;
	std::mutex mt;
	std::condition_variable cv;
	bool stop;
public:
	ThreadPool(int _thread_count) : thread_count(_thread_count), stop(false) {
		for (int i = 0; i < thread_count; i++) {
			threads.emplace_back([this] {
				while (true) {
					std::unique_lock<std::mutex> locker(mt);
					//Condition variable inorder to sleep while a job is not available at the moment
					cv.wait(locker, [this] { return jobs.size() > 0 || stop; });

					if (stop == true && jobs.empty()) return;
					std::function<void()> job = std::move(jobs.front());
					jobs.pop();

					locker.unlock();
					job();
				}
			});
		}
	}
	template<class F>
	void ExecuteJobs(F&& f) {
		std::unique_lock<std::mutex> locker(mt);
		jobs.push(f);
		locker.unlock();

		cv.notify_one();
	}

	~ThreadPool() {
		std::unique_lock<std::mutex> locker(mt);
		stop = true;
		locker.unlock();
		cv.notify_one();

		for (std::thread& th : threads) {
			th.join();
		}
	}
};

int main() {

	int thread_count;
	std::cin >> thread_count;

	ThreadPool pool(thread_count);

	int num_jobs;
	std::cin >> num_jobs;
	for (int i = 0; i < num_jobs; i++) {
		pool.ExecuteJobs([i] {
			std::cout << "Exection of ThreadPooling is success!!! Currently executing thread_id : " << std::this_thread::get_id() << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			//std::cin.get();
		});
	}

	return 0;
}