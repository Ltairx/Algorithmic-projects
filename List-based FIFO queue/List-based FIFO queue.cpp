#include <iostream>
#include <string>

using namespace std;

struct Node {
    int data;
    Node* ptr;
};

struct Pointers {
    Node* beg = nullptr, *end = nullptr, *front=nullptr, *back=nullptr, *preFront = nullptr; //preFront to next
    int qSize = 0, lSize = 0;
};

Node* ptr_xor(Node* a, Node* b) {
    return (Node*)((uintptr_t)(a) ^ (uintptr_t)(b));
}

Node* second_node(Node* other, Node* curr) {
    return ptr_xor(other, curr->ptr);
}

#pragma region add

Node* add(int value, Node** a, Node* b) {

    Node* node = new Node();
    node->data = value;
    node->ptr = ptr_xor(*a, b);
    (*a)->ptr = ptr_xor(node, ptr_xor((*a)->ptr, b));
    (*a) = node;

    return node;
}

void add_first(int value, Pointers& points) {
    Node* node = new Node();
    node->data = value;
    node->ptr = NULL;
    points.beg = node;
    points.end = node;

}

void add_beg(int value, Pointers& points) {

    if (points.beg == nullptr) {
        add_first(value, points);
        return;
    }

    add(value, &points.beg, NULL);

    if (points.beg->ptr == points.front) {
        points.preFront = points.beg;
    }
}

void add_end(int value, Pointers& points) {

    if (points.end == nullptr) {
        add_first(value, points);
        return;
    }

    add(value, &points.end, NULL);

    if (points.beg == points.front) {
        points.preFront = points.end;
    }
}

#pragma endregion

#pragma region print

void print_forward(Pointers points) {

    if (points.beg == NULL)
        cout << "NULL" << endl;
    else {
        Node* curr = points.beg, * next, * prev = NULL;

        while (curr != NULL) {
            cout << curr->data << " ";
            next = ptr_xor(prev, curr->ptr);
            prev = curr;
            curr = next;
        }
        cout << endl;
    }
}

void print_backward(Pointers points) {

    if (points.end == NULL)
        cout << "NULL" << endl;
    else {
        Node* curr = points.end, * next = NULL, * prev;

        while (curr != NULL) {

            cout << curr->data << " ";
            prev = ptr_xor(curr->ptr, next);
            next = curr;
            curr = prev;
        }
        cout << endl;
    }
}

void print_queue(Pointers list) {

    if (list.qSize == 0)
        cout << "NULL" << endl;
    else {

        Node* curr = list.front, * prev = list.preFront, *next = second_node(prev, curr);

        for (int i = 0; i < list.qSize; i++) {
            cout << curr->data << " ";

            if (curr == list.beg) {
                curr = list.end;
                next = NULL;
                prev = list.end->ptr;
            }
            else {
                prev = ptr_xor(curr->ptr, next);
                next = curr;
                curr = prev;
            }
        }
        cout << endl;
    }

}

#pragma endregion

#pragma region delete

void del_last(Pointers& points) {

    delete points.beg;
    points.front = nullptr;
    points.preFront = nullptr;
    points.back = nullptr;
    points.beg = nullptr;
    points.end = nullptr;

}

void del(Node** changed) {

    Node* node = (*changed);
    (*changed) = (*changed)->ptr;
    (*changed)->ptr = ptr_xor(node, (*changed)->ptr);

    delete node;
}

#pragma endregion

bool is_list(Pointers list, Node* node) {

    if (node == list.front || node == list.back || node == list.preFront)
        return 1;

    Node* curr = list.front, * prev = list.preFront, * next = second_node(prev, curr);

    for (int i = 0; i < list.qSize; i++) {
        if (curr == node)
            return 1;

        if (curr == list.beg) {
            curr = list.end;
            next = NULL;
            prev = list.end->ptr;
        }
        else {
            prev = second_node(next, curr);
            next = curr;
            curr = prev;
        }
    }

    return 0;
}

void push(Pointers& list, int value) {

    if (list.qSize == 0) {
        list.end->data = value;
        list.front = list.end;
        list.back = list.end;
        return;
    }

    Node* curr = list.beg, * next, * prev = NULL;

    while (curr != NULL) {

        if (curr == list.back) {

            if (prev != NULL) {
                (*prev).data = value;
                list.back = prev;
                if (list.preFront == NULL) {
                    list.preFront = list.back;
                }
               
            }
            else {
                list.end->data = value;
                list.back = list.end;
                if (list.preFront == NULL) {
                    list.preFront = list.back;
                }
            }
            break;
        }

        next = second_node(prev, curr);
        prev = curr;
        curr = next;
    }

}

void add_and_push(Pointers& list, int value) {

    Node* node = new Node();
    node->data = value;
    node->ptr = ptr_xor(list.front, list.back);

    if (list.front == list.beg) {
        list.front->ptr = node;
    }
    else
        list.front->ptr = ptr_xor(list.preFront, node);

    if (list.back == list.end) {
        list.back->ptr = node;
    }
    else {
        list.back->ptr = ptr_xor(node, second_node(list.front, list.back));
    }

    list.back = node;

}

