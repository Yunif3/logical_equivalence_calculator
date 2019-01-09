#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "linked_list.h"
#include <string>
#include <vector>
#include <iostream>
#include <cctype>
#include <cassert>

typedef Doubly_linked_list<std::string>::Iterator iter;

class Calculator {
    public:
        void simplify () {
            while(propositions.contains("IMPLIES")) {
                iter implies_it = propositions.iter_to("IMPLIES");
                iter op_it = find_closest_open_paren(implies_it);
                if (op_it == end) { op_it = propositions.begin();}
                implies(op_it);
            }
            
            return;
        }

        void take_input () {
            std::string input;
            while (std::cin >> input && input != "calculate") {
                propositions.push_back(input);
            }
        }

        void print(std::ostream &os) {
            os << "== ";
            
            for (auto p : propositions) {
                if (p == "NOT") {
                    os << p;
                }
                else if (isupper(*p.c_str())) {
                    os << " " << p << " ";
                }
                else {
                    os << p;
                }
            }
            os << std::endl;
        }
        void test_distribution () {
            iter begin_it, end_it;
            begin_it = propositions.begin();
            end_it = propositions.back();
            negation_distribute(begin_it, end_it);
        }

        // REQUIRES: there is an '(' after the NOT and ')' at the end 
        // begin it is the iterator to the "NOT"
        void negation_distribute (iter not_it, iter end_it) {
            assert(check_invariants(end_it));
            
            iter begin_it = not_it;
            ++begin_it;
            propositions.erase(not_it);
            iter open_parenthesis_it = begin_it;
            iter close_parenthesis_it = end_it;

            for (iter i = begin_it; i != end_it; ++i) {
                if (*i == "(" && i != begin_it) {
                    open_parenthesis_it = i;
                    break;
                }
            }
            for (iter i = end_it; i != begin_it; --i) {
                if (*i == ")" && i != end_it) {
                    close_parenthesis_it = i;
                    break;
                }
            }

            if (open_parenthesis_it == begin_it && close_parenthesis_it == end_it) {
                for (iter i = ++begin_it; i != end_it; ++i) {
                    negate(i);
                }
                
                return;
            }
            else {
                for (iter i = ++begin_it; i != open_parenthesis_it; ++i) {
                    negate(i);
                }
                for (iter i = --end_it; i != close_parenthesis_it; --i) {
                    negate(i);
                }
                propositions.insert(open_parenthesis_it, "NOT");
                print(std::cout);
                negation_distribute(--open_parenthesis_it, close_parenthesis_it);
            }
            print(std::cout);
        }

        // REQUIRES: the range contains an IMPLIES 
        //           start_it is an iterator to the '('
        void implies (iter start_it) {
            assert(check_invariants(start_it));
            iter implies_it = start_it;
            while (*implies_it != "IMPLIES") { ++implies_it;}
            for (iter i = start_it; i != implies_it; ++i) { negate(i);}
            *implies_it = "OR";
        }
        
        /*
        // REQUIRES: the range contains an IFF
        //           start_it is an iterator to the '('
        void iff (iter start_it, iter end_it) {
            assert(check_invariants(start_it));
            iter iff_it = start_it;
            while (*iff_it != "IFF") {++iff_it;}
            
        }
        */
    private:
        Doubly_linked_list<std::string> propositions;  
        iter end = propositions.end();

        // REQUIRES: i is an iterator that points to a non-nullptr
        void negate (iter i) {
            assert(check_invariants(i));
            if (*i == "AND" || *i == "OR") {
                reverse_and_or(i);
            }
            else if (*i == "(" || *i == ")") {
                return;
            }
            else {
                propositions.insert(i, "NOT");
            }
        }

        void reverse_and_or (iter i) {
            if (*i == "AND") {
                *i = "OR";
            }
            else {
                *i = "AND";
            }
        }

        // checks that the iterator isn't pointing to nullptr
        bool check_invariants(iter check) {
            Doubly_linked_list<std::string>::Iterator end = propositions.end();
            return check != end;
        }

        // Finds the closest open parenthesis to the left of the iterator
        // Returns end iterator if it's not found.
        iter find_closest_open_paren (iter start) {
            iter begin = propositions.begin();
            for (iter i = start; i != begin; --i) {
                if (*i == "(") {
                    return i;
                }
            }
            if (*begin == "(") { return begin;}
            return end;
        }

        // Finds the closest close parenthesis to the left of the iterator
        // Returns end iterator if it's not found.
        iter find_closest_close_paren (iter start) {
            for (iter i = start; i != end; ++i) {
                if (*i == ")") {
                    return i;
                }
            }
            return end;
        }

        /*
        void distribute (int dist_start, int dist_size, int receive_start, int recieve_size) {
            for (int i = receive_start; i < recieve_size + 1; ++i) {
                if (propositions[i] == "(") {
                    int temp = i;
                    while (propositions[temp] == ")") {
                        ++temp;
                    }
                    distribute(i, temp);
                }
                else 
            }
        }
        */
};

std::ostream & operator << (std::ostream &os, Calculator calc) {
    calc.print(os);
    return os;
}



#endif