#include <iostream>

#ifndef STRING_H
#define STRING_H

class String {
private:
	size_t size;
	char * arr;
	const size_t StringLength(const char *Str) const; // Return the lenght of a string, 0 if the size is or on error
	void StringCopy(const char * Source, char * Destination); // Copy a char array assuming the destination has enough space
	void setWithoutDelete(const char* Str);
	void setWithoutDelete(const String& Str);
public:
	String(); // Constructor
	String(const char* Str); // Copy constructor
	String(const String& Str); // Copy constructor
	~String(); // Destructor
	String& operator=(const String& Str); // operator=
	String& operator=(const char *Str); // operator=
	String& operator+=(const char); // Append a character to the end of the current object
	String& operator+=(const char *Str); // Append a string to  the end of current object
	String& operator+=(const String& Str); // Append a string to  the end of current object
	friend std::ostream &operator<<(std::ostream &stream, const String &This); // Overwrite the << operator to work correctly
	friend std::istream &operator>>(std::istream &stream, String &Str); // Overwrite the >> operator to work correctly
	const char operator[](const size_t Position) const; // Return the character on the given position
	bool operator==(const char*) const; // Compare 2 strings
	bool operator==(const String& Str) const; // Compare 2 strings
	bool operator!=(const char*) const; // Compare 2 strings
	bool operator!=(const String& Str) const; // Compare 2 strings
	const char * Get() const; // Return pointer to the array where the string is
	void Set(const char *Str); // Set the string to equal a char array
	void Set(const String& Str); // Set the string to equal another string
	size_t Length() const; // Return the length of the string
	String SubString(size_t From, size_t To) const; // Return a string containing a substring between the 2 indexes
	void Append(const String &Str); // Append a string to the end of this
	void Append(const char* Str); // Append a string to the end of this
	void Append(const char Char); // Append a string to the end of this
	String Concatinate(const String &Str) const; // Concatinate this and the new string and return a new string object containing the concatination
	void Clear(); // Empties the string
	void ToLower();
	void ToUpper();
};

#endif

