#include "String.h"

const size_t String::StringLength(const char *Str) const {
	if (Str != nullptr) {
		size_t i = 0;
		while (Str[i]) {
			i++;
		}
		return i;
	}
	return 0;
}

void String::StringCopy(const char *Source, char *Destination) {
	if (Source == nullptr || Destination == nullptr) {
		return;
	}
	size_t i = 0;
	while (Source[i] != '\0') {
		Destination[i] = Source[i];
		i++;
	}
	Destination[i] = '\0';
}

void String::setWithoutDelete(const char* Str) {
	size = StringLength(Str);
	arr = new char[size + 1];
	StringCopy(Str, arr);
}

void String::setWithoutDelete(const String& Str) {
	size = Str.size;
	arr = new char[size + 1];
	StringCopy(Str.arr, arr);
}

String::String() {
	setWithoutDelete("");
}

String::String(const char* Str) {
	if (Str == nullptr) {
		setWithoutDelete("");
	}
	else {
		setWithoutDelete(Str);
	}
}

String::~String() {
	delete[] arr;
}

String::String(const String &Str) {
	setWithoutDelete(Str);
}

String &String::operator=(const String &Str) {
	if (this != &Str) {
		Set(Str);
	}
	return *this;
}

String &String::operator=(const char *Str) {
	if (Str != nullptr) {
		Set(Str);
	}
	return *this;
}

void String::Set(const char *Str) {
	if (Str != nullptr) {
		delete[] arr;
		setWithoutDelete(Str);
	}
}

void String::Set(const String& Str) {
	delete[] arr;
	setWithoutDelete(Str);
}

String &String::operator+=(const char Char) {
	Append(Char);
	return *this;
}

String &String::operator+=(const char *Str) {
	Append(Str);
	return *this;
}

String& String::operator+=(const String& Str) {
	Append(Str);
	return *this;
}

const char *String::Get() const {
	return arr;
}

size_t String::Length() const {
	return size;
}

std::ostream & operator<<(std::ostream &stream, const String &Str) {
	stream << Str.arr;
	return stream;
}
const char String::operator[](const size_t Position) const {
	if (Position >= size)
		return 0;
	return arr[Position];
}

String String::SubString(size_t From, size_t To) const {
	if (From < 0 || To >= size || From > To) {
		return String();
	}

	char * newStr = new char[To - From + 2];
	for (size_t i = 0; i < To - From + 1; i++) {
		newStr[i] = arr[i + From];
	}
	newStr[To - From + 1] = '\0';
	String newString = newStr;
	return newString;
}

void String::Append(const String &Str) {
	*this += Str.Get();
}

void String::Append(const char* Str) {
	if (Str != nullptr) {
		size_t StrLen = StringLength(Str);
		char *newString = new char[size + StrLen + 1];
		StringCopy(arr, newString);
		StringCopy(Str, newString + size);
		newString[size + StrLen] = '\0';
		Set(newString);
	}
}

void String::Append(const char Char) {
	char *newString = new char[size + 2];
	StringCopy(arr, newString);
	newString[size] = Char;
	newString[size + 1] = '\0';
	Set(newString);
}

String String::Concatinate(const String &Str) const {
	String newString = *this;
	newString.Append(Str);
	return newString;
}

std::istream &operator>>(std::istream &stream, String &Str) {
	char n = '\0';
	Str.Clear();
	stream.get(n);
	while (n != '\n') {
		Str += n;
		stream.get(n);
	}
	return stream;
}

void String::Clear() {
	delete[] arr;
	setWithoutDelete("");
}

bool String::operator==(const String& Str) const {
	size_t len = Length();
	if (len != Str.Length()) {
		return false;
	}
	for (size_t i = 0; i < len; i++) {
		if (arr[i] != Str[i])
			return false;
	}
	return true;
}

bool String::operator==(const char* Str) const {
	String newStr = Str;
	return *this == newStr;
}

bool String::operator!=(const String& Str) const {
	size_t len = Length();
	if (len != Str.Length()) {
		return true;
	}
	for (size_t i = 0; i < len; i++) {
		if (arr[i] != Str[i])
			return true;
	}
	return false;
}

bool String::operator!=(const char* Str) const {
	String newStr = Str;
	return *this != newStr;
}

void String::ToUpper() {
	for (size_t i = 0; i < size; ++i) {
		if (arr[i] >= 'a' && arr[i] <= 'z') {
			arr[i] = arr[i] & ~32;
		}
	}
}

void String::ToLower() {
	for (size_t i = 0; i < size; ++i) {
		if (arr[i] >= 'A' && arr[i] <= 'Z') {
			arr[i] = arr[i] | 32;
		}
	}
}