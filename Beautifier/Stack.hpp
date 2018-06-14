#ifndef STACK_H
#define STACK_H

#include "Utils.h"
#if DEBUG == 1
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif
#endif // DEBUG == 1

template <typename T>
struct StackItem {
	T data;
	StackItem<T> * previous;
};

template <typename T>
class Stack {
private:
	StackItem<T> * top; // Points at the top of the stack
	void empty(); // Truncates the stack
	void copy(const Stack & St); // Copies the items of one stack into the current one appending them at the end
	void Rcopy(StackItem<T>* item); // ONLY TO BE USED FROM INSIDE copy() => Used to copy the items from one stack into another recursively
public:
	Stack();
	~Stack();
	Stack(const Stack<T>& St);
	Stack<T>& operator=(const Stack<T>& St);

	const bool IsEmpty(); // Returns whether the stack has elements or not
	const T Peek(); // View the last element
	void Push(T& Item); // Add an element
	T Pop(); // Get the last element and remove it from the stack
};

template <typename T>
void Stack<T>::empty() {
	StackItem<T> *toDelete;
	while (top != nullptr) {
		toDelete = top;
		top = top->previous;
		delete toDelete;
	}
}
template <typename T>
void Stack<T>::copy(const Stack<T> & St) {
	Rcopy(St.top);
}
template <typename T>
void Stack<T>::Rcopy(StackItem<T>* item) {
	if (item != nullptr) {
		Rcopy(item->previous);
		Push(item->data);
	}
}
template <typename T>
Stack<T>::Stack() : top(nullptr) {
}
template <typename T>
Stack<T>::~Stack() {
	empty();
}
template <typename T>
Stack<T>::Stack(const Stack<T>& St) {
	copy(St);
}
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& St) {
	if (this != &St) {
		empty();
		copy(St);
	}
	return *this;
}
template <typename T>
const bool Stack<T>::IsEmpty() {
	return top == nullptr;
}
template <typename T>
const T Stack<T>::Peek() {
	if (IsEmpty()) {
		return T();
	}
	else {
		return top->data;
	}
}
template <typename T>
void Stack<T>::Push(T& Item) {
	StackItem<T>* newItem = new StackItem<T>;
	newItem->data = Item;
	newItem->previous = top;
	top = newItem;
}
template <typename T>
T Stack<T>::Pop() {
	if (IsEmpty()) {
		return T();
	}
	StackItem<T> *toDelete = top;
	T TToReturn = top->data;
	top = top->previous;
	delete toDelete;
	return TToReturn;
}
#endif