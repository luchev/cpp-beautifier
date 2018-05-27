#pragma once
#include <iostream>
template <typename T, typename U>
class Pair {
private:
	T key;
	U value;
public:
	Pair(T Key = T(), U Value = U());
	void Key(T Key);
	void Value(U Value);
	T Key() const;
	U Value() const;
	bool operator==(const Pair<T, U> AnotherPair);
	bool operator!=(const Pair<T, U> AnotherPair);
};

template <typename T, typename U>
 Pair<T, U>::Pair(T Key, U Value) : key(Key), value(Value) {
}
template <typename T, typename U>
void Pair<T, U>::Key(T Key) {
	key = Key;
}
template <typename T, typename U>
void Pair<T, U>::Value(U Value) {
	value = Value;
}
template <typename T, typename U>
T Pair<T, U>::Key()  const{
	return key;
}
template <typename T, typename U>
U Pair<T, U>::Value() const {
	return value;
}

template<typename T, typename U>
bool Pair<T, U>::operator==(const Pair<T, U> AnotherPair)
{
	return key == AnotherPair.key && value == AnotherPair.value;
}

template<typename T, typename U>
inline bool Pair<T, U>::operator!=(const Pair<T, U> AnotherPair)
{
	return !operator==(AnotherPair);
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, Pair<T, U> pair) {
	return os << pair.Key() << ":" << pair.Value();
}