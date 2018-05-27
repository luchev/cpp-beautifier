#ifndef LIST_H
#define LIST_H
#include <iostream>

template <class T>
class List {
private:
	size_t numberOfItems;	// The number of the items in the list
	size_t size;			// The size of the list, 2^n where n is a positive integer
	T *items;				// Array of all the items in the list
							// Most methods modifying *this return *this in order to be able to chain them
	T tmp;					// Used as a backup to return in [] if no item is found instead of throwin errors
	bool IsFull() {
		return numberOfItems == size;
	}

	void growList() { // There are no safety checks, if you wish to grow the list check IsFull() to see if expansion is really necessary
					  // Double the size and copy the old items in the new array
		size *= 2;
		T* newItems = new T[size];
		for (size_t i = 0; i < numberOfItems; ++i) {
			newItems[i] = items[i];
		}
		// Delete the old memory and direct the pointer to the new array
		delete[] items;
		items = newItems;
	}

	void shrinkList() { // The safety checks are in the function
						// Check if it's possible to shrink
		if (size / 2 < numberOfItems || size == 1) {
			//std::cout << "Failed to shrink the list, there's not enough space for all the items.\n";
			return;
		}
		// Calculate the size that the list needs to have
		for (size = 1; size < numberOfItems; size *= 2);
		// Shrink the list copying the items in a new list
		T *newItems = new T[size];
		for (size_t i = 0; i < numberOfItems; ++i) {
			newItems[i] = items[i];
		}
		// Delete the old memory and assign the pointer to the new smaller array
		delete[] items;
		items = newItems;
	}

	void copyAnotherListInThis(const List<T>& AnotherList) { // Copies the items from a given list to this list from the start overwriting the old data 
															 //(WITHOUT deleting the pointer) Always call delete before this if necessary
		size = AnotherList.size;
		numberOfItems = AnotherList.numberOfItems;
		items = new T[size];
		for (size_t i = 0; i < numberOfItems; ++i) {
			items[i] = AnotherList.items[i];
		}
	}

public:
	// Constructors
	List() : size(1), numberOfItems(0), items(new T[size]) { } // Constructor

	~List() { // Destructor
		delete[] items;
	}

	List(const List<T>& AnotherList) { // Copy constructor
		copyAnotherListInThis(AnotherList);
	}

	// Operators
	List<T>& operator=(const List& AnotherList) {
		if (AnotherList != this) {
			delete[] items;
			copyAnotherListInThis(AnotherList);
		}
		return *this;
	}

	T& operator[](long long Position) {
		if (Position < numberOfItems && Position >= 0) {
			return items[Position];
		}
		return tmp;
	}

	List<T>& operator+=(const T& item) {
		Add(item);
		return *this;
	}

	List<T>& Print() {
		for (size_t i = 0; i < numberOfItems; ++i) {
			std::cout << items[i] << "\n";
		}
		return *this;
	}

	// Methods
	List<T>& Add(const T& item) { // Add an item to the end
								  // Check if there's enough space, if not - expand the size of the array
		if (IsFull()) {
			growList();
		}
		// Add the item
		items[numberOfItems] = item;
		numberOfItems++;
		return *this;
	}

	T Get(long long Index) const {
		if (Index >= 0 && Index < numberOfItems) {
			return items[Index];
		}
		return NULL;
	}

	List<T>& InsertAt(const T& item, long long Index) {		// Add the item in position
															// Check if the index is valid to add, if it's < 0 or it's bigger than the amount of items (maybe use Add?) do nothing
		if (Index < 0 || Index > numberOfItems) {
			return *this;
		}
		// Check if there's enough space, if not - expand the size of the array
		if (IsFull()) {
			growList();
		}
		// Shift the items right in the array to free one position for the new item to be added
		for (size_t i = numberOfItems; i >= Index; --i) {
			//std::cout << items[i];
			items[i] = items[i - 1];
		}
		// Add the item in the freed up position
		items[Index] = item;
		numberOfItems++;
		return *this;
	}

	const size_t Size() const { // Return the number of items
		return numberOfItems;
	}

	const size_t ActualSize() const { // Returns the amount of memory that is reserved for *items, used only for debugging
		return size;
	}

