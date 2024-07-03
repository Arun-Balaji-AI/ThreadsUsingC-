/*
Using Join, detach and Joinable in threads.
Join :

It is used to call in order to finish the started thread. Double Join will result in program termination.  If needed we should check 
whether the thread is joinable through joinable().

Detach:

This is used to detach the created thread from parent thread. Always need to check whether the thread is joinable before detaching.
Double detach terminates the program.If the thread is detached and main function is returning 
then the detach thread operation is suspended.

Joinable:
Joinable function checks whether the thread is valid to perform join / detach operation.

Either join or detach need to called in the program, else the thread destructor will terminate the program.
*/




#include <iostream>
#include <string.h>
#include <chrono>
#include <thread>

using namespace std;
void printNames(string name, int times) {
	while (times-- > 0) {
		cout << name << times << endl;
	}

	std::this_thread::sleep_for(chrono::seconds(5));
}

int main() {
	int count;
	string name;

	cin >> name >> count;

	std::thread t(printNames, name, count);
	//std::thread t1(printNames, "Balaji", count);
	cout << "Main() function is executing" << endl;
	if (t.joinable()) {
		t.detach();
	}
	t.join();
	cout << "Main() function is finished" << endl;
	return 0;
}