#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Pikachu {
	int health{ 0 };
	int strength{ 0 };
	int agility{ 0 };

	Pikachu(int h, int s, int a) : health(h), strength(s), agility(a){};

	bool operator<(const Pikachu& rhs) {
		return this->health < rhs.health;
	}
};


int main() {
	vector <Pikachu> pika{
		Pikachu(10 , 500 , 20),
		Pikachu(500 , 70 , 100),
		Pikachu(10 , 200 , 250)
	};

	sort(pika.begin(), pika.end());

	for (auto pikapika : pika) {
		cout << pikapika.health << " " << pikapika.strength << " " << pikapika.agility << endl;
	}

	cout << "Pika Pika Pikachu" << endl;
}