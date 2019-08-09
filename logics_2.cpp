#include "logic_2.h"
#include <unordered_map>
#include <deque>

bool is_operator(Node* n) {
	return n->val == "AND" || n->val == "OR";
}

bool is_AND(Node* n) {
	return n->val == "AND";
}

bool is_OR(Node* n) {
	return n->val == "OR";
}

bool is_NOT(Node* n) {
	return n->val == "NOT";
}

// Returns -1 if the string was not found
int Logics::search(Node* n, string query, int start = 0) {
	vector<Node*>& vec = n->children;
	for (int i = start; i < int(vec.size()); ++i) {
		if (vec[i]->val == query) { return i; }
	}
	return -1;
}

bool isotree(Node* n1, Node* n2) {
	bool one = true;
	deque<Node*> queue1_1, queue1_2, queue2_1, queue2_2;
	unordered_map<string, int> values;
	while (!(queue1_1.empty() && queue1_2.empty() && queue2_1.empty() && queue2_2.empty())) {
		if (one) {
			for (auto ptr : queue1_1) {
				for (auto child : ptr->children) {
					queue1_2.push_back(ptr);
				}
				values[ptr->val]++;
			}
			for (auto ptr : queue2_1) {
				for (auto child : ptr->children) {
					queue2_2.push_back(ptr);
				}
				values[ptr->val]--;
			}
			while (!queue1_1.empty()) { queue1_1.pop_front(); }
			while (!queue2_1.empty()) { queue2_1.pop_front(); }
		}
		else {
			for (auto ptr : queue1_2) {
				for (auto child : ptr->children) {
					queue1_1.push_back(ptr);
				}
				values[ptr->val]++;
			}
			for (auto ptr : queue2_2) {
				for (auto child : ptr->children) {
					queue2_1.push_back(ptr);
				}
				values[ptr->val]--;
			}
			while (!queue1_2.empty()) { queue1_2.pop_front(); }
			while (!queue2_2.empty()) { queue2_2.pop_front(); }
		}
		
		for (auto item : values) {
			if (item.second != 0) {
				return false;
			}
		}
	}
	return true;
}

int Logics::count_nodes() {
	int total = 0;
	count_nodes_helper(start, total);
	return total;
}

void count_nodes_helper(Node* n, int &count) {
	if (!n) { return; }
	count++;
	for (auto node : n->children) {
		count_nodes_helper(node, count);
	}
}

void replace_parent(Node* old, Node* replace) {
	Node * parent = old->parent;
	if (parent == nullptr) {
		replace->parent = nullptr;
	}
	else {
		replace->parent = parent;
		remove_child(parent, old);
		add_parent(replace, parent);
	}
}

void remove_child(Node* parent, Node* child) {
	auto it = find(parent->children.begin(), parent->children.end(), child);
	parent->children.erase(it);
}

void add_parent(Node *n, Node* par) {
	par->children.push_back(n);
	n->parent = par;
}

string Logics::inorder(Node* root) {
	string result = "";
	inorder_helper(root, result);
	return result;
}

void inorder_helper(Node* root, string &s) {
	if (!root) { return; }
	if (is_operator(root)) {
		for (auto node : root->children) {
			inorder_helper(node, s);
			s += root->val;
		}
	}
	else {
		s += root->val;
		for (auto node : root->children) {
			inorder_helper(node, s);
		}
	}
}

//FIXME 
void Logics::print(Node* root, string &s) {
	if (!root) { return; }
	if (is_operator(root)) { s += "("; }

	for (auto node : root->children) {
		s += inorder(node);
		s += root->val;
	}

	if (is_operator(root)) { s += ")"; }
	else { s += " "; }
}

bool Logics::domination(Node* &root) {
	if (search(root, "F") != -1 && is_AND(root)) {
		root->val = "F";
		erase_children(root);
		return true;
	}
	else if (search(root, "T") != -1 && is_OR(root)) {
		root->val = "T";
		erase_children(root);
		return true;
	}
	return false;
}

bool Logics::idempotent(Node* &root) {
	vector<Node*>& vec = root->children;
	int i = 0;
	bool success = false;
	while (i < vec.size() - 1) {
		string query = inorder(vec[i]);
		int old_size = vec.size();
		int j = i + 1;
		while (j < vec.size() - 1) {
			int loc = search(root, query, j);
			if (loc != -1 && is_operator(root)) {
				erase_all(vec[loc]);
				vec.erase(vec.begin() + loc);
				success = true;
			}
			else { j++; }
		}
		if (old_size == vec.size()) { ++i; }
	}
	return success;
}

bool Logics::identity(Node* &root) {
	vector<Node*>& vec = root->children;
	bool success = false;

	while (is_OR(root) && search(root, "F") != -1) {
		int loc = search(root, "F");
		erase_all(vec[loc]);
		vec.erase(vec.begin() + loc);
		success = true;
	}
	while (is_AND(root) && search(root, "T") != -1) {
		int loc = search(root, "T");
		erase_all(vec[loc]);
		vec.erase(vec.begin() + loc);
		success = true;
	}
	return success;
}

