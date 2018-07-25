#pragma once


template<typename Type>
class BST
{
private:
	struct Node
	{
		Type data;
		Node* left, *right;
	};

	Node *root;


public:
	BST();
	~BST();
	BST& operator=(const BST& that);
	BST(const BST& that);
	void insert(const Type& v);
	bool findAndRemove(const Type& v);
	bool find(const Type& v) const;
	void clear();
	void printInOrder() const;

	void CopyInOrder(Node const * const add);
	void DeletePostOrder(Node *remov);
	void Print(Node* me) const;
};

template<typename Type>
BST<Type>::BST(){
	root = nullptr;
}

template<typename Type>
BST<Type>::~BST(){
	clear();
	delete root;
}

template<typename Type>
BST<Type>& BST<Type>::operator=(const BST& that){
	if (this != &that){
		clear();
		CopyInOrder(that.root);
	}
	return *this;
}

template<typename Type>
void BST<Type>::CopyInOrder(Node const * const  add) {
	if (add == NULL){
		return;
	}
	else{
		insert(add->data);
		CopyInOrder(add->left);
		CopyInOrder(add->right);
	}
}

template<typename Type>
BST<Type>::BST(const BST& that){
	root = nullptr;
	*this = that;
}

template<typename Type>
void BST<Type>::insert(const Type& v){
	Node* temp = new Node;
	temp->data = v;
	temp->left = NULL;
	temp->right = NULL;
	Node* parent = root;
	Node *current = nullptr;
	while (true){
		if (parent == NULL){
			root = temp;
			break;
		}
		if (v < parent->data)
			current = parent->left;
		else
			current = parent->right;

		if (current == NULL){
			if (v < parent->data){
				parent->left = temp;
				break;
			}
			else {
				parent->right = temp;
				break;
			}
		}
		if (v < parent->data)
			parent = parent->left;
		else
			parent = parent->right;
	}
}

template<typename Type>
bool BST<Type>::findAndRemove(const Type& v){

	Node* parent = nullptr;
	Node *current = root;



	while (true){
		if (current == NULL){
			return false;
		}

		/////////////////////// NO CHILDREN
		if (current->data == v){
			if (current == root && root->left == NULL && root->right == NULL){
				delete root;
				root = nullptr;
				return true;
			}
			if (current->left == NULL && current->right == NULL){
				if (v < parent->data)
					parent->left = NULL;
				else
					parent->right = NULL;
				delete current;
				return true;
			}
			/////////////////////// NO CHILDREN END
			////////////////////// ONE CHILD - LEFT
			if (current == root && root->left != NULL && root->right == NULL){
				root = current->left;
				delete current;
				return true;
			}
			if (current->left != NULL && current->right == NULL){
				if (v < parent->data){
					parent->left = current->left;
					delete current;
					return true;
				}
				else{
					parent->right = current->left;
					delete current;
					return true;
				}
			}
			////////////////////// ONE CHILD - LEFT   END
			////////////////////// ONE CHILD - RIGHT
			if (current == root && root->left == NULL && root->right != NULL){
				root = current->right;
				delete current;
				return true;
			}
			if (current->left == NULL && current->right != NULL){
				if (v < parent->data){
					parent->left = current->right;
					delete current;
					return true;
				}
				else{
					parent->right = current->right;
					delete current;
					return true;
				}
			}
			////////////////////// ONE CHILD - RIGHT   END
			////////////////////// TWO CHILDREN
			else{
				Node* max = current;
				parent = current->right;
				current = parent->left;

				while (current->left != NULL){ // find min
					current = current->left;
					parent = parent->left;
				}

				Type a = current->data; // swap
				current->data = max->data;
				max->data = a;

				/////////////////////// NO CHILDREN
				if (current == root && current->left == NULL && current->right == NULL){
					delete root;
					root = nullptr;
					return true;
				}
				else if (current->left == NULL && current->right == NULL){
					parent->left = NULL;
					delete current;
					return true;
				}
				/////////////////////// NO CHILDREN END
				//////////////////////// ONE CHILD - LEFT
				//if (current == root && root->left != NULL && root->right == NULL){
				//	root = current->left;
				//	delete current;
				//	return true;
				//}
				//else if (current->left != NULL && current->right == NULL){
				//	//if (v < parent->data){
				//	parent->left = current->left;
				//	delete current;
				//	return true;
				//	//}
				//	//else{
				//	//	parent->right = current->left;
				//	//	delete current;
				//	//	return true;
				//	//}
				//}
				//////////////////////// ONE CHILD - LEFT   END
				////////////////////// ONE CHILD - RIGHT
				if (current == root && root->left == NULL && root->right != NULL){
					root = current->right;
					delete current;
					return true;
				}
				else if (current->left == NULL && current->right != NULL){

					parent->left = current->right;
					delete current;
					return true;
				}
				////////////////////// ONE CHILD - RIGHT   END

			}
		}
		//////////////////// FIND
		if (v < current->data){
			parent = current;
			current = current->left;

		}
		else{
			parent = current;
			current = current->right;
		}
	}

	return false;
}

template<typename Type>
bool BST<Type>::find(const Type& v) const{
	Node *current = root;
	while (true){
		if (current == NULL)
			return false;
		if (v == current->data)
			return true;
		if (v < current->data)
			current = current->left;
		else
			current = current->right;
	}

}

template<typename Type>
void BST<Type>::clear(){
	if (this == NULL){
		return;
	}
	else{
		DeletePostOrder(root);
		root = NULL;
	}
}

template<typename Type>
void BST<Type>::DeletePostOrder(Node *remov){
	if (remov == NULL){
		return;
	}
	else{
		DeletePostOrder(remov->left);
		DeletePostOrder(remov->right);
		delete remov;
	}
}

template<typename Type>
void BST<Type>::printInOrder() const{
	if (this == NULL){
		return;
	}
	else{
		Print(this->root);
		cout << '\n';
	}
}

template<typename Type>
void BST<Type>::Print(Node* me) const{
	if (me == NULL){
		return;
	}
	else{
		Print(me->left);
		cout << me->data << " ";
		Print(me->right);
	}

}