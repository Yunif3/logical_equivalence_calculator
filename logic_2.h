#pragma once
#include <string>
#include <vector>

using namespace std;

class Node {
public:
	Node(string v)
		:children(vector<Node*>()), parent(nullptr), val(v) {}

	Node(string v, Node* par)
		:children(vector<Node*>()), parent(par), val(v) {
		par->children.push_back(this);
	}

	Node(const Node* other) {
		val = other->val;
		for (auto n : other->children) {
			Node* copy = new Node(n);
			children.push_back(copy);
			copy->parent = this;
		}
	}

	~Node() {}
	vector<Node*> children;
	Node* parent;
	string val;
};

class Logics {
private:
	void erase_children(Node * n) {
		if (n) {
			for (auto node : n->children) {
				erase_all(node);
			}
			n->children = vector<Node*>();
		}
	}

	void erase_all(Node* n) {
		if (n) {
			erase_children(n);
			delete n;
		}
	}

	// returns the inorder of the root node to string s
	string inorder(Node* root);
	int search(Node* n, string query, int start);

public:
	Node* start;
	Logics()
		:start(nullptr) {}
	Logics(Node* root)
		:start(root) {}
	~Logics() {
		erase_all(start);
	}
	void set_start(Node* root) {
		start = root;
		return;
	}

	int count_nodes();
	void print(Node* root, string &s);
	// F with AND or T with OR
	bool domination(Node* &root);
	// left and right are the same
	bool idempotent(Node* &root);
	// p-OR-F or p-AND-T
	bool identity(Node* &root);
	// NOT-NOT-p
	bool double_negation(Node* &root);
	// LHS == NOT-RHS
	bool complement(Node* &root);
	// negating T or F
	bool bool_negation(Node* &root);
	// NOT-(p-OR-q) to (NOT-p)-AND-(NOT-q) and same for OR
	bool de_morgan(Node* &root);
	// IMPLIES to NOT-p-OR-q
	bool conditional(Node* root);
	// IFF to two IMPLIES
	bool double_conditional(Node* root);
	// p-OR-(q-AND-r)
	bool distributive(Node* &root);
	// p-OR-(p-AND-q)
	bool absorption(Node* &root);
	// swapping two subtrees
	bool commutative(Node* &root1, Node* &root2);
};