bool Logics::double_negation(Node* &root) {
	if (is_NOT(root) && search(root, "NOT") != -1) {
		Node* child = root->children[0];
		replace_parent(root, child->children[0]); // Because NOT only has one child
		delete child;
		return true;
	}
	return false;
}

bool Logics::complement(Node* &root) {
	vector<Node*>& vec = root->children;
	if (is_AND(root)) {
		int i = 0;
		while (i < vec.size()) {
			int old_size = vec.size();
			int j = 0;
			while (j < vec.size()) {
				j = search(root, "NOT", j);
				if (j != -1 && isotree(vec[i], vec[j])) {
					root->val = "F";
					erase_children(root);
					return true;
				}
				else { ++j; }
			}
			++i;
		}
	}
	else if (is_OR(root)) {
		int i = 0;
		while (i < vec.size()) {
			int old_size = vec.size();
			int j = 0;
			while (j < vec.size()) {
				j = search(root, "NOT", j);
				if (j != -1 && isotree(vec[i], vec[j])) {
					root->val = "T";
					erase_children(root);
					return true;
				}
				else { ++j; }
			}
			++i;
		}
	}

	return false;
}

bool Logics::bool_negation(Node* &root) {
	if (is_NOT(root) && search(root, "F") != -1) {
		root->val = "T";
		erase_children(root);
		return true;
	}
	else if (is_NOT(root) && search(root, "T") != -1) {
		root->val = "F";
		erase_children(root);
		return true;
	}
	return false;
}

bool Logics::de_morgan(Node* &root) {
	if (is_NOT(root) && is_operator(root->children[0])) {
		Node* child = root->children[0]; // Since there is only one child to NOT

		if (is_AND(child)) { child->val = "OR"; }
		else { child->val = "AND"; }

		for (auto n : child->children) {
			Node * not = new Node("NOT", child);
			replace_parent(n, not);
			not->children.push_back(n);
			delete n;
		}
		replace_parent(root, child);
		delete root;
		return true;
	}
	return false;
}
/* RESUME HERE */
bool Logics::conditional(Node* root) {
	if (root->val == "IMPLIES") {
		root->val = "OR";
		Node* left_not = new Node("NOT", root);
		left_not->children.push_back(root->children[0]);
		replace_parent(root->children[0], left_not);
		return true;
	}
	return false;
}

bool Logics::double_conditional(Node* root) {
	if (root->val == "IFF") {
		vector<Node*>& vec = root->children;
		root->val = "AND";

		Node* implies = new Node("IMPLIES");
		add_parent(vec[0], implies);
		add_parent(vec[1], implies);
		replace_parent(vec[0], implies);

		Node* implies_copy = new Node(implies); //TODO write copy constructor
		replace_parent(vec[1], implies_copy);
		return true;
	}
	return false;
}

bool Logics::distributive(Node* &root) {
	vector<Node*>& vec = root->children;
	int or_index = search(root, "OR");
	int and_index = search(root, "OR"); 
	int other_index = -1;
	for (int i = 0; i < vec.size(); ++i) {
		if (!is_operator(vec[i])) { 
			other_index = i; 
			break;
		}
	}
	
	if (or_index != -1 && other_index != -1 && root->val == "AND") {
		Node* or_node = vec[or_index];
		Node* other_node = vec[other_index];
		for (auto n : or_node->children) {
			Node* and = new Node("AND");
			replace_parent(n, and);
			add_parent(n, and);
			Node* copy = new Node(other_node);
			add_parent(copy, and);
		}
		remove_child(root, or_node);
		erase_all(or_node);
		return true;
	}
	else if (and_index != -1 && other_index != -1 && root->val == "OR") {
		Node* and_node = vec[and_index];
		Node* other_node = vec[other_index];
		for (auto n : and_node->children) {
			Node* or = new Node("OR");
			replace_parent(n, or );
			add_parent(n, or );
			Node* copy = new Node(other_node);
			add_parent(copy, or );
		}
		remove_child(root, and_node);
		erase_all(and_node);
		return true;
	}
	return false;
}

bool Logics::absorption(Node* &root) {
	vector<Node*>& vec = root->children;
	int or_index = search(root, "OR");
	int and_index = search(root, "OR");
	int other_index = -1;
	for (int i = 0; i < vec.size(); ++i) {
		if (!is_operator(vec[i])) {
			other_index = i;
			break;
		}
	}

	if (or_index != -1 && root->val == "AND") {
		Node* or_node = vec[or_index];
		Node* other_node = vec[other_index];
		for (auto root_child : root->children) {
			for (auto or_node_child : or_node->children) {
				if (isotree(or_node_child, root_child)) {
					remove_child(root, or_node);
					erase_all(or_node);
					return true;
				}
			}
		}
	}
	else if (and_index != -1 && root->val == "OR") {
		Node* and_node = vec[and_index];
		Node* other_node = vec[other_index];
		for (auto root_child : root->children) {
			for (auto and_node_child : and_node->children) {
				if (isotree(and_node_child, root_child)) {
					remove_child(root, and_node);
					erase_all(and_node);
					return true;
				}
			}
		}
	}
	return false;
}

bool commutative(Node* &root1, Node* &root2) {
	swap(root1, root2);
	return true;
}