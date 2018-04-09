//
// Created by SubangkarKr on 08-Apr-18.
//


#ifndef SYMBOLTABLE_LINKEDLIST_CPP
#define SYMBOLTABLE_LINKEDLIST_CPP




#include "LinkedList.h"


template<class T>
LinkedList<T>::~LinkedList() {
	Node<T> *current = head;

	while (current) {
		Node<T> *next = current->GetNext();
		delete (current);
		current = next;
	}

	head = tail = NULL;
}

// O(1)
template<class T>
bool LinkedList<T>::PushFront(const T &value) {
	Node<T> *element = new(std::nothrow) Node<T>(value);

	if (element) {
		if (!head) {
			// Special case, list is empty
			head = element;
			tail = element;
		} else {
			element->SetNext(head);
			head = element;
		}
		return true;
	} else {
		return false; // Error allocating memory
	}
}

// O(1)
template<class T>
bool LinkedList<T>::PushBack(const T &value) {
	Node<T> *element = new(std::nothrow) Node<T>(value);

	if (element) {
		if (!head) {
			// Special case, list is empty
			head = element;
			tail = element;
		} else {
			tail->SetNext(element);
			tail = element;
		}
		return true;
	} else {
		return false;
	}
}

// O(1)
template<class T>
bool LinkedList<T>::PopFront(T *value) {
	if (value && head) {
		*value = head->GetValue();

		Node<T> *new_head = head->GetNext();
		delete head;
		head = new_head;

		return false;
	} else
		return true; // Error: nullptr passed as parameter or list empty
}

// O(n)
template<class T>
bool LinkedList<T>::PopBack(T *value) {
	if (value && tail) {
		*value = tail->GetValue();

		// Special case: one element list
		if (head == tail) {
			delete head;
			head = NULL;
			tail = NULL;
		} else {
			Node<T> *new_tail;

			new_tail = head;

			while (new_tail->GetNext() != tail) // Skip elements till the element before tail
				new_tail = new_tail->GetNext();

			new_tail->SetNext(NULL);
			delete tail;
			tail = new_tail;
		}
		return false;
	} else
		return true; // Error: nullptr passed as parameter or list empty
}

// O(n)
template<class T>
void LinkedList<T>::Print() const {
	Node<T> *e;
	for (e = head; e; e = e->GetNext())
		e->Print();
}

// O(n)
template<class T>
int LinkedList<T>::length() const {
	Node<T> *e;
	int count = 0;

	for (e = head; e; e = e->GetNext()) ++count;

	return count;
}

// best O(1), avg O(n), wst O(n)
template<class T>
bool LinkedList<T>::Insert(const T &value, const int &position) {
	if (position < 0)
		return true;

	if (!position) // Special case: position = 0, insert into head
		return PushFront(value);

	Node<T> *element_before = head;

	for (int count = 0; count < position - 1; ++count) {
		element_before = element_before->GetNext();

		if (!element_before)
			return true; // Out of bounds
	}

	Node<T> *new_element = new(std::nothrow) Node<T>(value);
	if (!new_element)
		return true;

	new_element->SetNext(element_before->GetNext());
	element_before->SetNext(new_element);

	if (element_before == tail) // Special case: insert at the end
		tail = new_element;

	return false;
}

template<class T>
bool LinkedList<T>::Delete(const int &position) {
	if (position < 0)
		return true;

	if (!head)
		return true; // LinkedList empty

	Node<T> *target = head;

	if (!position) {// Special case: position = 0, delete head
		head = head->GetNext();
		delete target;

		return false;
	}

	Node<T> *element_before = head;
	for (int count = 0; count < position - 1; ++count) {
		element_before = element_before->GetNext();

		if (!element_before)
			return true; // Out of bounds
	}

	target = element_before->GetNext();

	if (!target)
		return true; // Out of bounds

	element_before->SetNext(target->GetNext());

	if (target == tail)
		tail = element_before;

	delete target;

	return false;
}

template<class T>
int LinkedList<T>::Find(const T &value) const {
	int position = -1;

	Node<T> *e = head;

	for (int count = 0; e; e = e->GetNext(), ++count) {
		if (e->GetValue() == value) {
			position = count;
			break;
		}
	}

	return position;
}

template<class T>
bool LinkedList<T>::Get(const int &position, T *value) {
	if (position < 0 || !value)
		return true; // invalid input

	Node<T> *e = head;

	for (int count = 0; e && count < position; e = e->GetNext(), ++count);

	if (!e)
		return true;

	*value = e->GetValue();

	return false;
}

template<class T>
bool LinkedList<T>::Insert(const T &value) {
	return this->PushBack(value);
}

//template<class T>
//bool LinkedList<T>::Delete(const T &value) {
//    return Delete(Find(value));
//}

template<class T>
bool LinkedList<T>::remove(const T &value) {
	return Delete(Find(value));
}

template<class T>
bool LinkedList<T>::insert(const T &value) {

	if (search(value)) return false;

	return this->Insert(value);
}

template<class T>
bool LinkedList<T>::search(const T &value) const {
//    return Find(value)!=-1;

	for (Node<T> *e = head; e; e = e->GetNext()) {
		if (e->GetValue() == value) {
			return true;
		}
	}

	return false;
}

template<class T>
std::vector<T> LinkedList<T>::get() const {

	std::vector<T> vc;

	for (Node<T> *e = head; e; e = e->GetNext()) {
		vc.push_back(e->GetValue());
	}

	return vc;
}

template<class T>
T *LinkedList<T>::getData(const T &value) {

	for (Node<T> *e = head; e; e = e->GetNext()) {
		if (e->GetValue() == value)
			return e->getData();
	}
	return NULL;
}

//template<class T>
//int LinkedList<T>::Find(const T &value) const {
//    int position = -1;
//
//    Node<T>* e = head;
//    int c=0;
//    for(e=head;e->GetNext();c++){
//        if(e->GetValue()==value)
//        {
//            return c;
//        }
//    }
//
//    return position;
//}






#endif //SYMBOLTABLE_LINKEDLIST_CPP

