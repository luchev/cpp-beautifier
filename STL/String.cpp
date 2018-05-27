#include "String.h"
#include "TFunc.h"

#include "Utils.h"
#if DEBUG == 1
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif
#endif // DEBUG == 1

size_t StrLen(const char* Str) {
	size_t i = 0;
	for (; Str[i] != 0; ++i);
	return i;
}
void StrCopy(char * Dest, const char * Source) {
	size_t i = 0;
	for (; Source[i] != 0; ++i)
		Dest[i] = Source[i];
	Dest[i] = 0;
}
short StrCmp(const char* Str1, const char* Str2) {
	size_t i = 0;
	for (; Str1[i] != 0 && Str2[i] != 0; ++i) {
		if (Str1[i] < Str2[i])
			return -1;
		else if (Str1[i] > Str2[i])
			return 1;
	}
	return Str1[i] != Str2[i];
}
bool StrEquals(const char* Str1, const char* Str2, size_t Limit) {
	for (size_t i = 0; i < Limit; ++i)
		if (Str1[i] != Str2[i])
			return 0;
		else if (Str1[i] == 0 || Str2[i] == 0)
			return Str1[i] == Str2[i];
	return 1;
}
void RReverse(char* Str, size_t Len) {
	if (Len < 2)
		return;
	swap(Str[0], Str[Len - 1]);
	RReverse(Str + 1, Len - 2);
}

bool IsEmptySpace(char Ch) {
	return Ch == ' ' || Ch == '\n' || Ch == '\t';
}

size_t String::calcSize(size_t StrLen) {
	size_t i = 1;
	for (; i <= StrLen; i *= 2);
	return i;
}
void String::onlySet(const char* Str) {
	size = StrLen(Str);
	realsize = calcSize(size);
	str = new char[realsize];
	StrCopy(str, Str);
}
void String::onlySet(char Char) {
	size = 1;
	realsize = 2;
	str = new char[realsize];
	str[0] = Char;
	str[1] = 0;
}
void String::grow() {
	if (realsize > size + 1) // Check if a grow is needed i.e the string is full
		return;
	realsize *= 2;
	char* newStr = new char[realsize];
	StrCopy(newStr, str);
	delete[] str;
	str = newStr;
}
void String::shrink() {
	size_t newLen = calcSize(size);
	if (realsize <= newLen) // Check if a shrink is needed i.e the string is taking too much space
		return;
	realsize = newLen;
	char* newStr = new char[realsize];
	StrCopy(newStr, str);
	delete[] str;
	str = newStr;
}

