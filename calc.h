#pragma once
#include "logic.h"
#include <stack>
#include <vector>
#include <iostream>
#include <sstream>

class Calculator {
private:
	Node* make_subtree(Node* n, vector<string>& vec);
	Logics lhs, rhs;
public:
	Node* read_input(string& s);
	void solve();
	void run_double_conditional(Node* n, Logics& l);
	void run_conditional(Node* n, Logics& l);
	void run_laws(Node* n, Logics& l, bool repeat);
	void print_side(Logics &l);
	void print_calc();
};

Node* Calculator::read_input(string& s) {
	istringstream input(s);
	stack<string> expression;
	Node* root = nullptr;

	string val = "";
	while (input >> val) {
		if (val == ")") {
			vector<string> reverse;
			while (expression.top() != "(") {
				reverse.push_back(expression.top());
				expression.pop();
			}
			expression.pop();

			Node* current = make_subtree(root, reverse);
			if (!current) {
				continue;
			}
			if (!root) {
				root = current;
			}
			else if (!root->left) {
				root->left = current;
				current->parent = root;
			}
			else if (!root->right) {
				root->right = current;
				current->parent = root;
			}
		}
		else if (val == "(") {
			if (expression.empty() || expression.top() == "(") {
				expression.push(val);
				continue;
			}
			vector<string> reverse;
			reverse.push_back(expression.top());
			string recur_str = "(";
			bool neg = false;
			if (expression.top() == "NOT") {
				neg = true;
			}
			
			int paren_count = 1;
			while (input >> val) {
				recur_str += " " + val;
				if (val == "(") { paren_count++; }
				else if (val == ")") { paren_count--; }
				if (paren_count == 0) { break; }
			}
			if (paren_count == 0) {
				Node* current = make_subtree(root, reverse);
				current->right = read_input(recur_str);
				current->right->parent = current;
				if (!root) {
					root = current;
				}
				else if (neg) {
					root->right = current;
					current->parent = root;
				}
				else {
					current->left = root;
					root->parent = current;
					root = current;
				}
				// gets rid of the BLANK
				expression.pop();
			}
		}
		else {
			expression.push(val);
		}
	}
	return root;
}

Node* Calculator::make_subtree(Node* n, vector<string>& vec) {
	Node* root;
	if (vec.size() == 0) {
		root = nullptr;
	}
	else if (vec.size() == 1) {
		root = new Node(vec[0]);
	}
	// when it's NOT-p
	else if (vec.size() == 2) {
		root = new Node(vec[1]);
		root->right = new Node(vec[0], root);
	}
	else {
		root = new Node(vec[1]);
		root->left = new Node(vec[0], root);
		root->right = new Node(vec[2], root);
	}
	
	return root;
}

void Calculator::solve() {
	string lhs_str = "";
	string rhs_str = "";

	string word = "";
	while (cin >> word) {
		if (word == "EQUALS") {
			break;
		}
		lhs_str += word + " ";
	}
	while (cin >> word) {
		rhs_str += word + " ";
	}

	Node* lhs_root = read_input(lhs_str);
	Node* rhs_root = read_input(rhs_str);

	lhs.set_start(lhs_root);
	rhs.set_start(rhs_root);
	print_calc();

	run_double_conditional(lhs.start, lhs);
	run_double_conditional(rhs.start, rhs);
	print_calc();

	run_conditional(lhs.start, lhs);
	run_conditional(rhs.start, rhs);
	print_calc();

	run_laws(lhs.start, lhs, false);
	print_calc();

	run_laws(rhs.start, rhs, false);
	print_calc();
}

void Calculator::run_double_conditional(Node* n, Logics& l) {
	if (!n) { return; }
	run_double_conditional(n->left, l);
	run_double_conditional(n->right, l);
	l.double_conditional(n);
}

void Calculator::run_conditional(Node* n, Logics& l) {
	if (!n) { return; }
	run_conditional(n->left, l);
	run_conditional(n->right, l);
	l.conditional(n);
}

// todo fix, might run forever
void Calculator::run_laws(Node* n, Logics& l, bool repeat) {
	if (!n) {
		return;
	}

	run_laws(n->left, l, false);
	run_laws(n->right, l, false);

	while (l.double_negation(n)) { 
		print_side(l);
		cout << "    (double negation law)" << endl;
	}
	while (l.identity(n)) {
		print_side(l);
		cout << "    (identity law)" << endl;
	}
	while (l.domination(n)) {
		print_side(l);
		cout << "    (domination law)" << endl;
	}
	while (l.idempotent(n)) {
		print_side(l);
		cout << "    (idempotent law)" << endl;
	}
	// Todo remove later, debugging purposes
	/*string out;
	l.print(n, out);
	cout << out << endl;*/
	while (l.absorption(n)) {
		print_side(l);
		cout << "    (absorption law)" << endl;
	}
	while (l.bool_negation(n)) {
		print_side(l);
		cout << "    (boolean negation law)" << endl;
	}
	while (l.complement(n)) {
		print_side(l);
		cout << "    (complement law)" << endl;
	}

	while (l.de_morgan(n)) {
		print_side(l);
		cout << "    (De Morgan's law)" << endl;
		run_laws(n->left, l, false);
		run_laws(n->right, l, false);
	}

	if (!repeat && l.distributive(n)) {
		print_side(l);
		cout << "    (distributive law)" << endl;
		run_laws(n->left, l, true);
		run_laws(n->right, l, true);
	}
	/*while (l.distributive(n)) {
		print_side(l);
		cout << "    (distributive law)" << endl;
		run_laws(n->left, l);
		run_laws(n->right, l);
	}*/
	
	return;
}

void Calculator::print_side(Logics &l) {
	string out;
	l.print(l.start, out);
	cout << out;
} 

void Calculator::print_calc() {
	cout << "LHS" << endl;
	print_side(lhs);
	cout << endl;
	cout << "RHS" << endl;
	print_side(rhs);
	cout << endl;
}