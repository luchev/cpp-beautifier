#include "Utils.h"
#include "String.h"
#include "Vector.hpp"

void VectorMemoryTest() {
	Vector<String> p;
	p.Add("Test");
	p.Add("ONE");
	Vector<int> v;
	v.Add(2);
	v.Add(5);
	v.Add(6);
	std::cout << v.Size();
	std::cout << v.RealSize();
	v.RemoveAt(1);
	std::cout << v.Size();
	std::cout << v.RealSize();
	std::cout << v.RealSize();
	for (size_t i = 0; i < v.Size(); ++i) {
		std::cout << v[i] << "\n";
	}
	std::cout << v[6];
	std::cout << v.Index(5);
	std::cout << v.LastIndex(5);
	std::cout << v.Contains(5);
	for (size_t i = 0; i < v.Size(); ++i) {
		std::cout << v[i] << "\n";
	}
	std::cout << "\n";

}

void StackMemoryTests() {
	// TODO
}

void StringMemoryTest() {
	String s;
	String s2 = 'a';
	String s3 = "Test";
	s2 = s3;
	String p = s3;
	s = p;
	s.Set('a');
	s.Set("Abc");
	s.Set(s2);
	s.SetAt('c', 2);
	s = "Testo";
	s = 'b';
	s = s2;
	s.Append('-');
	s.Append("One");
	s.Append(s2);
	s += "Two";
	s.InsertAt(8, " - ");
	char c = s[2];
	bool a = s < "One";
	size_t len = s.Size();
	size_t size = s.RealSize();
	s = s.Concat(s);
	s = s + s;
	s = s.Substr(0, 8);
	s2 = s.Clone();
	s.Clear();
	s = s2.ToLower();
	s.ToUpper();
	s.InsertAt(0, "   ");
	s.TrimStart();
	s.Append("   asd   ").Trim();
	s = "  ONE TWO - THREE   ";
	s.Trim();
	s.RemoveAt(0);
	s.RemoveAt(s.Size());
	s.Replace("NE", "REEEE");
	s.ReplaceAll("RE", "TWO");
	s.RemoveAll("TWO");
	s.Reverse().Reverse();
	std::cout << s.Contains("THE");
	std::cout << s.StartsWith("EEE ");
	std::cout << s.EndsWith("THE");
	s = "ee - - e";
	std::cout << s.Index("- -");
	std::cout << s.LastIndex("e");
	s.Trim();
	//std::cin >> s;
	std::cout << s << "^\n";
	s = "aaabaacd";
	std::cout << s.Index('a', 3) << "\n";
	std::cout << s.LastIndex('a', 3) << "\n";
	// TODO
}