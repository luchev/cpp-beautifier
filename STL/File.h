#ifndef FILE_H
#define FILE_H

#include "String.h"
#include <iostream>
#include <fstream>
#include "Utils.h"

class File {
private:
	String name;
public:
	File(String Name = "");
	void Name(String NewName);
	String Name() const;
	bool ReadAll(String& Str); // Reads the contents of the file into the string
	bool WriteAll(const String& Str); // Writes the contents of the string to the file
	bool AppendAll(const String& Str); // Appends the contents of the string to the file
};

#endif