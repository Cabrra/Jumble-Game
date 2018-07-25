#pragma once

template<typename Type> class SLLIter;

template<typename Type>
class SLList
{
private:
	friend class SLLIter<Type>;

	struct Node
	{
		Type data;
		Node* next;
	};

	Node* head;
	unsigned int _size;

public:
	SLList();
	~SLList();
	SLList<Type>& operator=(const SLList<Type>& that);
	SLList(const SLList<Type>& that);
	void addHead(const Type& v);
	void clear();
	void insert(SLLIter<Type>& index, const Type& v);
	void remove(SLLIter<Type>& index);
	inline unsigned int size() const{ return _size; }

	void RecursiveHelp(Node* add);
};

template<typename Type>
SLList<Type>::SLList(){
	head = NULL;
	_size = 0;
}

template<typename Type>
SLList<Type>::~SLList(){
	Node* temp;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		delete temp;
	}
}

template<typename Type>
SLList<Type>& SLList<Type>::operator=(const SLList<Type>& that){
	if (this != &that){
		clear();
		RecursiveHelp(that.head);
	}
	return *this;
}

template<typename Type>
void SLList<Type>::RecursiveHelp(Node* add){
	if (add == NULL){
		return;
	}
	else{
		RecursiveHelp(add->next);
		addHead(add->data);
	}
}

template<typename Type>
SLList<Type>::SLList(const SLList<Type>& that){
	head = nullptr;
	*this = that;
}

template<typename Type>
void SLList<Type>::addHead(const Type& v){
	Node* temp = new Node;
	temp->data = v;
	if (head == NULL){
		head = temp;
		head->next = NULL;
	}
	else{
		temp->next = head;
		head = temp;
	}
	_size++;
}

template<typename Type>
void SLList<Type>::clear(){
	Node* temp;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		delete temp;
	}
	head = NULL;
	_size = 0;
}

template<typename Type>
void SLList<Type>::insert(SLLIter<Type>& index, const Type& v){

	if (index.curr != NULL){
		Node* temp = new Node;
		temp->data = v;
		if (index.curr == head){
			temp->next = head;
			head = temp;
			index.curr = head;
		}
		else {
			index.prev->next = temp;
			temp->next = index.curr;
			index.curr = temp;
		}
		_size++;
	}
}

template<typename Type>
void SLList<Type>::remove(SLLIter<Type>& index){
	Node* temp;
	if (head != NULL){
		if (index.curr == head){
			temp = head;
			head = head->next;
			delete temp;
			index.curr = head;
		}
		else {
			temp = index.curr;
			index.prev->next = temp->next;
			index.curr = index.curr->next;
			delete temp;

		}
		_size--;
	}
}



template<typename Type>
class SLLIter
{
	friend class SLList<Type>;
	typename SLList<Type>::Node * curr, *prev;
	SLList<Type>* list;

public:
	SLLIter(SLList<Type>& listToIterate);
	void begin();
	bool end() const;
	SLLIter<Type>& operator++();
	Type& current() const;
};

template<typename Type>
SLLIter<Type>::SLLIter(SLList<Type>& listToIterate){
	list = &listToIterate;
	curr = listToIterate.head;
	prev = NULL;
}

template<typename Type>
void SLLIter<Type>::begin(){
	if (list->head != NULL){
	curr = list->head;
	prev = NULL;
	}
}

template<typename Type>
bool SLLIter<Type>::end() const{
	return (curr != NULL) ? false : true;
}

template<typename Type>
SLLIter<Type>& SLLIter<Type>::operator++(){
	if (curr != NULL){
		prev = curr;
		curr = curr->next;
	}
	return *this;
}

template<typename Type>
Type& SLLIter<Type>::current() const{
	return curr->data;
}