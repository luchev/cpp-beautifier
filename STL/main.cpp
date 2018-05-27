#include "Utils.h"
#include "Tests.h"
#include "String.h"
#include "Vector.hpp"
#include "File.h"
#include "CodeFormatter.h"
#include <cstdlib>
#include <crtdbg.h>
#include <fstream>
#include <iostream>
#include <iomanip>

int main(int argc, char **argv) {
	
	// Load the valid extensions
	CodeFormatter cf("", "cpp.ini");
	cf.LoadConfig();
	Vector<String> Extensions = cf.Extensions();

	// Store the files and commands here
	Vector<String> Files;
	Vector<String> Commands;

	// The different possible commands
	String html = "--html";
	String comments = "--comments";
	String indentation = "--indentation";
	String format = "--format";
	String newlines = "--newlines";
	String help = "--help";

	if (argc < 2) {
		std::cout << "Not enough arguments. Try --help\n";
		return 1;
	}

	String arg;
	for (size_t i = 1; i < argc; ++i) {
		arg = argv[i];
		if (arg == html ||
			arg == comments ||
			arg == indentation + "=tabs" ||
			arg == indentation + "=spaces" ||
			arg == format ||
			arg == newlines + "=CRLF" ||
			arg == newlines + "=LF" ||
			arg == newlines + "=CR" ||
			arg == help) {
			Commands.Add(arg);
		}
		else {
			bool addedFile = false;
			for (size_t j = 0; j < Extensions.Size(); ++j) {
				if (arg.EndsWith(Extensions[j])) {
					Files.Add(arg);
					addedFile = true;
					break;
				}
			}
			if (!addedFile) {
				std::cout << "Invalid file or command entered. Try --help\n";
				return 1;
			}
		}
	}

	if (Commands.Contains(help)) { // Handle help
		std::cout << "---Help menu---\n\n" << std::left <<
			std::setw(20) << "Commands:" << "Usage:\n\n" <<
			std::setw(20) << "--help" << "Shows this menu.\n" <<
			std::setw(20) << "--html" << "Applied after all other commands. Creates a <file>.html which is colored.\n" <<
			std::setw(20) << "--comments" << "Removes all comments in the given files.\n" <<
			std::setw(20) << "--indentation" << "Changes the indentation in the given file. --indentation=tabs or --indentation=spaces.\n" <<
			std::setw(20) << "--format" << "Beautifies the code in the file.\n" <<
			std::setw(20) << "--newlines" << "CHanges the new lines in the given file. --newlines=CRLF or --newlines=LF.\n"
			<< "\n";
		return 0;
	}
	else if (Files.Size() == 0) { // Handle when no files are added
		std::cout << "Please provide at least one file to work with.\n";
	}
	else if (Commands.Size() == 0) { // Handle when no commands are added
		std::cout << "Please provide at least one command. Try --help\n";
	}
	else {
		bool SaveHtml = false;
		if (Commands.Contains(html)) {
			Commands.RemoveAll(html);
			SaveHtml = true;
		}

		for (size_t f = 0; f < Files.Size(); ++f) { // For each file execute the proper commands in order
			CodeFormatter file(Files[f], "cpp.ini");
			for (size_t c = 0; c < Commands.Size(); ++c) { // Each command
				if (Commands[c] == comments) { // Comments
					file.RemoveComments();
					std::cout << "Removing comments for " << file.CodeFile() << "\n";
				}
				else if (Commands[c] == format) { // Format
					file.Beautify();
					//file.Format();
					std::cout << "Formatting " << file.CodeFile() << "\n";
				}
				else if (Commands[c].StartsWith(newlines)) { // New lines
					String eol = Commands[c].Split("=")[1].ToUpper();
					file.ChangeNewLines(eol);
					std::cout << "Changing new lines to " << eol << " for " << file.CodeFile() << "\n";
				}
				else if (Commands[c].StartsWith(indentation)) { // Indentation
					String indent = Commands[c].Split("=")[1].ToUpper();
					file.ChangeIndentation(indent);
					std::cout << "Changing indentation to " << indent << " for " << file.CodeFile() << "\n";
				}
			}

			if (SaveHtml) { // Save as html only
				file.Html();
				String fileName = file.CodeFile().Substr(0, file.CodeFile().LastIndex(".")) + ".html";
				file.SaveHtmlAs(fileName);
				std::cout << "Saving " << fileName << "\n";
			}
			else { // Save new formatted file with the same name and <file>.old with the old code
				file.SaveOriginal(file.CodeFile() + ".old");
				std::cout << "Saving original code in " << file.CodeFile() + ".old" << "\n";
				file.Save();
				std::cout << "Saving formatted code in " << file.CodeFile() << "\n";
			}
		}

	}



	//CodeFormatter c("Test.txt", "cpp.ini");
	//c.Beautify();
	//c.SaveAs("Test2.txt");
	//c.Html();
	//c.SaveHtmlAs("Test2.html");


#if DEBUG == 1
	//StringMemoryTest();
	//VectorMemoryTest();
	_CrtDumpMemoryLeaks();
#endif

	std::cout << std::endl;
	return 0;
}