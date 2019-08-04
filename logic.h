#pragma once
#include <string>

using namespace std;

//enum Oper {
//	not, and, or, implies
//};
class Node {
public:
	Node(string v)
		:left(nullptr), right(nullptr), parent(nullptr), val(v) {}

	Node(string v, Node* par)
		:left(nullptr), right(nullptr), parent(par), val(v) {}

	Node(Node* old) {
		if (old) {
			this->val = old->val;
			if (old->left) {
				this->left = new Node(old->left);
				this->left->parent = this;
			}
			else { this->left = nullptr; }
			if (old->right) {
				this->right = new Node(old->right);
				this->right->parent = this;
			}
			else { this->right = nullptr; }
		}
	}

	Node* left;
	Node* right;
	Node* parent;
	string val;
};

class Logics {
private:
	void erase(Node* n) {
		if (n) {
			erase(n->left);
			erase(n->right);
			delete n;
		}
	}
	// returns the inorder of the root node to string s
	void inorder(Node* root, string &s);

public:
	Node* start;
	Logics()
		:start(nullptr) {}
	Logics(Node* root)
		:start(root) {}
	~Logics() {
		erase(start);
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

int Logics::count_nodes() {
	int total = 0;
	count_nodes_helper(start, total);
	return total;
}

void count_nodes_helper(Node* n, int &count) {
	if (!n) { return; }
	else {
		count++;
		count_nodes_helper(n->left, count);
		count_nodes_helper(n->right, count);
	}
	return;
}

//TODO don't forget to set the left and right as null pointers
void Logics::inorder(Node* root, string &s) {
	if (!root) { return; }
	inorder(root->left, s);
	s += root->val;
	inorder(root->right, s);
}

void Logics::print(Node* root, string &s) {
	if (!root) { return; }
	// todo this is new
	if (root->left || root->right) {
		s += "(";
	}
	else {
		s += " ";
	}
	print(root->left, s);
	s += root->val;
	print(root->right, s);
	// todo this is new
	if (root->left || root->right) {
		s += ")";
	}
	else {
		s += " ";
	}
}

bool Logics::domination(Node* &root) {
	string l, r;
	inorder(root->left, l);
	inorder(root->right, r);
	if ((l == "F" || r == "F") && root->val == "AND") {
		root->val = "F";
		root->left = nullptr;
		root->right = nullptr;
		//delete
		return true;
	}
	else if ((l == "T" || r == "T") && root->val == "OR") {
		root->val = "T";
		root->left = nullptr;
		root->right = nullptr;
		//delete
		return true;
	}
	return false;
}

bool Logics::idempotent(Node* &root) {
	string l, r;
	inorder(root->left, l);
	inorder(root->right, r);
	if (l == r && (root->val == "AND" || root->val == "OR")) {
		if (root->parent == nullptr) {
			Node* replace = root->left;
			replace->parent = nullptr;
			//delete 
			root = replace;
		}
		else if (root == root->parent->left) {
			Node* replace = root->left;
			root->parent->left = replace;
			replace->parent = root->parent;
			//delete
			root = replace;
		}
		else if (root == root->parent->right) {
			Node* replace = root->left;
			root->parent->right = replace;
			replace->parent = root->parent;
			//delete 
			root = replace;
		}
		return true;
	}
	return false;
}

bool Logics::identity(Node* &root) {
	string l, r;
	inorder(root->left, l);
	inorder(root->right, r);
	if ((l == "F" && root->val == "OR") || (l == "T" && root->val == "AND")) {
		if (root->parent == nullptr) {
			Node* replace = root->right;
			replace->parent = nullptr;
			//delete root & left
			root = replace;
		}
		else if (root == root->parent->left) {
			Node* replace = root->right;
			replace->parent = root->parent;
			root->parent->left = replace;
			//delete root & left
			root = replace;
		}
		else if (root == root->parent->right) {
			Node* replace = root->right;
			replace->parent = root->parent;
			root->parent->right = replace;
			//delete root & left
			root = replace;
		}
		return true;
	}
	else if ((r == "F" && root->val == "OR") || (r == "T" && root->val == "AND")) {
		if (root->parent == nullptr) {
			Node* replace = root->left;
			replace->parent = nullptr;
			//delete root & right
			root = replace;
		}
		else if (root == root->parent->left) {
			Node* replace = root->left;
			replace->parent = root->parent;
			root->parent->left = replace;
			//delete root & right
			root = replace;
		}
		else if (root == root->parent->right) {
			Node* replace = root->left;
			replace->parent = root->parent;
			root->parent->right = replace;
			//delete root & right
			root = replace;
		}
		return true;
	}
	return false;
}

bool Logics::double_negation(Node* &root) {
	if (root->val == "NOT" && root->right && root->right->val == "NOT") {
		if (root->parent == nullptr) {
			Node* replace = root->right->right;
			replace->parent = nullptr;
			//delete root & right
			root = replace;
		}
		else if (root == root->parent->left) {
			Node* replace = root->right->right;
			replace->parent = root->parent;
			root->parent->left = replace;
			//delete root & right
			root = replace;
		}
		else if (root == root->parent->right) {
			Node* replace = root->right->right;
			replace->parent = root->parent;
			root->parent->right = replace;
			//delete root & right
			root = replace;
		}
		return true;
	}
	return false;
}

bool Logics::complement(Node* &root) {
	string l, r;
	inorder(root->left, l);
	inorder(root->right, r);
	if ("NOT" + l == r || l == "NOT" + r) {
		if (root->val == "AND") {
			root->val = "F";
			// delete left & right
			root->left = nullptr;
			root->right = nullptr;
		}
		else if (root->val == "OR") {
			root->val = "T";
			// delete left & right
			root->left = nullptr;
			root->right = nullptr;
		}
		return true;
	}
	return false;
}

bool Logics::bool_negation(Node* &root) {
	if (root->val == "NOT") {
		if (root->right && root->right->val == "T") {
			root->val = "F";
			//delete right
			root->right = nullptr;
			return true;
		}
		else if (root->right && root->right->val == "F") {
			root->val = "T";
			//delete right
			root->right = nullptr;
			return true;
		}
	}
	return false;
}

bool Logics::de_morgan(Node* &root) {
	if (root->val == "NOT" && root->right && (root->right->val == "AND" || root->right->val == "OR")) {
		if (root->right->val == "AND") { root->right->val = "OR"; }
		else { root->right->val = "AND"; }

		Node* left_not = new Node("NOT");
		left_not->right = root->right->left;
		left_not->parent = root->right;

		Node* right_not = new Node("NOT");
		right_not->right = root->right->right;
		right_not->parent = root->right;

		root->right->left = left_not;
		root->right->right = right_not;
	
		if (root->parent == nullptr) {
			Node* replace = root->right;
			replace->parent = nullptr;
			//delete root 
			root = replace;
		}
		else if (root == root->parent->left) {
			Node* replace = root->right;
			replace->parent = root->parent;
			root->parent->left = replace;
			//delete root 
			root = replace;
		}
		else if (root == root->parent->right) {
			Node* replace = root->right;
			replace->parent = root->parent;
			root->parent->right = replace;
			//delete root 
			root = replace;
		}
		return true;
	}
	return false;
}

bool Logics::conditional(Node* root) {
	if (root->val == "IMPLIES") {
		root->val = "OR";
		Node* left_not = new Node("NOT");
		left_not->parent = root;
		left_not->right = root->left;
		root->left->parent = left_not;
		root->left = left_not;
		return true;
	}
	return false;
}

bool Logics::double_conditional(Node* root) {
	if (root->val == "IFF") {
		Node* left_copy = new Node(root->left);
		Node* right_copy = new Node(root->right);
		Node* and_sign = new Node("AND");
		Node* left_implies = new Node("IMPLIES");
		Node* right_implies = new Node("IMPLIES");
		and_sign->parent = root->parent;
		and_sign->left = left_implies;
		and_sign->right = right_implies;

		left_implies->parent = and_sign;
		left_implies->left = root->left;
		left_implies->right = root->right;
		root->left->parent = left_implies;
		root->right->parent = left_implies;

		right_implies->parent = and_sign;
		right_implies->left = right_copy;
		right_implies->right = left_copy;
		right_copy->parent = right_implies;
		left_copy->parent = right_implies;

		if (root->parent == nullptr) {
			start = and_sign;
			//delete root 
		}
		else if (root == root->parent->left) {
			root->parent->left = and_sign;
			//delete root 
		}
		else if (root == root->parent->right) {
			root->parent->right = and_sign;
			//delete root
		}
		return true;
	}
	return false;
}

bool Logics::distributive(Node* &root) {
	if ((root->val == "AND" && root->left && root->left->val == "OR") ||
		(root->val == "OR" && root->left && root->left->val == "AND")) {

		Node* temp = root->left->right;
		Node* right_copy = new Node(root->right);
		Node* oper;
		if (root->val == "AND") { oper = new Node("AND"); }
		else { oper = new Node("OR"); }
		
		// LHS
		if (root->val == "AND") { root->val = "OR"; }
		else { root->val = "AND"; }
		if (root->left->val == "AND") { root->left->val = "OR"; }
		else { root->left->val = "AND"; }
		root->left->right = right_copy;
		right_copy->parent = root->left;

		// RHS
		oper->right = root->right;
		root->right->parent = oper;
		oper->left = temp;
		temp->parent = oper;
		root->right = oper;
		oper->parent = root;
		return true;
	}
	else if ((root->val == "AND" && root->right && root->right->val == "OR") ||
		(root->val == "OR" && root->right && root->right->val == "AND")) {

		Node* temp = root->right->left;
		Node* left_copy = new Node(root->left);
		Node* oper;
		if (root->val == "AND") { oper = new Node("AND"); }
		else { oper = new Node("OR"); }

		// LHS
		if (root->val == "AND") { root->val = "OR"; }
		else { root->val = "AND"; }
		if (root->right->val == "AND") { root->right->val = "OR"; }
		else { root->right->val = "AND"; }
		root->right->left = left_copy;
		left_copy->parent = root->right;

		// RHS
		oper->left = root->left;
		root->left->parent = oper;
		oper->right = temp;
		temp->parent = oper;
		root->left = oper;
		oper->parent = root;
		return true;
	}
	return false;
}

bool Logics::absorption(Node* &root) {
	if ((root->val == "AND" && root->left && root->left->val == "OR")
		|| (root->val == "OR" && root->left && root->left->val == "AND")) {
		string right, left_left, left_right;
		print(root->right, right);
		print(root->left->left, left_left);
		print(root->left->right, left_right);
		if (right == left_left || right == left_right) {
			if (root->parent == nullptr) {
				Node* replace = root->right;
				replace->parent = nullptr;
				//delete root 
				root = replace;
			}
			else if (root == root->parent->left) {
				Node* replace = root->right;
				replace->parent = root->parent;
				root->parent->left = replace;
				//delete root 
				root = replace;
			}
			else if (root == root->parent->right) {
				Node* replace = root->right;
				replace->parent = root->parent;
				root->parent->right = replace;
				//delete root 
				root = replace;
			}
			//delete all the previous nodes
			return true;
		}
	}
	else if ((root->val == "AND" && root->right && root->right->val == "OR")
		|| (root->val == "OR" && root->right && root->right->val == "AND")) {
		string left, right_left, right_right;
		print(root->left, left);
		print(root->right->left, right_left);
		print(root->right->right, right_right);
		if (left == right_left || left == right_right) {
			if (root->parent == nullptr) {
				Node* replace = root->left;
				replace->parent = nullptr;
				//delete root 
				root = replace;
			}
			else if (root == root->parent->left) {
				Node* replace = root->left;
				replace->parent = root->parent;
				root->parent->left = replace;
				//delete root 
				root = replace;
			}
			else if (root == root->parent->right) {
				Node* replace = root->left;
				replace->parent = root->parent;
				root->parent->right = replace;
				//delete root 
				root = replace;
			}
			//delete all the previous nodes
			return true;
		}
	}
	return false;
}

bool commutative(Node* &root1, Node* &root2) {
	if ((root1->parent->val == "and" && root2->parent->val == "and") &&
		(root1->parent->val == "or" && root2->parent->val == "or")) {
		Node r1 = *root1;
		root1->parent = root2->parent;
		if (root2 == root2->parent->left) {
			root2->parent->left = root1;
		}
		else if (root2 == root2->parent->right) {
			root2->parent->right = root1;
		}

		root2->parent = r1.parent;
		if (root1 == r1.parent->left) {
			r1.parent->left = root2;
		}
		else if (root1 == r1.parent->right) {
			r1.parent->right = root2;
		}

		swap(root1, root2);
		return true;
		}
	return false;
}