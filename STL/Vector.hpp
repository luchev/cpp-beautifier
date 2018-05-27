#ifndef VECTOR_H
#define VECTOR_H
#include "TFunc.h"
#include <iostream>

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
class Vector {
private:
	size_t size; // Number of items
	size_t realsize; // Actual size of the array
	T* list;
	void grow();
	void shrink();
	void copyList(T* Dest, T* Source, size_t Size);
	size_t calcGrow(size_t Size);
	size_t calcShrink(size_t Size);
	T backup;
public:
	// Constructors
	Vector<T>();
	Vector<T>(const Vector<T>& Vect);
	~Vector<T>();
	Vector<T>& operator=(const Vector<T>& Vect);
	// Other methods
	Vector<T>& Add(const T& Item);
	Vector<T>& Append(Vector<T>& Vect);
	// Add an array
	Vector<T>& Clear();
	size_t Index(const T& Item) const;
	size_t LastIndex(const T& Item) const;
	bool Contains(const T& Item) const;
	Vector<T> Clone() const;
	size_t Size() const;
	size_t RealSize() const;
	Vector<T>& RemoveAt(size_t Index);
	Vector<T>& RemoveAll(T& Item);
	T& operator[](size_t Index);
	Vector<T> Concatinate(Vector<T>& AnotherVector);
	Vector<T> operator+(Vector<T>& AnotherVector);
	// RemoveAll, Remove
	// Sort
	// Reverse
	// Concat
	// Count
	// First
	// Last
	// Prepend
};

template <typename T>
Vector<T>::Vector() : size(0), realsize(0) {
	list = new T[realsize];
}
template<typename T>
Vector<T>::Vector(const Vector<T>& Vect) {
	size = Vect.size;
	realsize = Vect.realsize;
	list = new T[realsize];
	copyList(list, Vect.list, size);
}
template <typename T>
Vector<T>::~Vector() {
	delete[] list;
}
template <typename T>
void Vector<T>::copyList(T* Dest, T* Source, size_t Size) {
	for (size_t i = 0; i < Size; ++i)
		Dest[i] = Source[i];
}
template <typename T>
size_t Vector<T>::calcGrow(size_t Size) {
	size_t i = 1;
	for (; i <= Size; i *= 2);
	return i;
}
template <typename T>
size_t Vector<T>::calcShrink(size_t Size) {
	size_t i = 1;
	for (; i < Size; i *= 2);
	return i;
}
template <typename T>
void Vector<T>::grow() {
	if (realsize > size) // Check if a grow is needed i.e the list is full
		return;
	realsize = calcGrow(size);
	T* newList = new T[realsize];
	copyList(newList, list, size);
	delete[] list;
	list = newList;
}
template <typename T>
void Vector<T>::shrink() {
	size_t newRealSize = calcShrink(size);
	if (realsize < newRealSize) // Check if a shrink is needed i.e the list is taking too much space
		return;
	realsize = newRealSize;
	T* newList = new T[realsize];
	copyList(newList, list, size);
	delete[] list;
	list = newList;
}
template <typename T>
T& Vector<T>::operator[](size_t Index) {
	if (Index < size)
		return list[Index];
	return backup;
}
template<typename T>
Vector<T> Vector<T>::Concatinate(Vector<T>& AnotherVector) {
	Vector<T> newVect;
	newVect.Append(*this);
	newVect.Append(AnotherVector);
	return newVect;
}
template<typename T>
Vector<T> Vector<T>::operator+(Vector<T>& AnotherVector) {
	return Concatinate(AnotherVector);
}
template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& Vect)
{
	if (this != &Vect) {
		size = Vect.size;
		realsize = Vect.realsize;
		delete[] list;
		list = new T[realsize];
		copyList(list, Vect.list, size);
	}
	return *this;
}
template <typename T>
Vector<T>& Vector<T>::Add(const T& Item) {
	grow();
	list[size] = Item;
	++size;
	return *this;
}
template <typename T>
size_t Vector<T>::Size() const {
	return size;
}
template <typename T>
size_t Vector<T>::RealSize() const {
	return realsize;
}
template <typename T>
Vector<T>& Vector<T>::Append(Vector<T>& Vect) {
	for (size_t i = 0; i < Vect.Size(); ++i) {
		Add(Vect[i]);
	}
	return *this;
}
template <typename T>
Vector<T>& Vector<T>::Clear() {
	delete[] list;
	size = 0;
	realsize = 0;
	list = new T[realsize];
	return *this;
}
template <typename T>
size_t Vector<T>::Index(const T& Item) const {
	for (size_t i = 0; i < size; ++i) {
		if (list[i] == Item)
			return i;
	}
	return (size_t)-1;
}
template <typename T>
size_t Vector<T>::LastIndex(const T& Item) const {
	for (size_t i = size; i >= 0; --i) {
		if (list[i] == Item)
			return i;
	}
	return (size_t)-1;
}
template <typename T>
bool Vector<T>::Contains(const T& Item) const {
	return Index(Item) != (size_t)-1;
}
template <typename T>
Vector<T> Vector<T>::Clone() const {
	return Vector<T>(*this);
}
template <typename T>
Vector<T>& Vector<T>::RemoveAt(size_t Index) {
	if (Index < size) {
		for (size_t i = Index; i < size - 1; ++i) {
			list[i] = list[i + 1];
		}
		--size;
		shrink();
	}
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::RemoveAll(T & Item) {
	for (size_t i = 0; i < size; ++i) {
		if (list[i] == Item) {
			RemoveAt(i);
			--i;
		}
	}
	return *this;
}
template <typename T>
std::ostream& operator<<(std::ostream& os, Vector<T> Vect) {
	for (size_t i = 0; i < Vect.Size(); ++i) {
		os << Vect[i] << std::endl;
	}
	return os;
}
#endif
