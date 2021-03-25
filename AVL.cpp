#include "AVL.h"

AVL::AVL() {
	root = 0;
}
AVL::~AVL() {
	clear();
}

//Please note that the class that implements this interface must be made
//of objects which implement the NodeInterface

/*
* Returns the root node for this tree
*
* @return the root node for this tree.
*/
Node* AVL::getRootNode() const {
	return root;
}

/*
* Attempts to add the given int to the AVL tree
* Rebalances the tree if data is successfully added
*
* @return true if added
* @return false if unsuccessful (i.e. the int is already in tree)
*/
bool AVL::add(int data) {
	Node* foundNode = find(root, data);

	if (foundNode != 0) {
		return false;
	} else {
		addHelper(root, 0, data);
		return true;
	}
}

/*
* Attempts to remove the given int from the AVL tree
* Rebalances the tree if data is successfully removed
*
* @return true if successfully removed
* @return false if remove is unsuccessful(i.e. the int is not in the tree)
*/
bool AVL::remove(int data) {
	Node* foundNode = find(root, data);

	if (foundNode == 0) {
		return false;
	} else {
		Node* parentNode = foundNode->parent;
		removeHelper(foundNode);

		/*if (parentNode == 0) {
			rebalance(root);
			root->height = getMaxChildHeight(root) + 1;
		}*/

		while (parentNode != 0) {
			rebalance(parentNode);
			parentNode->height = getMaxChildHeight(parentNode) + 1;
			parentNode = parentNode->parent;
		}
		return true;
	}
}

/*
* Removes all nodes from the tree, resulting in an empty tree.
*/
void AVL::clear() {
	while (root != 0) {
		remove(root->data);
	}
}

// --------------------------MY METHODS-------------------------------------------- //
/*
* Determines if a node is in the tree
*
* @return the node if it is found
* @return NULL ptr if it is not found
*/

Node* AVL::find(Node* currentNode, int value) {
	if (currentNode != nullptr) {
		if (value == currentNode->data) {
			return currentNode;
		} else if (value < currentNode->data) {
			return find(currentNode->leftChild, value);
		} else if (value > currentNode->data) {
			return find(currentNode->rightChild, value);
		}
	}

	return 0;
}
/*
* Recursively adds value to a true, and we know the value isn't in the tree
* so we don't need to worry about it possibly not being added
*/
void AVL::addHelper(Node* currentNode, Node* prevNode, int value) {
	bool valueAdded = false;
	if (currentNode == 0) {
		// Extra case for the root (parent isn't defined)
		if (currentNode == root) {
			Node* newNode = new Node(0, value);
			root = newNode;
		} else {
			Node* newNode = new Node(prevNode, value);
			if (prevNode->data > value) {
				prevNode->leftChild = newNode;
			} else {
				prevNode->rightChild = newNode;
			}
		}
		return;
	} else if (value > currentNode->data) {
		addHelper(currentNode->rightChild, currentNode, value);
		// Check conditions to rebalance the tree
		rebalance(currentNode);
		currentNode->height = getMaxChildHeight(currentNode) + 1;
	} else if (value < currentNode->data) {
		addHelper(currentNode->leftChild, currentNode, value);
		// Check conditions to rebalance the tree
		rebalance(currentNode);
		currentNode->height = getMaxChildHeight(currentNode) + 1;
	}
}

Node* AVL::findRemoveReplacement(Node *currentNode, int iteration) {
	Node* parentNode = currentNode;
	if (iteration == 0) {
		currentNode = currentNode->leftChild;
	} else {
		currentNode = currentNode->rightChild;
	}
	if (currentNode == 0) {
		parentNode->height -= 1;
		return parentNode;
	}
	Node* returnNode = findRemoveReplacement(currentNode, iteration + 1);
	// Check conditions to rebalance the tree
	if (currentNode->height != 0 && currentNode->height != 1) {
		rebalance(currentNode);
		currentNode->height = getMaxChildHeight(currentNode) + 1;
	}
	return returnNode;
}

void AVL::removeHelper(Node *currentNode) {
	if (currentNode->leftChild == 0) {
		if (currentNode != root) {
			if (currentNode->parent->leftChild == currentNode) {
				currentNode->parent->leftChild = currentNode->rightChild;
			} else {
				currentNode->parent->rightChild = currentNode->rightChild;
			}
			if (currentNode->rightChild != 0) {
				currentNode->rightChild->parent = currentNode->parent;
			}
		} else {
			root = root->rightChild;
			if (root != 0 && root->parent != 0) {
				root->parent = 0;
			}
		}
		delete currentNode;
	} else if (currentNode->rightChild == 0) {
		if (currentNode != root) {
			if (currentNode->parent->leftChild == currentNode) {
				currentNode->parent->leftChild = currentNode->leftChild;
			} else {
				currentNode->parent->rightChild = currentNode->leftChild;
			}
			if (currentNode->leftChild != 0) {
				currentNode->leftChild->parent = currentNode->parent;
			}
		} else {
			root = root->leftChild;
			if (root != 0 && root->parent != 0) {
				root->parent = 0;
			}
		}
		delete currentNode;
	} else {
		Node* replacement = findRemoveReplacement(currentNode, 0);
		rebalance(currentNode);
		currentNode->height = getMaxChildHeight(currentNode) + 1;
		int replacementData = replacement->data;
		removeHelper(replacement);
		currentNode->data = replacementData;
	}
}

