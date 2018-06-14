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

int main(int argc, char *argv[]) {
	// Create the backup cpp.ini file if one isn't present so the program can use it below
	String cppini = "language_name = C++\r\n\r\nfile_extensions = c, C, cpp, c++, cc, cxx, cp, h, H, hpp, h++, hh, hxx. hp\r\n\r\nkeywords = alignas,alignof,and,and_eq,asm,atomic_cancel,atomic_commit,atomic_noexcept,auto,bitand,bitor,bool,break,case,catch,char,char16_t,char32_t,class,compl,concept,const,constexpr,const_cast,continue,co_await,co_return,co_yield,decltype,default,delete,do,double,dynamic_cast,else,enum,explicit,export,extern,false,float,for,friend,goto,if,import,inline,int,long,module,mutable,namespace,new,noexcept,not,not_eq,nullptr,operator,or,or_eq,private,protected,public,register,reinterpret_cast,requires,return,short,signed,sizeof,static,static_assert,static_cast,struct,switch,synchronized,template,this,thread_local,throw,true,try,typedef,typeid,typename,union,unsigned,using,virtual,void,volatile,wchar_t,while,xor,xor_eq,override,final,transaction_safe,transaction_safe_dynamic,\r\n\r\nkeywords_with_indent = if,for,while,switch, catch, else if, else, try\r\n\r\nkeywords_with_brackets = if, for, while, switch, else if, catch\r\n\r\noperators = +,-,*,/,%,^,|,||,&,&&,~,<<,>>,<,>,=,==,!=,>=,<=,!,+=,-=,,*=,/=,%=,&=,|=,^=,<<=,>>=,++,--,.,::,?,:,;\r\n\r\nbrackets = (,),{,},[,]\r\n\r\nraw_css = \"@font-face { font-family: Fira Code; src: url('https://github.com/tonsky/FiraCode/blob/master/distr/otf/FiraCode-Regular.otf?raw=true'); }\"\r\n\r\nbody_formattting = \"background-color: #1e1e1e; color: c8c8c8; font-family: Fira Code;\"\r\n\r\ncomment_formatting = \"color: #57a64a; font-style: italic;\"\r\n\r\nkeyword_formatting = \"color: #3073d6; font-weight: bold;\"\r\n\r\nstring_formatting = \"color: #d69d85;\"\r\n\r\npreprocessor_formatting = \"color: #7f7f7f;\"\r\n\r\nnumber_formatting = \"color: #bd63c5;\"\r\n\r\nchar_formatting = \"color: #b5b366;\"\r\n\r\noperator_formatting = \"color: #ffdc00;\"\r\n\r\nbracket_formatting = \"color: #ae2344;\"\r\n\r\n; End of settings\r\n\r\n";
	String tmpCheck;
	File("cpp.ini").ReadAll(tmpCheck);
	if (tmpCheck.Size() < 1) {
		File("cpp.ini").WriteAll(cppini);
	}
	
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
	//_CrtDumpMemoryLeaks();
#endif

	std::cout << std::endl;
	return 0;
}