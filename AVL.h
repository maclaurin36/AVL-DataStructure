#include "AVLInterface.h"
#include "Node.h"

class AVL : public AVLInterface {
public:
	AVL();
	~AVL();

	//Please note that the class that implements this interface must be made
	//of objects which implement the NodeInterface

	/*
	* Returns the root node for this tree
	*
	* @return the root node for this tree.
	*/
	Node* getRootNode() const;

	/*
	* Attempts to add the given int to the AVL tree
	* Rebalances the tree if data is successfully added
	*
	* @return true if added
	* @return false if unsuccessful (i.e. the int is already in tree)
	*/
	bool add(int data);

	/*
	* Attempts to remove the given int from the AVL tree
	* Rebalances the tree if data is successfully removed
	*
	* @return true if successfully removed
	* @return false if remove is unsuccessful(i.e. the int is not in the tree)
	*/
	bool remove(int data);

	/*
	* Removes all nodes from the tree, resulting in an empty tree.
	*/
	void clear();
private:
	Node* root;
	Node* find(Node* currentNode, int value);
	void addHelper(Node* currentNode, Node* prevNode, int value);
	void removeHelper(Node* currentNode);
	Node* findRemoveReplacement(Node* currentNode, int iteration);
	int getMaxChildHeight(Node* currentNode);
	int getChildHeightDiff(Node* currentNode);
	void rotateRight(Node* currentNode);
	void rotateLeft(Node* currentNode);
	void rebalance(Node* currentNode);
};