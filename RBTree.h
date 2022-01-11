/* author: Anton Amstadt 
*  date: 1/10/2022 
*  website: antonamstadt.com
*/

#ifndef GENERIC_RBTREE_H
#define GENERIC_RBTREE_H

#include <iostream>
#include <stack>

/* operations of a red-black tree:
* insert, delete, search, successor, predecessor, minimum, maximum
* These operations can be found in the public functions: 
* insert(), del(), search(), suc(), pred(), min(), max()
*/

/*
* red-black tree properties:
* 1. every node is either red or black
* 2. the root is black
* 3. every leaf (nullNode) is black
* 4. if a node is red, then both of its children is black
* 5. for each node, all simple paths from the node to descendant leaves contain
* the same number of black nodes
*/

// T must have default constructor and support following operators: < , == , ostream operator<<
template <class T>
class RBTree {
	struct Node {
		T val;
		Node* p; //parent
		Node* left;
		Node* right;
		int color; //Red = 1, Black = 0
		Node(T value) : val{ value }, p{ nullptr }, left{ nullptr }, right{ nullptr }, color{ 1 }{}

	};

	Node* root;
	Node* nullNode;

	void left_rotate(Node*);
	void right_rotate(Node*);
	void insert(Node*&);
	void insert_fixup(Node*);
	void deleteSubTree(Node*&);
	void display(const Node*) const;
	void rbTransplant(Node*, Node*);
	void rbDelete(Node*);
	void rbDeleteFixup(Node*);
	void rbTest(Node*, int, int) const;
	Node* minimum(Node*) const;
	Node* maximum(Node*) const;
	Node* searchNode(T) const;
	Node* successorNode(Node*) const;
	Node* predecessorNode(Node*) const;

public:
	RBTree();
	~RBTree();
	void displayInOrder() const;
	void displayPostOrder() const;
	void insert(T);
	bool del(T);
	bool search(T) const;
	T max() const;
	T min() const;
	T pred(T) const;
	T suc(T) const;
	void validRBTree() const;

};

//constructor allocates memory for nullNode and properly sets its values. also sets root to nullNode
template <class T>
RBTree<T>::RBTree() : root{ nullptr }, nullNode{ new Node{T{}} } {
	nullNode->color = 0;
	nullNode->left = nullptr;
	nullNode->right = nullptr;
	root = nullNode;
}

//in a tree with no duplicate values, returns the next-smallest value in the tree
template <class T>
T RBTree<T>::pred(T value) const {
	Node* n{ predecessorNode(searchNode(value)) };
	if (n != nullptr)
		return n->val;
	else {
		std::cout << "pred called with invalid argument: " << value << '\n';
		return T{};
	}

}

//in a tree with no duplicate values, successorNode returns pointer to the node with the next-smallest value
template <class T>
typename RBTree<T>::Node* RBTree<T>::predecessorNode(Node* n) const {
	if (n == nullptr)
		return nullptr;
	if (n->left != nullNode)
		return maximum(n->left);
	Node* y = n->p;
	while (y != nullptr && n == y->left) {
		n = y;
		y = y->p;
	}
	return y;
}

//in a tree with no duplicate values, returns the next-largest value in the tree
template <class T>
T RBTree<T>::suc(T value) const {
	Node* n{ successorNode(searchNode(value)) };
	if (n != nullptr)
		return n->val;
	else {
		std::cout << "suc called with invalid argument: " << value << '\n';
		return T{};
	}
}

//in a tree with no duplicate values, successorNode returns pointer to the node with the next-largest value
template <class T>
typename RBTree<T>::Node* RBTree<T>::successorNode(Node* n) const {
	if (n == nullptr)
		return nullptr;
	if (n->right != nullNode)
		return minimum(n->right);
	Node* y = n->p;
	while (y != nullptr && n == y->right) {
		n = y;
		y = y->p;
	}
	return y;
}

//return minimum of the tree, returns default value of T if nothing in tree
template <class T>
T RBTree<T>::min() const {
	Node* n{ minimum(root) };
	if (n != nullNode)
		return n->val;
	else
		return T{};
}

//return maximum of the tree, returns default value of T if nothing in tree
template <class T>
T RBTree<T>::max() const {
	Node* n{ maximum(root) };
	if (n != nullNode)
		return n->val;
	else
		return T{};
}

//if value found in a node in the tree - return true, else return false
template <class T>
bool RBTree<T>::search(T value) const {
	Node* n{ root };
	while (n != nullNode) {
		if (value == n->val)
			return true;
		if (value < n->val)
			n = n->left;
		else
			n = n->right;
	}
	return false;
}

//returns pointer to first node found with val matching the argument, if no node found - return nullptr
template <class T>
typename RBTree<T>::Node* RBTree<T>::searchNode(T value) const {
	Node* n{ root };
	while (n != nullNode) {
		if (value == n->val)
			return n;
		if (value < n->val)
			n = n->left;
		else
			n = n->right;
	}
	return nullptr;
}

//return true if deletion of node successful, return false if node not found
//public function for deleting nodes
template <class T>
bool RBTree<T>::del(T value) {
	Node* n{ searchNode(value) };
	if (n) {
		rbDelete(n);
		return true;
	}
	else
		return false;
}

