#ifndef CODEFORMATTER_H
#define CODEFORMATTER_H
#include "String.h"
#include "File.h"

class CodeFormatter {
private: // Helper functions and members
	size_t isInVector(const char * Str, Vector<String> Vect, bool VarCharAfter = true) const; // Returns whether Str matches one of the words in the vector compaing with the VarCharAfter functions
	size_t longestMatch(const char * Str, Vector<String> Vect, bool VarCharAfter = true) const; // Returns the longest match from the vector
	size_t isNumber(const char * Str) const; // Returns the index of where the number ends, -1 if no number is found from the start
	bool StrEqualsWithVarCharAfter(const char* What, const char* Where, size_t Limit) const; // Compares the stringgs and checks the last char in Where for being a variable char
	bool StrBeginsWith(const char* What, const char* Where, size_t Limit) const; // Returns true if Where begins with What
	bool VarChar(char Ch) const; // Checks if the character is not a letter or number or underscore

	// Heavy functions doing random stuff
	void RemoveEmptyLines(bool OnlyWithSpaces = false); // Removes all empty lines, to be used after removing comments to clean up the code
	void RemoveEmptySpace(); // Removes all double sapces, leaving one at most
	void StatementsOnNewLine(); // Puts each statement, {, }, or () on a new line
	void TrimLines(); // Trims each line of leading or ending " " (space), run RemoveEmptySpace before this
	void RemoveSpacesAroundBrackets(); // Run RemoveEmptySpace before this
	void RemoveSpacesAroundOperators(); // Run RemoveEmptySpace before this
	void AddSpacesAroundOperators(); // Run RemoveEmptySpace before this
	void RemoveNewLinesAroundOperators(); // Run RemoveSpacesAroundOperators before this
	void RemoveNewLineBeforeOpenBrackets(); // Replaces "\n(" with "(", run StatementsOnNewLine, TrimLines and RemoveSpacesAroundParentheses before this
	void AddSpaceBetweenKeywordsAndBrackets(); // Replaces keyword + ( with keyword + space + (, run StatementsOnNewLine, TrimLines, RemoveSpacesAroundParentheses and RemoveNewLineBeforeOpenParentheses before this
	void Indent(); // Indent every new line properly, run StatementsOnNewLine, TrimLines and RemoveSpacesAroundParentheses and other of the above methods if needed before this

	// From the config
	Vector<String> brackets;
	Vector<String> operators;
	Vector<String> keywords;
	Vector<String> keywords_with_indent;
	Vector<String> keywords_with_brackets;
	Vector<String> extensions;
protected:
	String codeFile;
	String configFile;
	String codeOriginal;
	String codeFormatted;
	String config;
	String html;
public:
	CodeFormatter(String CodeFile = "", String ConfigFile = "");
	String CodeFile() const;
	String ConfigFile() const;
	String Code() const;
	String Config() const;
	Vector<String> Extensions() const;
	void Code(const String& Cod);
	void Config(const String& Conf);
	void CodeFile(const String& Name);
	void ConfigFile(const String& Name);
	void LoadCode();
	void LoadConfig();
	void Save();
	void SaveOriginal(String Name);
	void SaveAs(String Name);
	void SaveHtmlAs(String Name);

	void RemoveComments(); // Removes all comments in the code
	void ChangeNewLines(const String Format); // Changes new lines LF = "\n" CRLF = "\r\n"
	void ChangeIndentation(const String Format); // Changes the indentation TABS = "\t" SPACES = "    " (4 spaces)
	void Format(); // Formats the code thoroughly
	void Beautify(); // Fixes the code indentation, spacing, etc.
	void Html(); // Converts the code to colored html and saves it in the html variable
};

#endif