int AVL::getMaxChildHeight(Node *currentNode) {
	int leftHeight = 0;
	int rightHeight = 0;
	int returnHeight = -1;
	if (currentNode->leftChild != 0) {
		leftHeight = currentNode->leftChild->height;
	}
	if (currentNode->rightChild != 0) {
		rightHeight = currentNode->rightChild->height;
	}
	if (leftHeight > rightHeight) {
		returnHeight = leftHeight;
	} else {
		returnHeight = rightHeight;
	}
	return returnHeight;
}

int AVL::getChildHeightDiff(Node* currentNode) {
	int rHeight = 0;
	int lHeight = 0;
	if (currentNode->leftChild != 0) {
		lHeight = currentNode->leftChild->height;
	}
	if (currentNode->rightChild != 0) {
		rHeight = currentNode->rightChild->height;
	}
	return rHeight - lHeight;
}

void AVL::rotateRight(Node* currentNode) {
	currentNode->parent->leftChild = currentNode->rightChild;
	currentNode->rightChild = currentNode->parent;
	currentNode->parent = currentNode->rightChild->parent;
	currentNode->rightChild->parent = currentNode;
	if (currentNode->rightChild->leftChild != 0) {
		currentNode->rightChild->leftChild->parent = currentNode->rightChild;
	}
	if (currentNode->parent != 0) {
		// problem here
		if (((currentNode->parent->rightChild == currentNode->rightChild) && (currentNode->rightChild != 0)) || ((currentNode->parent->rightChild == currentNode->leftChild) && (currentNode->leftChild != 0))) {
			currentNode->parent->rightChild = currentNode;
		} else {
			currentNode->parent->leftChild = currentNode;
		}
	} else {
		root = currentNode;
	}
}
// arg is rotateNodeChild
void AVL::rotateLeft(Node* currentNode) {
	currentNode->parent->rightChild = currentNode->leftChild;
	currentNode->leftChild = currentNode->parent;
	currentNode->parent = currentNode->leftChild->parent;
	currentNode->leftChild->parent = currentNode;
	if (currentNode->leftChild->rightChild != 0) {
		currentNode->leftChild->rightChild->parent = currentNode->leftChild;
	}
	if (currentNode->parent != 0) {
		// problem here
		if (((currentNode->parent->rightChild == currentNode->rightChild) && (currentNode->rightChild != 0)) || ((currentNode->parent->rightChild == currentNode->leftChild) && (currentNode->leftChild != 0))) {
			currentNode->parent->rightChild = currentNode;
		} else {
			currentNode->parent->leftChild = currentNode;
		}
	} else {
		root = currentNode;
	}
}

void AVL::rebalance(Node* currentNode) {
	int curHeightDiff = getChildHeightDiff(currentNode);
	if (curHeightDiff == -2) {
		int leftChildHeightDiff = getChildHeightDiff(currentNode->leftChild);
		if (leftChildHeightDiff == -1 || leftChildHeightDiff == 0) {
			rotateRight(currentNode->leftChild);
		} else if (leftChildHeightDiff == 1) {
			rotateLeft(currentNode->leftChild->rightChild);
			rotateRight(currentNode->leftChild);
		}
		// Adjust heights for current, parent, and cur-par-left
		currentNode->height = getMaxChildHeight(currentNode) + 1;
		if (currentNode->parent != 0) {
			currentNode->parent->leftChild->height = getMaxChildHeight(currentNode->parent->leftChild) + 1;
			currentNode->parent->height = getMaxChildHeight(currentNode->parent) + 1;
		}
	} else if (curHeightDiff == 2) {
		int rightChildHeightDiff = getChildHeightDiff(currentNode->rightChild);
		if (rightChildHeightDiff == 1 || rightChildHeightDiff == 0) {
			rotateLeft(currentNode->rightChild);
		} else if (rightChildHeightDiff == -1) {
			rotateRight(currentNode->rightChild->leftChild);
			rotateLeft(currentNode->rightChild);
		}
		// Adjust heights for current, parent, and cur-par-right
		currentNode->height = getMaxChildHeight(currentNode) + 1;
		if (currentNode->parent != 0) {
			currentNode->parent->rightChild->height = getMaxChildHeight(currentNode->parent->rightChild) + 1;
			currentNode->parent->height = getMaxChildHeight(currentNode->parent) + 1;
		}
	}
	currentNode->height = getMaxChildHeight(currentNode) + 1;
}