//private function to return the node with the minimum value starting from nodePtr
template <class T>
typename RBTree<T>::Node* RBTree<T>::minimum(Node* nodePtr) const {
	if (nodePtr == nullNode || nodePtr == nullptr)
		return nodePtr;
	while (nodePtr->left != nullNode)
		nodePtr = nodePtr->left;
	return nodePtr;
}

//private function to return the node with the maximum value starting from nodePtr
template <class T>
typename RBTree<T>::Node* RBTree<T>::maximum(Node* nodePtr) const {
	if (nodePtr == nullNode || nodePtr == nullptr)
		return nodePtr;
	while (nodePtr->right != nullNode)
		nodePtr = nodePtr->right;
	return nodePtr;
}

//used by the rbDelete funciton
template <class T>
void RBTree<T>::rbTransplant(Node* u, Node* v) {
	if (u->p == nullptr)
		root = v;
	else if (u == u->p->left)
		u->p->left = v;
	else
		u->p->right = v;
	v->p = u->p;
}

// deletes a node in the tree
template <class T>
void RBTree<T>::rbDelete(Node* z) {
	Node* x;
	Node* y = z;
	int y_orig_color = y->color;
	if (z->left == nullNode) {
		x = z->right;
		rbTransplant(z, z->right);
	}
	else if (z->right == nullNode) {
		x = z->left;
		rbTransplant(z, z->left);
	}
	else {
		y = minimum(z->right);
		y_orig_color = y->color;
		x = y->right;
		if (y->p == z)
			x->p = y;
		else {
			rbTransplant(y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		rbTransplant(z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	if (y_orig_color == 0)
		rbDeleteFixup(x);
}

//makes the tree into a valid rb tree after a delete
template <class T>
void RBTree<T>::rbDeleteFixup(Node* x) {
	Node* w;
	while (x != root && x->color == 0) {
		if (x == x->p->left) {
			w = x->p->right;
			if (w->color == 1) {
				w->color = 0;
				x->p->color = 1;
				left_rotate(x->p);
				w = x->p->right;
			}
			if (w->left->color == 0 && w->right->color == 0) {
				w->color = 1;
				x = x->p;
			}
			else {
				if (w->right->color == 0) {
					w->left->color = 0;
					w->color = 1;
					right_rotate(w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = 0;
				w->right->color = 0;
				left_rotate(x->p);
				x = root;
			}
		}
		else {
			w = x->p->left;
			if (w->color == 1) {
				w->color = 0;
				x->p->color = 1;
				right_rotate(x->p);
				w = x->p->left;
			}
			if (w->right->color == 0 && w->left->color == 0) {
				w->color = 1;
				x = x->p;
			}
			else {
				if (w->left->color == 0) {
					w->right->color = 0;
					w->color = 1;
					left_rotate(w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = 0;
				w->left->color = 0;
				right_rotate(x->p);
				x = root;
			}
		}
	}
	x->color = 0;
}

//the public function for inserting a value into the tree
template <class T>
void RBTree<T>::insert(T value) {
	Node* nodePtr{ new Node{value} };
	nodePtr->left = nullNode;
	nodePtr->right = nullNode;
	insert(nodePtr);
}

// z is assumed to be: red, have some value for val, have left/right child as nullNode
// insert a node into the tree and maybe call insertFixup
template <class T>
void RBTree<T>::insert(Node*& z) {
	Node* y{ nullptr };
	Node* x{ root };
	while (x != nullNode) {
		y = x;
		if (z->val < x->val)
			x = x->left;
		else
			x = x->right;
	}
	z->p = y;
	if (y == nullptr) {
		root = z;
	}
	else if (z->val < y->val)
		y->left = z;
	else
		y->right = z;

	if (z->p == nullptr) {
		z->color = 0;
		return;
	}

	//insert_fixup requires grandparent to be not be null, should leave the tree in valid RB state
	if (z->p->p == nullptr)
		return;

	insert_fixup(z);
}

// z is required to have a valid grandparent, and the have been inserted into a valid RB tree
// make the tree a valid rb tree after an insert
template <class T>
void RBTree<T>::insert_fixup(Node* z) {
	Node* y;
	// while z->p is red
	while (z->p->color == 1) {
		if (z->p == z->p->p->left) {
			y = z->p->p->right;
			if (y->color == 1) {
				z->p->color = 0;
				y->color = 0;
				z->p->p->color = 1;
				z = z->p->p;
			}
			else {
				if (z == z->p->right) {
					z = z->p;
					left_rotate(z);
				}
				z->p->color = 0;
				z->p->p->color = 1;
				right_rotate(z->p->p);
			}
		}
		else {
			y = z->p->p->left;
			if (y->color == 1) {
				z->p->color = 0;
				y->color = 0;
				z->p->p->color = 1;
				z = z->p->p;
			}
			else {
				if (z == z->p->left) {
					z = z->p;
					right_rotate(z);
				}
				z->p->color = 0;
				z->p->p->color = 1;
				left_rotate(z->p->p);
			}
		}
		if (z == root)
			break;
	}
	root->color = 0;
}

// right_rotate assumes that x.left != nullptr and that the parent of the root is nullptr
// rotates nodes
template <class T>
void RBTree<T>::right_rotate(Node* x) {
	Node* y = x->left;
	x->left = y->right;
	if (y->right != nullNode)
		y->right->p = x;
	y->p = x->p;
	if (x->p == nullptr) {
		root = y;
	}
	else if (x == x->p->right)
		x->p->right = y;
	else
		x->p->left = y;
	y->right = x;
	x->p = y;
}

// left_rotate assumes that x.right != nullptr and that the parent of the root is nullptr
// rotates nodes
template <class T>
void RBTree<T>::left_rotate(Node* x) {
	Node* y = x->right;
	x->right = y->left;
	if (y->left != nullNode)
		y->left->p = x;
	y->p = x->p;
	if (x->p == nullptr)
		root = y;
	else if (x == x->p->left)
		x->p->left = y;
	else
		x->p->right = y;
	y->left = x;
	x->p = y;
}

//destructor - free all dynamically allocated memory
template <class T>
RBTree<T>::~RBTree() {
	deleteSubTree(root);
	delete nullNode;
}

//deletes nodes recursively, used in destructor
template <class T>
void RBTree<T>::deleteSubTree(Node*& nodePtr) {
	if (nodePtr != nullNode) {
		if (nodePtr->left)
			deleteSubTree(nodePtr->left);
		if (nodePtr->right)
			deleteSubTree(nodePtr->right);
		delete nodePtr;
	}
}

//recursive function used in validRBTree() to verify the tree has the properties of a rb tree
template <class T>
void RBTree<T>::rbTest(Node* n, int cur_b, int total_b) const {
	if (n) {
		if (n->color == 0)
			cur_b++;
		else if (n->color != 1)
			std::cout << "INVALID TREE: node with color other than red or black\n";
		else if (n->left->color == 1 || n->right->color == 1)
			std::cout << "INVALID TREE: red node with red child\n";
		if (n->left == nullptr || n->right == nullptr) {
			if (cur_b != total_b)
				std::cout << "INVALID TREE: different number of black nodes on path\n";
			if (n != nullNode)
				std::cout << "INVALID TREE: somehow a non-nullNode has a left and/or right child set as nullptr\n";
			else if (n->color != 0)
				std::cout << "INVALID TREE: nullNode is not black!\n";
		}
		if (n->left)
			rbTest(n->left, cur_b, total_b);
		if (n->right)
			rbTest(n->right, cur_b, total_b);
	}
}

//prints error message(s) if the tree is invalid in some way, if tree is valid - prints nothing
template <class T>
void RBTree<T>::validRBTree() const {
	int total_b_node_in_path{ 0 };
	Node* n{ root };
	if (root->color != 0) {
		std::cout << "INVALID TREE: root is not black (violates rule 1 of rb trees). will not check rest of tree\n";
		return;
	}
	while (n != nullptr) {
		if (n->color == 0)
			total_b_node_in_path++;
		n = n->left;
	}
	rbTest(root, 0, total_b_node_in_path);
}

//iterative post order traversal
template <class T>
void RBTree<T>::displayPostOrder() const {
	Node* t{ nullptr };
	Node* n{ root };
	std::stack<Node*> s;
	do {
		while (n) {
			if (n->right)
				s.push(n->right);
			s.push(n);
			n = n->left;
		}
		n = s.top();
		s.pop();

		if (s.empty())
			t = nullptr;
		else
			t = s.top();

		if (n->right && t == n->right) {
			s.pop();
			s.push(n);
			n = n->right;
		}
		else {
			display(n);
			n = nullptr;
		}
	} while (!s.empty());
}

//iterative in order traversal
template <class T>
void RBTree<T>::displayInOrder() const {
	Node* nodePtr{ root };
	std::stack<Node*> s;
	while (nodePtr || s.empty() == false) {
		while (nodePtr) {
			s.push(nodePtr);
			nodePtr = nodePtr->left;
		}
		nodePtr = s.top();
		s.pop();
		display(nodePtr);
		nodePtr = nodePtr->right;
	}
}

//display information for a node
template <class T>
void RBTree<T>::display(const Node* nodePtr) const {
	if (nodePtr == nullNode)
		return;
	std::cout << "value: " << nodePtr->val << " * color: ";
	if (nodePtr->color)
		std::cout << "red";
	else
		std::cout << "black";
	std::cout << " * parent value: ";
	if (nodePtr->p)
		std::cout << nodePtr->p->val;
	else
		std::cout << "nullptr";
	std::cout << " * left value: ";
	if (nodePtr->left) {
		if (nodePtr->left == nullNode)
			std::cout << "nullNode";
		else
			std::cout << nodePtr->left->val;
	}
	else
		std::cout << "nullptr";
	std::cout << " * right value: ";
	if (nodePtr->right) {
		if (nodePtr->right == nullNode)
			std::cout << "nullNode";
		else
			std::cout << nodePtr->right->val;
	}
	else
		std::cout << "nullptr";
	std::cout << '\n';
}

#endif
