#include <iostream>
#include <string>

using namespace std;

struct Node {
    int data;
    Node* ptr;
};

struct Pointers {
    Node * beg = nullptr, * act[3], * end = nullptr;
};

Node* ptr_xor(Node* a, Node* b) {
    return (Node*)((uintptr_t)(a) ^ (uintptr_t)(b));
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
    points.act[1] = node;
    points.act[0] = node;
    points.act[2] = node;
    points.end = node;

}

void add_beg(int value, Pointers& points) {

    if (points.beg == nullptr) {
        add_first(value, points);
        return;
    }

    add(value, &points.beg, NULL);

    if (points.beg->ptr == points.act[1]) {
        points.act[0] = points.beg;
    }
}

void add_end(int value, Pointers& points) {

    if (points.end == nullptr) {
        add_first(value, points);
        return;
    }

    add(value, &points.end, NULL);

    if (points.end->ptr == points.act[1]) {
        points.act[2] = points.end;
    }
}

void add_act(int value, Pointers& points) {

    if (points.act[1] == NULL) {
        add_first(value, points);
        return;
    }

    if (points.act[1] == points.beg) {
        add_beg(value, points);
        return;
    }

    Node* node = add(value, &points.act[0], points.act[1]);

    if (points.act[1] == points.end) {
        points.act[1]->ptr = node;
    }
    else {
        points.act[1]->ptr = ptr_xor(node, points.act[2]);
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

void print_act(Pointers points) {

    if (points.act[1] == NULL)
        cout << "NULL" << endl;
    else
        cout << points.act[1]->data << endl;
}

void print(Node** act, Node** other, Node** changed, Node* cover, Node* secCover) {

    if ((*act) == cover && (*act) != NULL) {
        cout << secCover->data << endl;
        (*act) = secCover;
        (*other) = cover;
        (*changed) = secCover->ptr;
    }
    else if ((*changed) == NULL)
        cout << "NULL" << endl;
    else {
        cout << (*changed)->data << endl;

        Node* tmp = ptr_xor((*changed)->ptr, (*act));

        (*other) =(*act);
        (*act) = (*changed);
        (*changed) = tmp;
    }
}

#pragma endregion

#pragma region delete

void del_last(Pointers& points) {

    delete points.beg;
    points.act[0] = NULL;
    points.act[1] = NULL;
    points.act[2] = NULL;
    points.beg = nullptr;
    points.end = nullptr;

}

bool del(Node* act, Node** changed) {

    bool changing = 0;

    if ((*changed) == act) {
        changing = 1;
    }

    Node* node = (*changed);
    (*changed) = (*changed)->ptr;
    (*changed)->ptr = ptr_xor(node, (*changed)->ptr);

    delete node;
    return changing;
}

void del_act(Pointers& points) {

    if (points.act[1] == points.end) {
        del(points.act[1], &points.end);
        points.act[1] = points.end;
    }
    else if (points.act[1] == points.beg) {
        del(points.act[1], &points.beg);
        points.act[1] = points.end;
    }
    else {
        Node* node = points.act[1];


        if (points.act[1] == points.beg->ptr) {
            points.act[1] = points.beg;
            points.act[0] = points.end;
            points.act[1]->ptr = points.act[2];
            points.act[2]->ptr = ptr_xor(ptr_xor(node, points.act[2]->ptr), points.act[1]);
        }
        else {
            points.act[1] = points.act[0];
            points.act[0] = ptr_xor(points.act[1]->ptr, node);

            if (points.act[1] == points.end->ptr)
                points.act[2]->ptr = points.act[1];
            else
                points.act[2]->ptr = ptr_xor(points.act[1], ptr_xor(node, points.act[2]->ptr));

            points.act[1]->ptr = ptr_xor(points.act[0], points.act[2]);
        }


        delete node;
    }

}

void del_val(int value, Pointers& points) {

    Node* curr = points.beg, * next, * prev = NULL;

    while (curr != NULL) {
        
        next = ptr_xor(prev, curr->ptr);

        if (curr->data == value) {

            if (curr == points.end) {
                if (del(points.act[1], &points.end)) {
                    points.act[1] = points.end;
                }
                return;
            }
            else if (curr == points.beg) {
                if (del(points.act[1], &points.beg))
                    points.act[1] = points.end;
            }
            else if (curr == points.act[1])
                del_act(points);
            else {
                prev->ptr = ptr_xor(ptr_xor(prev->ptr, curr), next);
                next->ptr = ptr_xor(ptr_xor(curr, next->ptr), prev);
            }
        }
        else
            prev = curr; 
        
            
        curr = next;
        
    }
          
}

#pragma endregion

void input(Pointers &points) {

    string command;

    while(cin >> command) {
      
        if (command == "ADD_BEG") {
            int n;
            cin >> n;
            add_beg(n, points);
        }
        else if (command == "ADD_END") {
            int n;
            cin >> n;
            add_end(n, points);
        }
        else if (command == "ADD_ACT") {
            int n;
            cin >> n;
            add_act(n, points);
        }
        else if (command == "PRINT_FORWARD") {
            print_forward(points);
        }
        else if (command == "PRINT_BACKWARD") {
            print_backward(points);
        }
        else if (command == "ACTUAL") {
            print_act(points);
        }
        else if (command == "NEXT") {
            print(&points.act[1], &points.act[0], &points.act[2], points.end, points.beg);
        }
        else if (command == "PREV") {
            print(&points.act[1], &points.act[2], &points.act[0], points.beg, points.end);
        }
        else if (command == "DEL_VAL") {
            int n;
            cin >> n;
            if (points.beg != nullptr)
                if (points.beg == points.end && points.beg->data == n)
                    del_last(points);
                else
                    del_val(n, points);
        }
        else if (command[0] == 'D') {

            if (points.beg != nullptr)
                if (points.beg == points.end)
                    del_last(points);
                else if (command == "DEL_BEG") {
                    if (del(points.act[1], &points.beg))
                        points.act[1] = points.end;
                }
                else if (command == "DEL_END") {
                    if (del(points.act[1], &points.end)) {
                        points.act[1] = points.end;
                    }
                }
                else if (command == "DEL_ACT")
                    del_act(points);
                else
                    break;
        }
        else
            break;
    }
}

int main(){
    
    Pointers points;
    points.act[0] = NULL;
    points.act[1] = NULL;
    points.act[2] = NULL;

    input(points);

}

