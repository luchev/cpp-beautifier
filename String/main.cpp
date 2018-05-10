#include <iostream>
#include "String.h"
#include <bitset>

int main() {
	String s = "ABC";
	s.ToLower();
	std::cout << s;

	//String p("One");
	//String d = p;
	//std::cout << p << "<< P \n";
	//std::cout << d << "<< D \n";

	//std::cout << s << '\n';
	//s.Set("Test");
	//std::cout << s << '\n';
	//s = p;
	//std::cout << s.Get() << '\n';
	//s = "Reset";
	//std::cout << s << '\n';
	//s += '-';
	//std::cout << s << '\n';
	//s += "Two";
	//std::cout << s << '\n';
	//std::cout << s.Length() << '\n';
	//std::cout << s[1] << '\n';
	//String f = s.SubString(6, 8);
	//std::cout << f << '\n';
	//f.Append(p);
	//std::cout << f << '\n';
	//String k = f.Concatinate(s);
	//std::cout << k << '\n';

	//std::cin >> s;
	//std::cin >> s;
	//std::cout << s << "\n";

	system("pause");
	return 0;
}