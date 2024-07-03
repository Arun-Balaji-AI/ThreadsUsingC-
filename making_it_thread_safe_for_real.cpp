/*
In this round we need to create a M-Ary Tree scratch.
In that tree we need to perform three operations.
1. lock(name , id)
2. unlock(name , id)
3. upgrade_lock(name , id)

In lock we need to lock the vertex of the given name. For locking a vertex
we need to follow some specific conditions. They are,
-> If parent node of the vertex is locked, then locking cannot be performed
-> If the node is already locked, then it can't be locked.
-> If the the node has no child nodes without locked by the same id, it can't be locked.

In unlocking we need to perform the exact opposite of the lock function.
-> While unlocking, we need to check whether the parents are locked are not.

In upgrading a node, we need to lock the ancestor, that is the parent of the node.
-> First we need to check whether node itself is locked. If so, the we cant lock it.
-> Then we need to check if all the children of the node is locked by the same id, if not then the parent can't be locked.
-> We need to check if any parent are locked, if they are locked, then we can't lock the parent (If we try to do so, we exploit the functions of the lock)

*/

#include <iostream>
#include <vector>
#include <unordered_map> 
#include <queue>
#include <set>
#include <mutex>
#include <thread>

using namespace std;

std::mutex mt;

// Class for TreeNode
class TreeNode {
public:
	string name;
	int uid;
	TreeNode* parent;
	bool is_locked;
	vector<TreeNode*> children;
	set<TreeNode*> locked_children;

	TreeNode(string n, TreeNode* parent_node) {
		name = n;
		parent = parent_node;
		uid = -1;
		is_locked = false;
	}

	void create_children(vector<string> names, TreeNode* parent_node) {
		for (auto it : names) {
			TreeNode* node = new TreeNode(it, parent_node);
			children.push_back(node);
		}
	}
};

class MaryTree {
public:
	TreeNode* root;
	unordered_map<string, TreeNode*> map_name_to_node;

	MaryTree(string name) {
		root = new TreeNode(name, nullptr);
	}
	// Creates the tree Initially
	void create_descendants(vector<string> names, int m) {
		int start_index = 1, i, n = names.size();
		queue<TreeNode*> node_container;
		node_container.push(root);
		while (!node_container.empty()) {
			TreeNode* node = node_container.front();
			node_container.pop();
			 //cout << "Parent Node  : " << node->name << endl; 
			map_name_to_node[node->name] = node;
			vector<string> node_storage;
			for (i = start_index; i < min(start_index + m, n); i++) {
				node_storage.push_back(names[i]);
			}
			node->create_children(node_storage, node);
			 //cout << "Children : " << endl;
			for (TreeNode* it : node->children) {
				node_container.push(it);
				 //cout << it->name << " ";
			}
			 //cout << endl;
			start_index = i;
		}
	}

	void inform_parents(TreeNode* curr) {
		TreeNode* parent_node = curr->parent;
		while (parent_node != nullptr) {
			parent_node->locked_children.insert(curr);
			parent_node = parent_node->parent;
		}
	}

	// This function locks the vertex in consideration with the given conditions
	bool lock(string name, int uid) {
		//cout << "I am Here!!!" << endl;
		std::unique_lock<std::mutex> lock(mt);
		TreeNode* curr = map_name_to_node[name];
		if (curr->is_locked == true || curr->locked_children.size() > 0) {
			return false;
		}
		TreeNode* parent_node = curr->parent;
		while (parent_node != nullptr) {
			if (parent_node->is_locked == true) return false;
			parent_node = parent_node->parent;
		}
		lock.unlock();
		inform_parents(curr);
		

		curr->is_locked = true;
		curr->uid = uid;
		return true;
	}

	// This function unlocks the node.
	bool unlock(string name, int uid) {
		TreeNode* curr = map_name_to_node[name];
		TreeNode* parent_node = curr->parent;

		if (curr->is_locked == false || curr->uid != uid) {
			return false;
		}
		while (parent_node != nullptr) {
			parent_node->locked_children.erase(curr);
			parent_node = parent_node->parent;
		}
		curr->is_locked = false;
		curr->uid = -1;

		return true;
	}

	bool upgrade_lock(string name, int uid) {
		TreeNode* curr = map_name_to_node[name];
		TreeNode* parent_node = curr->parent;

		if (curr->is_locked || curr->locked_children.size() == 0) return false;
		while (parent_node != nullptr) {
			if (parent_node->is_locked == true) return false;
			parent_node = parent_node->parent;
		}
		set<TreeNode*> st = curr->locked_children;
		for (TreeNode* it : st) {
			if (it->uid != uid) return false;
		}
		for (auto it : st) unlock(it->name, uid);
		lock(curr->name, uid);
		return true;
	}
};

class ImplementThreadSync {
private:
	MaryTree* tree;
	vector<pair<int, pair<string, int>>> queries;
	vector<std::thread> threads;
	int thread_count = 0;
	bool wait_variable = false;
public:
	ImplementThreadSync(MaryTree* tree , vector<pair<int, pair<string, int>>>& queries) {
		this->tree = tree;
		this->queries = queries;
	}

	void create_threads() {
		int thread_size = this->queries.size();
		int count = 0;
		//cout << "creating Threads!!!" << endl;
		for (pair<int , pair<string , int>> it : this->queries) {
			int type = it.first;
			string name = it.second.first;
			int uid = it.second.second;
			count++;
			thread t(&ImplementThreadSync::handle_queries , this, type, name, uid , count);
			threads.emplace_back(move(t));

		
		}
		int size = threads.size();
		for (auto& it : threads) {
			//cout << "thread has started" << endl;
			it.join();
			thread_count++;
		}
		wait_for_all_threads();
		
	}

	
	

	void wait_for_all_threads() {
		while (thread_count++ != threads.size());
		//cout << "All threads are started" << endl;
		
	}

	void handle_queries(int type , string name , int uid , int count) {
		
		//while (!wait_variable);

		
		
		if (type == 1)
		{
			cout << "Name : " << name << " Query Type : " << type << " Count : " << count << endl;
			bool val = this->tree->lock(name, uid);
			cout << (val ? "true" : "false") << endl;
		}
		else if (type == 2)
		{
			cout << "Name : " << name << " Query Type : " << type << " Count : " << count << endl;
			bool val = this->tree->unlock(name, uid);
			cout << (val ? "true" : "false") << endl;
		}
		else if (type == 3)
		{
			cout << "Name : " << name << " Query Type : " << type << " Count : " << count << endl;
			bool val = this->tree->upgrade_lock(name, uid);
			cout << (val ? "true" : "false") << endl;
		}
	}

};

int main() {


	int n, m, q;
	cin >> n >> m >> q;
	vector<string> names(n);
	vector<pair<int, pair<string, int>>> queries;
	for (int i = 0; i < n; i++) {
		cin >> names[i];
	}

	MaryTree* tree = new MaryTree(names[0]);

	tree->create_descendants(names, m);

	while (q--) {
		int type, uid;
		string name;
		cin >> type >> name >> uid;

		queries.push_back(make_pair(type, make_pair(name, uid)));
	}
	ImplementThreadSync its(move(tree), queries);

	
	its.create_threads();

	return 0;
}