void pop(Pointers& list) {

    if (list.qSize == 0) {
        cout << "NULL" << endl;
        return;
    }

    cout << list.front->data << endl;


    if (list.qSize > 1) {

        if(list.front == list.beg) {
            list.front = list.end;
            list.preFront = list.front->ptr;
        }
        else {
            Node* prev = second_node(list.front, list.preFront);
            list.front = list.preFront;
            if (prev == NULL) {

                Node* prevv, * next = NULL;
                prev = list.end;
                while (!is_list(list, prev) && prev != NULL) {
                    prevv = second_node(next, prev);
                    next = prev;
                    prev = prevv;
                }
            }

            list.preFront = prev;
        }
    }
    else {
        list.front = nullptr;
    }

    list.qSize--;
}

void soft(Node** node) {
    (*node)->data = 0;
}

void hard(Node* node, Node** prev, Pointers& list) {

    list.lSize--;

    if (list.beg == list.end) {
        del_last(list);
    }
    else if (list.beg == node) {
        del(&list.beg);
    }
    else if (list.end == node) {
        del(&list.end);
    }
    else {

        Node* next = second_node((*prev), node);

        if (node == list.beg->ptr) {
            (*prev)->ptr = next;
            next->ptr = ptr_xor((*prev), second_node(node, next));
        }
        else if(node == list.end->ptr){
            next->ptr = (*prev);
            (*prev)->ptr = ptr_xor(next, second_node(node, (*prev)));
        }
        else {
            next->ptr = ptr_xor((*prev), second_node(node, next));
            (*prev)->ptr = ptr_xor(next, second_node(node, (*prev)));
        }

        Node* toDelete = node;
        delete toDelete;
    }

}

void garbage(Pointers& list, bool isSoft) {

    if (list.beg == NULL)
        return;
    else {
        Node* curr = list.beg, * next, * prev = NULL;

        while (curr != NULL) {

            next = ptr_xor(prev, curr->ptr);

            if (list.qSize < 1 || !is_list(list, curr)) {
                if (isSoft) {
                    soft(&curr);
                    prev = curr;
                }
                else {
                    hard(curr, &prev, list);
                }

            }
            else           
                prev = curr;

            curr = next;
        }
        
    }

}

void input() {

    Pointers list;

    string command;

    while (cin >> command) {

        if (command == "ADD_BEG") {
            int n;
            cin >> n;

            if (list.qSize > 1 && is_list(list, list.beg) && is_list(list, list.end))
                list.qSize++;

            if (list.qSize > 1 && list.back == list.beg && list.front == list.end)
                list.qSize--;

            add_beg(n, list);

            list.lSize++;
        }
        else if (command == "ADD_END") {
            int n;
            cin >> n;

            if (list.qSize > 1 && is_list(list, list.beg) && is_list(list, list.end))
                list.qSize++;

            if (list.qSize > 1 && list.back == list.beg && list.front == list.end)
                list.qSize--;

            add_end(n, list);
            list.lSize++;
        }
        else if (command == "PUSH") {
            int n;
            cin >> n;

            if (list.lSize == 0) {
                add_beg(n, list);
                list.lSize++;
                list.front = list.beg;
                list.back = list.end;

            }
            else if (list.lSize == list.qSize) {

                if (list.beg == list.back && list.end == list.front) {

                    add_beg(n, list);
                    list.lSize++;
                    list.back = list.beg;

                    if (list.preFront == NULL) { 
                        list.preFront = list.back;
                    }

                }
                else {
                    add_and_push(list, n);
                    list.lSize++;
                }
                
            }
            else {
                push(list, n);                
            }
            list.qSize++;
        }
        else if (command == "PRINT_FORWARD") {
            print_forward(list);
        }
        else if (command == "PRINT_BACKWARD") {
            print_backward(list);
        }
        else if (command == "PRINT_QUEUE") {
            print_queue(list);
        }
        else if (command == "POP") {
            pop(list);
        }
        else if (command == "GARBAGE_SOFT") {
            garbage(list, 1);
        }
        else if (command == "GARBAGE_HARD") {
            garbage(list, 0);
        }
        else if (command[0] == 'D') {

            if (list.beg != nullptr) {
                if (list.beg == list.end)
                    del_last(list);
                else if (command == "DEL_BEG") {
                    if (is_list(list, list.beg)) {
                        list.qSize--;
                    }
                    del(&list.beg);

                }
                else if (command == "DEL_END") {
                    if (is_list(list, list.end)) {
                        list.qSize--; 
                    }
                    del(&list.end);
                }
                else
                    break;
                list.lSize--;

            }
        }
        else if (command == "SIZE") {
            cout<< list.lSize <<endl;
        }
        else if (command == "COUNT") {
            cout << list.qSize << endl;
        }
        else
            break;
    }
}

int main() {

    input();

}

