#ifndef TFUNC_H
#define TFUNC_H

template <typename T>
void swap(T& item1, T& item2) { // Swaps the 2 items in the memory
	T tempItem = item1;
	item1 = item2;
	item2 = tempItem;
}

#endif