	List<T>& RemoveAt(long long Index) { // Remove the item at position Position
										 // Check if such index is valid
		if (numberOfItems <= Index || Index < 0) {
			//std::cout << "Failed to remove at index " << Index << ", no such index exists in the list.\n";
			return *this;
		}
		// Shift all the items after the index left by one
		numberOfItems--;
		for (size_t i = Index; i < numberOfItems; ++i) {
			items[i] = items[i + 1];
		}
		shrinkList();
		return *this;
	}

	List<T>& RemoveRange(long long From, long long To) { // Remove a number of items between two positions including the positions
														 // Swap the 2 if one is less than the other
		if (To < From) {
			long long temp = From;
			From = To;
			To = temp;
		}
		// Security check if such indexes exist
		if (From < 0 || To < 0 || From >= numberOfItems || To >= numberOfItems) {
			//std::cout << "Failed to remove from index " << From << " to " << To << ", no such index exists in the list.\n";
			return *this;
		}
		// Shift the items left as many as needed
		size_t shift = To - From + 1;
		numberOfItems -= shift;
		for (size_t i = From; i < numberOfItems; ++i) {
			items[i] = items[i + shift];
		}
		shrinkList();
		return *this;
	}

	List<T>& Delete(const T& Item) { // Remove one item that is == Item
		for (size_t i = 0; i < numberOfItems; ++i) {
			if (items[i] == Item) {
				RemoveAt(i);
				break;
			}
		}
		return *this;
	}

	List<T>& DeleteAll(const T& Item) { // Remove all items that are == Item
		for (size_t i = 0; i < numberOfItems; ++i) {
			if (items[i] == Item) {
				RemoveAt(i);
				i--;
			}
		}
		return *this;
	}

	List<T>& Delete(bool(*RemoveIfTrue)(const T& Item)) { // Delete 1 item where the cheker function returns true
		for (size_t i = 0; i < numberOfItems; ++i) {
			if (RemoveIfTrue(items[i])) {
				RemoveAt(i);
				break;
			}
		}
		return *this;
	}

	List<T>& DeleteAll(bool(*RemoveIfTrue)(const T& Item)) { // Delete all items where the cheker function returns true
		for (size_t i = 0; i < numberOfItems; ++i) {
			if (RemoveIfTrue(items[i])) {
				RemoveAt(i);
				i--;
			}
		}
		return *this;
	}

	long long Index(const T& Item) { // Return the index of an item that is == Item
		for (size_t i = 0; i < numberOfItems; ++i) {
			if (items[i] == Item) {
				return i;
			}
		}
		return -1;
	}

	long long Index(bool(*IndexIfTrue)(const T& Item)) { // Return the index of an item that the checker function returns to be true
		for (size_t i = 0; i < numberOfItems; ++i) {
			if (IndexIfTrue(items[i])) {
				return i;
			}
		}
		return -1;
	}

	List<T> Find(bool(*ReturnIfTrue)(const T& Item)) {	// Return a list containing 1 item if such is found or empty list if no item is found
														// Because something has to be returned, better return an empty list
		List<T> newList;
		for (size_t i = 0; i < numberOfItems; ++i) {
			if (ReturnIfTrue(items[i])) {
				newList.Add(items[i]);
				break;
			}
		}
		return newList;
	}

	List<T> FindAll(bool(*ReturnIfTrue)(const T& Item)) { // Return a list containing all item matching the criteria
		List<T> newList;
		for (size_t i = 0; i < numberOfItems; ++i) {
			if (ReturnIfTrue(items[i])) {
				newList.Add(items[i]);
			}
		}
		return newList;
	}

	List<T>& Append(const List<T>& AnotherList) { // Append another list to the end of the current one
		for (size_t i = 0; i < AnotherList.numberOfItems; ++i) {
			Add(AnotherList[i]);
		}
		return *this;
	}

	List<T> Concatinate(const List<T>& AnotherList) const { // Return a new list the concatination of this and AnotherList 
		List<T> newList;
		newList.Append(*this);
		newList.Append(AnotherList);
		return newList;
	}

};

#endif
