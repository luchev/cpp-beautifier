#ifndef STRING_H
#define STRING_H
#include <iostream>
#include "Vector.hpp"
#include "Pair.hpp"

class String {
private:
	size_t size; // The length of the string
	size_t realsize; // The actual length of str, where the string is stored, powers of 2
	char* str; // The array where the string is stored
	void onlySet(const char* Str); // Only set string without deleting (used in copying, operator=, etc.)
	void onlySet(char Char); // Only set string without deleting (used in copying, operator=, etc.)
	void grow(); // Increase the size of the array <str>
	void shrink(); // Decrease the size of the array <str>
	size_t calcSize(size_t StrLen); // Calculates how big str needs to be, Returns the smallest power of 2 which is bigger than StrLen
public:
	// Constructors
	String(); // Default constructor, only initializes the '\0'
	String(char Char); // Constructs a string from a given single character
	String(const char* Str); // Constructs a string from a given char array
	// Copy constructor
	String(const String& Str); // Constructs a string from another string
	// Destructor
	~String(); // Frees the memory taken by <str>
	// Assignment
	String& Set(char Char);// Sets the contents of a string, like what the operator= would do usually
	String& Set(const char* Str);
	String& Set(const String& Str);
	String& SetAt(char Char, size_t Position); // Sets the character at some position if that position is valid
	String& operator=(char Char); // Operator= acting as the copy constructor but deleting the current str first
	String& operator=(const char* Str);
	String& operator=(const String& Str);
	// Append
	String& Append(char Char); // Append to the end of the string
	String& Append(const char* Str);
	String& Append(const String& Str);
	String& Prepend(const String& Str); // Append to the start of the string
	String& operator+=(const String& Str); // Calls Append
	String& InsertAt(size_t Pos, const String& Str); // Inserts the string at the given position
	friend String operator*(String Str, size_t Number); // Makes N copies of the string
	friend String operator*(size_t Number, String Str); // Makes N copies of the string
	// Operators << and >>
	friend std::ostream& operator<<(std::ostream& stream, const String& Str);
	friend std::istream& operator>>(std::istream& stream, String& Str);
	String& ReadStream(std::istream& stream); // Read all the contents of a string until fail() into the string
	// Accessing
	char operator[](const size_t Position) const; // Return a copy of the character at the position
	const char* Get() const; // Return the whole string
	// Comparison
	bool operator<(const String& Str) const; 
	bool operator>(const String& Str) const;
	bool operator<=(const String& Str) const;
	bool operator>=(const String& Str) const;
	bool operator==(const String& Str) const;
	bool operator!=(const String& Str) const;
	// Properties
	size_t Size() const; // The length of the str
	size_t RealSize() const; // The actual space that the object takes in the memory
	size_t Lines() const; // Return the number of lines in the string aka "\n" + 1
	// Concatination
	String Concat(const String& Str) const; // Return a new string which is the concatination of this and Str
	String operator+(const String& Str) const; // Calls Concat
	friend String operator+(const char* Str1, const String& Str2); // Calls Concat
	// Other functionalities
	String Substr(size_t From, size_t To) const; // Return a new string which is a substring of this
	String GetLine(size_t Index) const; // Return a substring from Index until the end of the line
	String Clone() const; // Return a copy of the current object
	String& Clear(); // Set the contentst to be ""
	String& ToLower(); // Convert the English letters in the string to lowercase
	String& ToUpper(); // Convert the English letters in the string to uppercase
	String& TrimStart(); // Remove all spaces at the beginning of the string
	String& TrimEnd(); // Remove all spaces at the end of the string
	String& Trim(); // TrimStart and TrimEnd
	String& RemoveAt(size_t Index); // Remove the character at the Index position from the string
	String& RemoveRange(size_t From, size_t To); // Remove a substring
	String& Replace(const String& What, const String& With, size_t AfterIndex = 0, size_t BeforeIndex = -1); // Replace What with With ONCE
	String& ReplaceAt(const String& What, const String& With, size_t IndexAt); // Replaces What with With at the given Index, used to bypass the Replace which replaces the first instance of What
	String& ReplaceAll(const String& What, const String& With, size_t AfterIndex = 0, size_t BeforeIndex = -1); // Replace What with With until there's no more What in the string. WARNING: Be careful what you replace, could enter infinite loop
	String& ReplaceAllSafe(const String& What, const String& With); // Replace What with With safely, by replacing each found index only once
	String& ReplaceAllSafe(const String& What, const String& With, Vector<size_t>& Indexes); // Replace What with With safely by replacing each found index only once
	String& ReplaceAllOutsidePairs(const String& What, const String& With, Vector<Pair<size_t, size_t>> Pairs); // Replaces all where the index of What is not withing a Pair
	String& ReplaceAllInsidePairs(const String& What, const String& With, Vector<Pair<size_t, size_t>> Pairs); // Replaces all where the index of What is withing a Pair
	String& ReplaceFromUntil(size_t From, const String& Until, const String& With); // Replace everything from From to the end of Str
	String& ReplaceFromTo(const String& From, const String& To, const String& With); // Replace everything from From to the end of To with With
	String& ReplaceFromTo(const Pair<size_t, size_t> Indexes, const String& With); // Replaces everything within the inddexe with With
	String& Remove(const String& What); // Removes substring from the string
	String& RemoveAll(const String& What); // Removes all substrings from the string
	String& Reverse(); // Reverse the string
	Vector<String> Split(const String& Str); // Splits the string into substrings
	String& RemoveEmptyLines(bool NewLines = true, bool Spaces = true, bool Tabs = true, bool TrimStart = true, bool TrimEnd = true); // Removes all new lines which have spaces in them
	// Lookup
	bool Contains(const String& Str) const; // Returns if the string contains Str as a substring
	bool StartsWith(const String& Str) const; // Returns if the string starts with Str
	bool EndsWith(const String& Str) const; // Returns if the string ends with Str
	size_t Index(const String& Str, size_t AfterIndex = 0, size_t BeforeIndex = -1) const; // Returns the first position where Str is encountered
	size_t IndexNotPrecededBy(const String& Str, size_t AfterIndex = 0, char NotPrecededBy = ' ') const; // Returns the first position where Str is encountered and is not preceeded by NotPrecededBy
	size_t IndexNotFollowedBy(const String& Str, size_t AfterIndex = 0, char NotFollowedBy = ' ') const; // Returns the first position where Str is encountered
	size_t LastIndex(const String& Str, size_t BeforeIndex = -1) const; // Returns the last position where Str is encountered
	size_t LastIndexNonEmpty(size_t BeforeIndex = -1) const; // Return the last index which is not \t \n or a space
	Vector<size_t> IndexAll(const String& Str, size_t AfterIndex = 0, size_t BeforeIndex = -1) const; // Returns all the indexes of an occurence of Str in the string, not overlapping
	Vector<size_t> IndexOverlapping(const String& Str, size_t AfterIndex = 0, size_t BeforeIndex = -1) const; // Returns all the indexes of Str occurring in the string overlapping
	Pair<size_t, size_t> IndexPair(const String& From, const String& To, size_t AfterIndex = 0, size_t BeforeIndex = -1) const; // Returns a pair of the starting index where From was found and the ending index of To where To was found after From in the string
	Pair<size_t, size_t> IndexPairAdv(const String& From, const String& To, Vector<char>& CharsBetween, size_t AfterIndex = 0, size_t BeforeIndex = -1) const; // Returns a pair of the starting index where From was found and the ending index of To where To was found after From in the string, the characters between From and To have to be from the Vector CharsBetween
	Pair<size_t, size_t> IndexPairExcludingNested(const String& From, const String& To, size_t AfterIndex = 0, size_t BeforeIndex = -1) const; // Returns a pair of the starting index where From was found and the ending index of To where To was found after From, it returns the outermost To, e.g (()) it will return the outermost brackets because ( is repeated inside, so it will ignore the first )
	Vector<Pair<size_t, size_t>> IndexAllPairs(const String& From, const String& To, size_t AfterIndex = 0, size_t BeforeIndex = -1) const; // Indexes all pairs starting with From, ending with To
	Vector<Pair<size_t, size_t>> IndexAllPairsOutsidePairs(const String& From, const String& To, size_t AfterIndex = 0, size_t BeforeIndex = -1) const; // Indexes all pairs starting with From, ending with To outside of pairs
	Vector<Pair<size_t, size_t>> IndexAllPairsExcludingNested(const String& From, const String& To, size_t AfterIndex = 0, size_t BeforeIndex = -1) const; // Indexes all pairs starting with From, ending with To, using the ExcludingNexted Pair indexing
	// Regex search, match
	// IndexAll
};

size_t StrLen(const char* Str); // Returns the length of a null-terminated string
void StrCopy(char * Dest, const char * Source); // Copies the contents of string Source into string Dest
short StrCmp(const char* Str1, const char* Str2); // Compares 2 strings like the usual strcpy does
bool StrEquals(const char* Str1, const char* Str2, size_t Limit); // Determines if 2 strings are equal, takes the length to which to compare
void RReverse(char* Str, size_t Len); // Recursively reverses a string
bool IsEmptySpace(char Ch); // Returns if it's a \n \t or space
#endif
