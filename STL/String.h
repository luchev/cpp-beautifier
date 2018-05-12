#include <iostream>

#ifndef STRING_H
#define STRING_H

class String {
private:
	size_t len;
	char* str;
	void onlySet(const char* Str);
	void grow();
	void shrink();
public:
	// Constructors
	String();
	String(char Char);
	String(const char* Str);
	// Copy constructor
	String(const String& Str);
	// Destructor
	~String();
	// Assignment
	String& Set(const String& Str);
	String& SetAt(const String& Str, size_t Pos);
	String& operator=(const String& Str);
	// Append
	String& Append(const String &Str);
	String& operator+=(const String& Str);
	String& InsertAt(const String& Str, size_t Pos);
	// Operators << and >>
	friend std::ostream& operator<<(std::ostream& stream, const String& Str);
	friend std::istream& operator>>(std::istream& stream, String& Str);
	// Accessing
	char operator[](const size_t Position) const;
	const char* Get() const;
	// Comparison
	bool operator<(const String& Str) const;
	bool operator>(const String& Str) const;
	bool operator<=(const String& Str) const;
	bool operator>=(const String& Str) const;
	bool operator==(const String& Str) const;
	bool operator!=(const String& Str) const;
	// Properties
	size_t Length() const;
	// Concatination
	String Concat(const String& Str) const;
	String operator+(const String& Str) const;
	// Other functionalities
	String Substr(size_t From, size_t To) const;
	String Clone();
	String& Clear();
	String& ToLower();
	String& ToUpper();
	String& TrimStart();
	String& TrimEnd();
	String& Trim();
	String& Replace(const String& What, const String& With);
	String& ReplaceAll(const String& What, const String& With);
	String& Remove(const String& What);
	String& RemoveAll(const String& What);
	// Split, Explode
	// Lookup
	bool Contains(const String& Str) const;
	bool StartsWith(const String& Str) const;
	bool EndsWith(const String& Str) const;
	long long Index(const String& Str) const;
	long long LastIndex(const String& Str) const;
	size_t Find(const String& Str) const; // Returns how many times the substring has been found
	size_t FindOverlap(const String& Str) const;
	// Regex search, match, findall, findoverlapall
	// IndexAll
};

size_t StrLen(const String& Str);
void StrCopy(const char * Source, char * Destination);
short StrCmp(String& Str1, String& Str2);
#endif

