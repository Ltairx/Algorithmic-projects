#include <iostream>
using namespace std;

class BNode {
public:
	bool isLeaf;
	int cntKey;
	int t;
	int* keys;
	BNode** sons;
	BNode(int _t, bool _isLeaf);
	void traverse();
    bool BTreeFind(int k);
	void insertNonFull(int k);
	void split(int i, BNode* y);
};

class BTree {
public:
	BNode* root;
	int t;
	BTree(int _t) { root = NULL;  t = _t; }
	void traverse(){
		if (root != NULL) root->traverse();
	}
    bool BTreeFind(int k){
		return (root == NULL) ? false : root->BTreeFind(k);
	}
	void insert(int k);
};

BNode::BNode(int _t, bool _isLeaf){

	t = _t;
	isLeaf = _isLeaf;

	keys = new int[2 * t - 1];
	sons = new BNode * [2 * t];

	cntKey = 0;
}

void BNode::traverse(){

	int i;
	for (i = 0; i < cntKey; i++){

		if (!isLeaf)
			sons[i]->traverse();
		cout << keys[i] << " ";
	}

	if (!isLeaf)
		sons[i]->traverse();
}

bool BNode::BTreeFind(int k){

	int i = 0;
	while (i < cntKey && k > keys[i])
		i++;

	if (keys[i] == k)
		return true;

	if (isLeaf)
		return false;

	return sons[i]->BTreeFind(k);
}

void BTree::insert(int k){

    if (root == NULL){
        root = new BNode(t, true);
        root->keys[0] = k;
        root->cntKey = 1; 
    }
    else{
       
        if (root->cntKey == 2 * t - 1){

            BNode* s = new BNode(t, false);

            s->sons[0] = root;

            s->split(0, root);

            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->sons[i]->insertNonFull(k);

            root = s;
        }
        else  
            root->insertNonFull(k);
    }
}

void BNode::insertNonFull(int k){

    int i = cntKey - 1;

    if (isLeaf){
        while (i >= 0 && keys[i] > k){
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = k;
        cntKey ++;
    }
    else{

        while (i >= 0 && keys[i] > k)
            i--;

        if (sons[i + 1]->cntKey == 2 * t - 1){

            split(i + 1, sons[i + 1]);

            if (keys[i + 1] < k)
                i++;
        }
        sons[i + 1]->insertNonFull(k);
    }
}

void BNode::split(int i, BNode* y){

    BNode* z = new BNode(y->t, y->isLeaf);
    z->cntKey = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    if (!(y->isLeaf)){
        for (int j = 0; j < t; j++)
            z->sons[j] = y->sons[j + t];
    }

    y->cntKey = t - 1;

    for (int j = cntKey; j >= i + 1; j--)
        sons[j + 1] = sons[j];

    sons[i + 1] = z;

    for (int j = cntKey - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];

    cntKey ++;
}

int main()
{
	int t = 2;
    char command;
    cin >> command;
	cin >> t;
	BTree tree(t);

    while (cin >> command) {
        if (command == 'A') {
            int x;
            cin >> x;
            tree.insert(x);
        }
        else if (command == '?') {
            int x;
            cin >> x;
            cout << x;
            if (tree.BTreeFind(x))
                cout << " +" << endl;
            else
                cout << " -" << endl;
        }
        else if (command == 'P') {
            tree.traverse();
            cout << endl;
        }
        else if (command == 'X')
            break;
    }


}