String::String() {
	onlySet("");
}
String::String(char Char) {
	onlySet(Char);
}
String::String(const char* Str) {
	onlySet(Str);
}
String::String(const String& Str) {
	onlySet(Str.str);
}
String::~String() {
	delete[] str;
}
String& String::Set(char Char) {
	delete[] str;
	onlySet(Char);
	return *this;
}
String& String::Set(const char* Str) {
	delete[] str;
	onlySet(Str);
	return *this;
}
String& String::Set(const String& Str) {
	if (&Str != this) {
		Set(Str.str);
	}
	return *this;
}
String& String::SetAt(char Char, size_t Position) {
	if (Position < size)
		str[Position] = Char;
	return *this;
}
String& String::operator=(char Char) {
	Set(Char);
	return *this;
}
String& String::operator=(const char* Str) {
	Set(Str);
	return *this;
}
String& String::operator=(const String& Str) {
	Set(Str);
	return *this;
}
String& String::Append(char Char) {
	grow();
	str[size] = Char;
	size++;
	str[size] = 0;
	return *this;
}
String& String::Append(const char* Str) {
	size_t length = StrLen(Str);
	for (size_t i = 0; i < length; ++i) {
		Append(Str[i]);
	}
	return *this;
}
String& String::Append(const String& Str) {
	const String& ref = (&Str == this) ? Clone() : Str;
	for (size_t i = 0; i < ref.size; ++i) {
		Append(Str.str[i]);
	}
	return *this;
}
String & String::Prepend(String Str)
{
	return InsertAt(0, Str);
}
String& String::operator+=(const String& Str) {
	return Append(Str);
}
String& String::InsertAt(size_t Pos, const String& Str) {
	if (Pos > size) // If it's bigger return
		return *this;
	size_t newLen = size + Str.size;
	size_t newSize = calcSize(newLen);
	char* newStr = new char[newSize];
	size_t curPos = 0;
	for (; curPos < Pos; ++curPos) // Copy the beginning
		newStr[curPos] = str[curPos];
	for (size_t i = 0; i < Str.size; ++i, ++curPos) // Copy the addition
		newStr[curPos] = Str[i];
	for (size_t i = Pos; str[i] != 0; ++i, ++curPos) // Copy the remainder after the replaces sub string
		newStr[curPos] = str[i];
	newStr[curPos] = 0;
	size = newLen;
	realsize = newSize;
	delete[] str;
	str = newStr;
	return *this;
}
String operator*(String Str, size_t Number) {
	String tmp("");
	for (size_t i = 0; i < Number; ++i) {
		tmp.Append(Str.str);
	}
	return tmp;
}
String operator*(size_t Number, String Str) {
	return operator*(Str, Number);
}
std::ostream& operator<<(std::ostream& stream, const String& Str) {
	stream << Str.str;
	return stream;
}
std::istream& operator>>(std::istream& stream, String& Str) {
	Str.Clear();
	char ch = '\n';
	while (stream.get(ch) && ch != '\n')
		Str.Append(ch);
	return stream;
}
String operator+(const char * Str1, const String & Str2) {
	String tmp(Str1);
	tmp.Append(Str2);
	return tmp;
}
String & String::ReadStream(std::istream & stream)
{
	Clear();
	char ch = 0;
	while (stream.get(ch))
		Append(ch);
	return *this;
}
char String::operator[](const size_t Position) const {
	if (Position < size)
		return str[Position];
	return 0;
}
const char* String::Get() const {
	return str;
}
bool String::operator<(const String& Str) const {
	return StrCmp(str, Str.str) < 0;
}
bool String::operator>(const String& Str) const {
	return StrCmp(str, Str.str) > 0;
}
bool String::operator<=(const String& Str) const {
	return StrCmp(str, Str.str) <= 0;
}
bool String::operator>=(const String& Str) const {
	return StrCmp(str, Str.str) >= 0;
}
bool String::operator==(const String& Str) const {
	return StrCmp(str, Str.str) == 0;
}
bool String::operator!=(const String& Str) const {
	return !operator==(Str);
}
size_t String::Size() const {
	return size;
}
size_t String::RealSize() const {
	return realsize;
}
size_t String::Lines() const
{
	return IndexAll("\n").Size() + 1;
}
String String::Concat(const String& Str) const {
	String newStr = *this;
	newStr.Append(Str);
	return newStr;
}
String String::operator+(const String& Str) const {
	return Concat(Str);
}
String String::Substr(size_t From, size_t To) const {
	if (To < From)
		swap(To, From);
	if (To >= size)
		To = size;
	String newStr;
	for (; From < To && From < size; ++From)
		newStr.Append(str[From]);
	return newStr;
}
String String::GetLine(size_t Index) const {
	if (Index > size)
		return String();
	String newStr;
	for (; str[Index] != '\n' && str[Index] != 0; ++Index)
		newStr.Append(str[Index]);
	return newStr;
}
String String::Clone() const {
	return String(*this);
}
String& String::Clear() {
	delete[] str;
	onlySet("");
	return *this;
}
String& String::ToLower() {
	for (size_t i = 0; i < size; ++i)
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 'a' - 'A';
	return *this;
}
String& String::ToUpper() {
	for (size_t i = 0; i < size; ++i)
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 'a' - 'A';
	return *this;

}
String& String::TrimStart() {
	size_t removeTill = 0;
	for (size_t i = 0; i < size; ++i) {
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			++removeTill;
		else
			break;
	}
	RemoveRange(0, removeTill);
	return *this;
}
String& String::TrimEnd() {
	size_t removeFrom = size;
	for (size_t i = size - 1; i >= 0; --i) {
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
			removeFrom = i;
		}
		else
			break;
	}
	RemoveRange(removeFrom, size);
	return *this;
}
String& String::Trim() {
	TrimStart();
	TrimEnd();
	return *this;
}
String& String::RemoveAt(size_t Index) {
	if (Index < size) {
		size_t newLen = size - 1;
		size_t newSize = calcSize(newLen);
		char* newStr = new char[newSize];
		for (size_t i = 0; i < Index; ++i) {
			newStr[i] = str[i];
		}
		StrCopy(newStr + Index, str + Index + 1);
		delete[] str;
		str = newStr;
		size = newLen;
		realsize = newSize;
		shrink();
	}
	return *this;
}
String& String::RemoveRange(size_t From, size_t To) {
	if (To < From)
		swap(To, From);
	if (From >= size)
		return *this;
	if (To > size)
		To = size;
	size_t newSize = size - (To - From);
	size_t newRealSize = calcSize(newSize + 1);
	char* newStr = new char[newRealSize];
	size_t curPos = 0;
	for (; curPos < From; ++curPos) // Copy the beginning
		newStr[curPos] = str[curPos];
	for (; To < size; ++To, ++curPos) // Copy the ending
		newStr[curPos] = str[To];
	newStr[curPos] = 0;
	delete[] str;
	str = newStr;
	size = newSize;
	realsize = newRealSize;
	return *this;
}
String& String::Replace(const String& What, const String& With, size_t AfterIndex, size_t BeforeIndex) {
	size_t index = Index(What, AfterIndex, BeforeIndex);
	if (index != (size_t)-1) {
		size_t newLen = size - What.size + With.size;
		size_t newSize = calcSize(newLen + 1);
		char* newStr = new char[newSize];
		size_t curPos = 0;
		for (; curPos < index; ++curPos) // Copy the beginning
			newStr[curPos] = str[curPos];
		for (size_t i = 0; i < With.size; ++i, ++curPos) // Copy the replacement
			newStr[curPos] = With[i];
		for (size_t i = index + What.size; str[i] != 0; ++i, ++curPos) // Copy the remainder after the replaces sub string
			newStr[curPos] = str[i];
		newStr[curPos] = 0;
		delete[] str;
		str = newStr;
		size = newLen;
		realsize = newSize;
	}
	return *this;
}
String& String::ReplaceAt(const String & What, const String & With, size_t IndexAt)
{
	if (IndexAt != (size_t)-1) {
		size_t newLen = size - What.size + With.size;
		size_t newSize = calcSize(newLen + 1);
		char* newStr = new char[newSize];
		size_t curPos = 0;
		for (; curPos < IndexAt; ++curPos) // Copy the beginning
			newStr[curPos] = str[curPos];
		for (size_t i = 0; i < With.size; ++i, ++curPos) // Copy the replacement
			newStr[curPos] = With[i];
		for (size_t i = IndexAt + What.size; str[i] != 0; ++i, ++curPos) // Copy the remainder after the replaces sub string
			newStr[curPos] = str[i];
		newStr[curPos] = 0;
		delete[] str;
		str = newStr;
		size = newLen;
		realsize = newSize;
	}
	return *this;
}
String& String::ReplaceAll(const String& What, const String& With, size_t AfterIndex, size_t BeforeIndex) {
	size_t index = Index(What, AfterIndex, BeforeIndex);
	while (index != (size_t)-1) {
		ReplaceAt(What, With, index);
		index = Index(What, AfterIndex, BeforeIndex);
	}
	return *this;
}
String& String::ReplaceAllSafe(const String& What, const String& With) {
	Vector<size_t> indexes = IndexAll(What);
	return ReplaceAllSafe(What, With, indexes);
}
String& String::ReplaceAllSafe(const String & What, const String & With, Vector<size_t>& Indexes) {
	if (Indexes.Size() == 0)
		return *this;
	size_t newSize = size + Indexes.Size() * With.Size() - Indexes.Size() * What.Size();
	size_t newRealSize = calcSize(newSize);
	char* newStr = new char[newRealSize];
	size_t replaceIndex = 0;
	size_t newStrIndex = 0;
	for (size_t i = 0; i < size; i++) {
		if (i == Indexes[replaceIndex]) {
			for (size_t j = 0; j < With.Size(); j++) {
				newStr[newStrIndex + j] = With[j];
			}
			newStrIndex += With.Size();
			i += What.Size() - 1;
			if (replaceIndex < Indexes.Size())
				replaceIndex++;
		}
		else {
			newStr[newStrIndex] = str[i];
			newStrIndex++;
		}
	}
	size = newSize;
	realsize = newRealSize;
	newStr[newSize] = 0;
	delete[] str;
	str = newStr;
	return *this;
}
String& String::ReplaceAllOutsidePairs(const String & What, const String With, Vector<Pair<size_t, size_t>>& Pairs) {
	if (Pairs.Size() == 0)
		return *this;
	Vector<size_t> indexes = IndexAll(What);
	size_t j = 0;
	bool shouldRemove = false;
	for (size_t i = 0; i < indexes.Size(); ++i) {
		shouldRemove = false;
		for (size_t j = 0; j < Pairs.Size(); ++j) {
			if (Pairs[j].Key() < indexes[i] && indexes[i] < Pairs[j].Value()) {
				shouldRemove = true;
				break;
			}
		}
		if (shouldRemove) {
			indexes.RemoveAt(i);
			--i;
		}
	}
	return ReplaceAllSafe(What, With, indexes);
}
String & String::ReplaceAllInsidePairs(const String & What, const String With, Vector<Pair<size_t, size_t>>& Pairs) {
	if (Pairs.Size() == 0)
		return *this;
	Vector<size_t> indexes = IndexAll(What);
	bool shouldRemove = true;
	for (size_t i = 0; i < indexes.Size(); ++i) {
		shouldRemove = true;
		for (size_t j = 0; j < Pairs.Size(); ++j) {
			if (Pairs[j].Key() <= indexes[i] && indexes[i] <= Pairs[j].Value()) {
				shouldRemove = false;
				break;
			}
		}
		if (shouldRemove) {
			indexes.RemoveAt(i);
			--i;
		}
	}
	return ReplaceAllSafe(What, With, indexes);
}
String& String::Remove(const String& What) {
	return Replace(What, "");
}
String & String::ReplaceFromUntil(size_t From, const String & To, const String& With) {
	size_t nextIndex = Index(To);
	RemoveRange(From, nextIndex);
	InsertAt(From, With);
	return *this;
}
String & String::ReplaceFromTo(const String& From, const String& To, const String& With) {
	Pair<size_t, size_t> pair = IndexPair(From, To);
	ReplaceFromTo(pair, With);
	return *this;
}
String & String::ReplaceFromTo(Pair<size_t, size_t> Indexes, const String & With) {
	RemoveRange(Indexes.Key(), Indexes.Value());
	InsertAt(Indexes.Key(), With);
	return *this;
}
String& String::RemoveAll(const String& What) {
	return ReplaceAll(What, "");
}
String& String::Reverse() {
	RReverse(str, size);
	return *this;
}
Vector<String> String::Split(const String & Str) {
	Vector<String> strings;
	Vector<size_t> indexes = IndexAll(Str);
	if (indexes.Size() == 0) {
		strings.Add(*this);
		return strings;
	}
	String buffer;
	size_t currentIndex = 0;
	for (size_t i = 0; i < indexes.Size(); ++i) {
		buffer = Substr(currentIndex, indexes[i]).RemoveAll(Str);
		if (buffer != "") {
			strings.Add(buffer);
			currentIndex = indexes[i] + Str.size;
		}
	}
	buffer = Substr(currentIndex, size).RemoveAll(Str);
	if (buffer != "")
		strings.Add(buffer);
	return strings;
}
String& String::RemoveEmptyLines(bool NewLines, bool Spaces, bool Tabs, bool Trimstart, bool Trimend) {
	Vector<char> spaces;
	if (NewLines)
		spaces.Add('\n');
	if (Spaces)
		spaces.Add(' ');
	if (Tabs)
		spaces.Add('\t');
	Pair<size_t, size_t> pair(0, 0);
	while (true) {
		pair = IndexPairAdv("\n", "\n", spaces);
		if (pair.Key() == (size_t)-1 || pair.Value() == (size_t)-1)
			break;
		Replace(Substr(pair.Key(), pair.Value()), "\n");
	}
	if (Trimstart)
		TrimStart();
	if (Trimend)
		TrimEnd();
	return *this;
}
bool String::Contains(const String& Str) const {
	return Index(Str) != (size_t)-1;
}
bool String::StartsWith(const String& Str) const {
	return StrEquals(str, Str.str, Str.size);
}
bool String::EndsWith(const String& Str) const {
	if (Str.size > size)
		return 0;
	size_t start = size - Str.size;
	return StrEquals(str + start, Str.str, Str.size);
}
size_t String::Index(const String& Str, size_t AfterIndex, size_t BeforeIndex) const {
	if (Str.size == 0)
		return -1;
	size_t Cap = size - Str.size + 1 > BeforeIndex ? BeforeIndex : size - Str.size + 1;
	for (size_t i = AfterIndex; i < Cap; ++i) {
		if (StrEquals(str + i, Str.str, Str.size))
			return i;
	}
	return -1;
}
size_t String::IndexNotPrecededBy(const String & Str, size_t AfterIndex, char NotPrecededBy) const {
	size_t tmpIndex = Index(Str, AfterIndex);
	while (tmpIndex != (size_t)-1) {
		if (tmpIndex > 0 && str[tmpIndex - 1] != NotPrecededBy)
			break;
		tmpIndex = Index(Str, tmpIndex + Str.size);
	}
	return tmpIndex;
}
size_t String::IndexNotFollowedBy(const String & Str, size_t AfterIndex, char NotFollowedBy) const {
	size_t tmpIndex = Index(Str, AfterIndex);
	while (tmpIndex != (size_t)-1) {
		if (tmpIndex > 0 && str[tmpIndex + Str.size + 1] != NotFollowedBy)
			break;
		tmpIndex = Index(Str, tmpIndex + Str.size);
	}
	return tmpIndex;
}
size_t String::LastIndex(const String& Str, size_t BeforeIndex) const {
	size_t Cap = size - Str.size - 1;
	if (Cap > BeforeIndex)
		Cap = BeforeIndex;
	for (size_t i = Cap; i >= 0; --i) {
		if (StrEquals(str + i, Str.str, Str.size))
			return i;
	}
	return -1;
}
size_t String::LastIndexNonEmpty(size_t BeforeIndex) const {
	size_t Cap = size - 1;
	if (Cap > BeforeIndex)
		Cap = BeforeIndex;
	for (size_t i = Cap; i >= 0; --i) {
		if (!IsEmptySpace(str[i]))
			return i;
	}
	return -1;
}
Vector<size_t> String::IndexAll(const String& Str, size_t AfterIndex, size_t BeforeIndex) const {
	Vector<size_t> indexes;
	size_t index = 0;
	while (true) {
		index = Index(Str, AfterIndex);
		if (index == (size_t)-1 || index > BeforeIndex)
			break;
		AfterIndex = index + Str.size;
		indexes.Add(index);
	}
	return indexes;
}
Vector<size_t> String::IndexOverlapping(const String & Str, size_t AfterIndex, size_t BeforeIndex) const {
	Vector<size_t> indexes;
	size_t index = 0;
	while (true) {
		index = Index(Str, AfterIndex);
		if (index == (size_t)-1 || index > BeforeIndex)
			break;
		++AfterIndex;
		indexes.Add(index);
	}
	return indexes;
}
Pair<size_t, size_t> String::IndexPair(const String & From, const String & To, size_t AfterIndex, size_t BeforeIndex) const {
	Pair<size_t, size_t> pair(-1, -1);
	size_t key = Index(From, AfterIndex);
	size_t val = Index(To, key + From.size);
	if (key == (size_t)-1 || val == (size_t)-1 || val > BeforeIndex) {
		pair.Key(-1);
		pair.Value(-1);
	}
	else {
		pair.Key(key);
		pair.Value(val + To.size);
	}
	return pair;
}
Pair<size_t, size_t> String::IndexPairAdv(const String & From, const String & To, Vector<char>& CharsBetween, size_t AfterIndex, size_t BeforeIndex) const {
	Pair<size_t, size_t> potential(0, 0);
	bool valid = true;
	while (potential.Key() != (size_t)-1 && potential.Value() != (size_t)-1) {
		valid = true;
		potential = IndexPair(From, To, AfterIndex, BeforeIndex);
		for (size_t i = potential.Key() + From.size; i < potential.Value() - To.size; ++i) {
			if (!CharsBetween.Contains(str[i])) {
				AfterIndex = potential.Key() + From.size;
				valid = false;
				break;
			}
		}
		if (valid)
			return potential;
	}
	return potential;
}
Pair<size_t, size_t> String::IndexPairExcludingNested(const String & From, const String & To, size_t AfterIndex, size_t BeforeIndex) const {
	Pair<size_t, size_t> potential(0, 0);
	potential.Key(Index(From, AfterIndex, BeforeIndex));
	if (potential.Key() == (size_t)-1) {
		potential.Value(-1);
		return potential;
	}
	int fromNumber = 0;
	AfterIndex = potential.Key() + From.size;
	while (true) {
		size_t tmpFromIndex = Index(From, AfterIndex, BeforeIndex);
		size_t tmpToIndex = Index(To, AfterIndex, BeforeIndex);
		if (tmpFromIndex < tmpToIndex) {
			++fromNumber;
			AfterIndex = tmpFromIndex + From.size;
		}
		else if (tmpToIndex == (size_t)-1) {
			break;
		}
		else {
			if (fromNumber == 0) {
				potential.Value(tmpToIndex + To.size);
				return potential;
			}
			--fromNumber;
			AfterIndex = tmpToIndex + To.size;
		}
	}
	return Pair<size_t, size_t>(-1, -1);
}
Vector<Pair<size_t, size_t>> String::IndexAllPairs(const String & From, const String & To, size_t AfterIndex, size_t BeforeIndex) const {
	Vector<Pair<size_t,size_t>> indexes;
	Pair<size_t, size_t> index;
	while (true) {
		index = IndexPair(From, To, AfterIndex);
		if (index.Key() == (size_t)-1 || index.Value() == (size_t)-1 || index.Value() > BeforeIndex)
			break;
		AfterIndex = index.Value() + To.size;
		indexes.Add(index);
	}
	return indexes;
}

Vector<Pair<size_t, size_t>> String::IndexAllPairsOutsidePairs(const String & From, const String & To, size_t AfterIndex, size_t BeforeIndex) const {

	// TODO
	return Vector<Pair<size_t, size_t>>();
}

Vector<Pair<size_t, size_t>> String::IndexAllPairsExcludingNested(const String & From, const String & To, size_t AfterIndex, size_t BeforeIndex) const {

	Vector<Pair<size_t, size_t>> indexes;
	Pair<size_t, size_t> index;
	while (true) {
		index = IndexPairExcludingNested(From, To, AfterIndex, BeforeIndex);
		if (index.Key() == (size_t)-1 || index.Value() == (size_t)-1 || index.Value() > BeforeIndex)
			break;
		AfterIndex = index.Value() + To.size;
		indexes.Add(index);
	}
	return indexes;
}
