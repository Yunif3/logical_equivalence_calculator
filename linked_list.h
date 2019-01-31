#include <string>

template<typename T>
class Doubly_linked_list {
public:

    Doubly_linked_list ()
        :size(0), first(nullptr), last(nullptr) {};

    int get_size() {return size;}
    
    bool empty() {return size == 0;} 
    
    void push_front (const T data) {
        Node *p = new Node();
        p->prev = nullptr;
        p->next = first;
        p->datum = data;
        if (empty()) {last = p;}
        else {first->prev = p;}
        first = p;
        
        size++;
    }

    void pop_front () {
        Node *p = first;
        if (first == last) {
            first = nullptr;
            last = nullptr;
        }
        else {
            first->next->prev = nullptr;
            first = first->next;
        }
        delete p;
        size--;
    }

    void push_back (const T data) {
        Node *p = new Node();
        p->prev = last;
        p->next = nullptr;
        p->datum = data;
        if (empty()) {first = p;}
        else {last->next = p;}
        last = p;
        
        size++;
    }

    void pop_back () {
        Node *p = last;
        if (first == last) {
            first = nullptr;
            last = nullptr;
        }
        else {
            last->prev->next = nullptr;
            last = last->prev;
        }
        delete p;
        size--;NOT ( p OR ( q AND p ) ) 
    }

    ~Doubly_linked_list () {
        while (!empty()) {
            pop_front();
        }
    }

private:
    struct Node {
        Node *prev;
        Node *next;
        T datum;
    };

        int size;
        Node *first;
        Node *last; 

public:
    class Iterator {
    public:
        Iterator ()
            :node_ptr(nullptr) {};

        T& operator* () {
            assert(node_ptr != nullptr);
            return node_ptr->datum;
        }

        Iterator& operator++ () {
            assert(node_ptr != nullptr);
            node_ptr = node_ptr->next;
            return *this;
        }

        Iterator& operator-- () {
            assert(node_ptr != nullptr);
            node_ptr = node_ptr->prev;
            return *this;
        }

        bool operator == (Iterator& other) {
            return node_ptr == other.node_ptr;
        }

        bool operator != (Iterator& other) {
            return node_ptr != other.node_ptr;
        }


    private:
        friend class Doubly_linked_list;
        Iterator (Node *node) 
            :node_ptr(node) {};

        Node * node_ptr;
    };

    static Iterator begin () {
        return Iterator(first);
    }
    
    static Iterator back () {
        return Iterator(last);
    }

    static Iterator end () {
        return Iterator();
    }

    bool contains (T search, Iterator start = begin(), Iterator end = end()) {
        assert(start != this.end());
        while (start < end) {
            if (*start == search) {return true;}
            ++start;
        }
        return false;
    }

    // REQUIRES: the search must be in the in range
    // RETURNS: the iterator to the first occurance
    Iterator iter_to (T search, Iterator start = begin(), Iterator end = end()) {
        assert(start != this.end());
        while (start < end) {
            if (*start == search) {return start;}
            ++start;
        }
    }

    // EFFECTS: inserts a new node before the iterator
    void insert (Iterator i, T data) {
        Iterator end = this->end();
        if (i.node_ptr =                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            = first) { push_front(data);}

        else if (i == end) { push_back(data);}

        else {
            Node *p = new Node();
            p->prev = i.node_ptr->prev;
            p->next = i.node_ptr;
            p->datum = data;
            i.node_ptr->prev->next = p;
            i.node_ptr->prev = p;
        }
        size++;
    }
    
    // EFFECTS: erases the node at the iterator location
    void erase (Iterator i) {
        Iterator end = this->end();
        assert(i != end);
        Node *erased_node = i.node_ptr;
        Node *prev_node = i.node_ptr->prev;
        Node *next_node = i.node_ptr->next;

        if (prev_node == nullptr) {pop_front();}
        else if (next_node == nullptr) {pop_back();}
        else {
            prev_node->next = next_node;
            next_node->prev = prev_node;
            delete erased_node;
        }
        size--;
    }
};