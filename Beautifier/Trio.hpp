#pragma once

#include <iostream>
template <typename T, typename U, typename V>
class Trio {
private:
	T a;
	U b;
	V c;
public:
	Trio(T A = T(), U B = U(), V C = V());
	void A(T Val);
	void B(U Val);
	void C(V Val);
	T A() const;
	U B() const;
	V C() const;
	bool operator==(const Trio<T, U, V> AnotherTrio);
	bool operator!=(const Trio<T, U, V> AnotherTrio);
};


template<typename T, typename U, typename V>
std::ostream& operator<<(std::ostream& stream, const Trio<T, U, V>& trio) {
	stream << trio.A() << " - " << trio.B() << " - " << trio.C();
	return stream;
}

template<typename T, typename U, typename V>
inline Trio<T, U, V>::Trio(T A, U B, V C) : a(A), b(B), c(C)
{
}

template<typename T, typename U, typename V>
inline void Trio<T, U, V>::A(T Val)
{
	a = Val;
}

template<typename T, typename U, typename V>
inline void Trio<T, U, V>::B(U Val)
{
	b = Val;
}

template<typename T, typename U, typename V>
inline void Trio<T, U, V>::C(V Val)
{
	c = Val;
}

template<typename T, typename U, typename V>
inline T Trio<T, U, V>::A() const
{
	return a;
}

template<typename T, typename U, typename V>
inline U Trio<T, U, V>::B() const
{
	return b;
}

template<typename T, typename U, typename V>
inline V Trio<T, U, V>::C() const
{
	return c;
}

template<typename T, typename U, typename V>
inline bool Trio<T, U, V>::operator==(const Trio<T, U, V> AnotherTrio)
{
	return a == AnotherTrio.a && b == AnotherTrio.b && c == AnotherTrio.c;
}

template<typename T, typename U, typename V>
inline bool Trio<T, U, V>::operator!=(const Trio<T, U, V> AnotherTrio)
{
	return !operator==(AnotherTrio);
}
