#include <iostream>
#include <queue>
#include <stack>

// this class should go inside the BinarySearchTree class
// can't fix it at the moment
template <typename T>
struct Node {
	T value;
	Node* left = nullptr;
	Node* right = nullptr;

	Node(const T& value, Node* left = nullptr, Node* right = nullptr)
		: value(value), left(left), right(right) { }
	Node(const Node<T>& other) = delete;
	Node& operator=(const Node<T>& other) = delete;

	// the only benefit from this destr is that you can easily delete the whole tree
	~Node() {
		free();
	}
private:
	void free() {
		delete left;
		delete right;
		left = right = nullptr;
	}
};

template <typename T>
class BinarySearchTree {
private:
	Node<T>* root = nullptr;

	Node<T>* _insert(Node<T>* current, const T& value);
	bool _contains(const Node<T>* current, const T& value) const;
	Node<T>* _remove(Node<T>* current, const T& value);
	void _dfs(const Node<T>* current) const;
public:
	BinarySearchTree() = default;
	BinarySearchTree(const BinarySearchTree<T>& other) = delete; // use the deepCopy func at the end if you want to implement them
	BinarySearchTree<T>& operator=(const BinarySearchTree<T>& other) = delete;
	~BinarySearchTree() { free(); }

	bool contains(const T&) const;
	void insert(const T&);
	void remove(const T&);

	// might not need these
	void dfs() const;
	void iterativeDfs() const;
	void bfs() const;
private:
	void free();
};

template <typename T>
void BinarySearchTree<T>::free() {
	delete root;
}

template <typename T>
Node<T>* BinarySearchTree<T>::_insert(Node<T>* current, const T& value) {
	if (!current) {
		return new Node<T>(value);
	}

	if (current->value < value) {
		current->right = _insert(current->right, value);
	}
	else if (value < current->value) {
		current->left = _insert(current->left, value);
	}

	return current;
}

template <typename T>
bool BinarySearchTree<T>::_contains(const Node<T>* current, const T& value) const {
	if (!current) {
		return false;
	}

	if (current->value == value) {
		return true;
	}

	return current->value > value ? contains(current->left, value) : contains(current->right, value);
}

template <typename T>
Node<T>* BinarySearchTree<T>::_remove(Node<T>* current, const T& value) {
	if (!current) {
		return nullptr;
	}

	if (value < current->value) {
		current->left = _remove(current->left, value);
	}
	else if (current->value < value) {
		current->right = _remove(current->right, value);
	}
	else {
		if (!current->left && !current->right) {
			delete current;
			return nullptr;
		}
		if (!current->left) {
			auto temp = current->right;
			// needed because deleting the node deletes also the children
			current->right = nullptr;
			delete current;
			return temp;
		}
		if (!current->right) {
			auto temp = current->left;
			current->left = nullptr;
			delete current;
			return temp;
		}

		Node<T>* iter = current->right;
		while (iter->left) {
			iter = iter->left;
		}

		current->value = iter->value;
		current->right = _remove(current->right, current->value);
	}

	return current;
}

template <typename T>
bool BinarySearchTree<T>::contains(const T& value) const {
	return _contains(root, value);
}

template <typename T>
void BinarySearchTree<T>::insert(const T& value) {
	root = _insert(root, value);
}

template <typename T>
void BinarySearchTree<T>::remove(const T& value) {
	root = _remove(root, value);
}

template <typename T>
void BinarySearchTree<T>::_dfs(const Node<T>* current) const {
	if (!current) {
		return;
	}

	_dfs(current->left);
	std::cout << current->value << " ";
	_dfs(current->right);
}

template <typename T>
void BinarySearchTree<T>::dfs() const {
	_dfs(root);
}

template <typename T>
void BinarySearchTree<T>::iterativeDfs() const {
	// inorder
	// implement similarly to forward iterator for a binary tree
	std::stack<Node<T>*> stack;
	Node<T>* current = root;

	while (current != nullptr || !stack.empty()) {
		while (current != nullptr) {
			stack.push(current);
			current = current->left;
		}
		current = stack.top();
		stack.pop();
		std::cout << current->value << " ";
		current = current->right;
	}
}

template <typename T>
void BinarySearchTree<T>::bfs() const {
	std::queue<Node<T>*> container;
	container.push(root);

	while (!container.empty()) {
		size_t levelSize = container.size();
		while (levelSize > 0) {
			Node<T>* current = container.front();
			container.pop();

			if (current) {
				std::cout << current->value << " ";
				container.push(current->left);
				container.push(current->right);
			}
			levelSize--;
		}
		std::cout << std::endl;
	}
}

// in case you want cc and op=
Node<int>* deepCopy(const Node<int>* curr) {
	if (!curr) {
		return nullptr;
	}

	return new Node<int>(curr->value, deepCopy(curr->left), deepCopy(curr->right));
}

int main() {
	std::vector<int> v;
	BinarySearchTree<int> bst;
	bst.insert(7);
	bst.insert(4);
	bst.insert(2);
	bst.insert(6);
	bst.insert(5);
	bst.insert(8);
	bst.insert(3);
	bst.insert(1);
	bst.bfs();
    bst.dfs();
}
