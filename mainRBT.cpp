/* author: Anton Amstadt
*  date: 1/10/2022
*  website: antonamstadt.com
*/

#include <iostream>
#include <random>
#include "RBTree.h"

//testing for my RBTree class, examples of how to use the functions
//look at RBTree.h for the declaration/definition of the class
int main() {
	//RBTree<int> t{};
	//t.insert(8);
	//t.displayInOrder();
	//t.insert(100);
	//t.displayInOrder();
	//t.insert(22);
	//t.displayInOrder();
	//t.insert(28);
	//t.displayInOrder();
	//std::cout << '\n';
	//t.insert(26);
	//std::cout << '\n';
	//t.displayInOrder();
	//std::cout << '\n';
	//t.displayPostOrder();

	//std::cout << '\n';
	//std::cout << "testing validity of tree\n";
	//t.validRBTree();
	//std::cout << "if no error message popped up, then tree is valid\n";

	//std::cout << '\n';
	//std::cout << "min: " << t.min() << '\n' << "max: " << t.max();
	//std::cout << '\n';
	//std::cout << t.suc(22);
	//std::cout << '\n';
	//std::cout << t.pred(10);
	//
	//std::cout << '\n';
	//t.del(26);
	//t.del(100);
	//t.del(9);
	//t.displayInOrder();
	//t.displayPostOrder();
	RBTree<int> randTree{};
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 1000);
	for (int i = 0; i < 1000; i++)
		randTree.insert(dist(gen));
	randTree.displayInOrder();
	std::cout << "error message(s) will be shown if tree is invalid:\n";
	randTree.validRBTree();
	randTree.del(50);
	randTree.del(51);
	randTree.del(52);
	randTree.del(53);
	std::cout << "error message(s) will be shown if tree is invalid:\n";
	randTree.validRBTree();
	randTree.del(450);
	randTree.del(451);
	randTree.del(452);
	randTree.del(453);
	std::cout << "error message(s) will be shown if tree is invalid:\n";
	randTree.validRBTree();
	randTree.del(950);
	randTree.del(951);
	randTree.del(952);
	randTree.del(953);
	std::cout << "error message(s) will be shown if tree is invalid:\n";
	randTree.insert(10);
	randTree.validRBTree();

}

