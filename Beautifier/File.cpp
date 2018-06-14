#include "File.h"
#include <fstream>

File::File(String Name) : name(Name) {
}

void File::Name(String NewName) {
	name = NewName;
}

String File::Name() const {
	return name;
}

bool File::ReadAll(String& Str) {
	std::ifstream is(name.Get());
	if (is.fail())
		return false;
	Str.ReadStream(is);
	return true;
}

bool File::WriteAll(const String& Str) {
	std::ofstream os(name.Get(), std::ios_base::binary);
	if (os.fail())
		return false;
	os << Str;
	return true;
}

bool File::AppendAll(const String& Str)
{
	std::ofstream os(name.Get(), std::ofstream::app || std::ios_base::binary);
	if (os.fail())
		return false;
	os << Str;
	return true;
}
