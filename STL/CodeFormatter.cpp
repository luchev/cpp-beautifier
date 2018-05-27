#include "CodeFormatter.h"
#include "File.h"
#include <iostream>
#include "Pair.hpp"
#include "String.h"
#include "Vector.hpp"
#include "Stack.hpp"

CodeFormatter::CodeFormatter(String CodeFile, String ConfigFile) : codeFile(CodeFile), configFile(ConfigFile) {
	if (CodeFile.Size() != 0 && ConfigFile.Size() != 0) {
		LoadCode();
		LoadConfig();
	}
}

String CodeFormatter::CodeFile() const
{
	return codeFile;

}

String CodeFormatter::ConfigFile() const
{
	return configFile;
}

String CodeFormatter::Code() const
{
	return codeFormatted;
}

String CodeFormatter::Config() const {
	return config;
}

Vector<String> CodeFormatter::Extensions() const {
	return extensions;
}

void CodeFormatter::Code(String Cod) {
	codeFormatted = Cod;
}

void CodeFormatter::Config(String Conf) {
	config = Conf;
}

void CodeFormatter::CodeFile(String Name) {
	codeFile = Name;
}

void CodeFormatter::ConfigFile(String Name) {
	configFile = Name;
}

void CodeFormatter::LoadCode() {
	File f(codeFile);
	f.ReadAll(codeOriginal);
	codeFormatted = codeOriginal;
}

void CodeFormatter::LoadConfig() {
	File f(configFile);
	f.ReadAll(config);

	// Get extensions
	extensions = config.GetLine(config.Index("extensions")).ReplaceFromTo("extensions", "=", "").Split(",");
	for (size_t i = 0; i < extensions.Size(); ++i) {
		extensions[i].Trim();
		if (extensions[i].Size() == 0)
			extensions.RemoveAt(i--);
	}

	// Get keywords
	keywords = config.GetLine(config.Index("keywords")).ReplaceFromTo("keywords", "=", "").Split(",");
	for (size_t i = 0; i < keywords.Size(); ++i) {
		keywords[i].Trim();
		if (keywords[i].Size() == 0)
			keywords.RemoveAt(i--);
	}

	// Get keywords with indent
	keywords_with_indent = config.GetLine(config.Index("keywords_with_indent")).ReplaceFromTo("keywords_with_indent", "=", "").Split(",");
	for (size_t i = 0; i < keywords_with_indent.Size(); ++i) {
		keywords_with_indent[i].Trim();
		if (keywords_with_indent[i].Size() == 0)
			keywords_with_indent.RemoveAt(i--);
	}

	// Get keywords with brackets
	keywords_with_brackets = config.GetLine(config.Index("keywords_with_brackets")).ReplaceFromTo("keywords_with_brackets", "=", "").Split(",");
	for (size_t i = 0; i < keywords_with_brackets.Size(); ++i) {
		keywords_with_brackets[i].Trim();
		if (keywords_with_brackets[i].Size() == 0)
			keywords_with_brackets.RemoveAt(i--);
	}

	// Get operators
	operators = config.GetLine(config.Index("operators")).ReplaceFromTo("operators", "=", "").Split(",");
	operators.Add(",");
	for (size_t i = 0; i < operators.Size(); ++i) {
		operators[i].Trim();
		if (operators[i].Size() == 0)
			operators.RemoveAt(i--);
	}

	// Get brackets
	brackets = config.GetLine(config.Index("brackets")).ReplaceFromTo("brackets", "=", "").Split(",");
	for (size_t i = 0; i < brackets.Size(); ++i) {
		brackets[i].Trim();
		if (brackets[i].Size() == 0)
			brackets.RemoveAt(i--);
	}
}

void CodeFormatter::Save() {
	File f(codeFile);
	f.WriteAll(codeFormatted);
}

void CodeFormatter::SaveOriginal(String Name) {
	File f(Name);
	f.WriteAll(codeOriginal);
}

void CodeFormatter::SaveAs(String Name) {
	File f(Name);
	f.WriteAll(codeFormatted);
}

void CodeFormatter::SaveHtmlAs(String Name) {
	File f(Name);
	f.WriteAll(html);
}

void CodeFormatter::RemoveComments() {
	String newCode; // New code stored here;
	bool openQuotes = false;
	bool openOneLineComment = false;
	bool openComment = false;
	bool openChar = false;
	char lastChar = '\n';
	bool addChar = true;
	char ii = 0;

	for (size_t i = 0; i < codeFormatted.Size(); ++i) {
		ii = codeFormatted[i];
		lastChar = codeFormatted[i - 1];
		addChar = true;

		if (ii == '/' && !openQuotes && !openChar) {
			if (codeFormatted[i + 1] == '/' && !openOneLineComment) { // One line comment
				openOneLineComment = true;
			}
			else if (codeFormatted[i + 1] == '*' && !openComment) { // Multiline comment
				openComment = true;
			}
			else if (lastChar == '*' && openComment) { // Close multiline comment
				openComment = false;
				addChar = false;
			}
		}
		else if (ii == '"' && !openOneLineComment && !openComment && !openChar) {
			if (!openQuotes && lastChar != '\\') { // Open Quotes
				openQuotes = true;
			}
			else if (openQuotes) {
				if (lastChar != '\\' || (lastChar == '\\' && newCode[i - 2] == '\\'))
					openQuotes = false; // Close Quotes
			}
		}
		else if (ii == '\'' && !openQuotes && !openComment && !openOneLineComment) {
			//std::cout << i << "\n";
			if (!openChar && lastChar != '\\') {
				openChar = true; // Open char
			}
			else if (openChar) {
				if (lastChar != '\\' || (lastChar == '\\' && codeFormatted[i - 2] == '\\'))
					openChar = false; // Close char
			}
		}

		if (ii == '\n' || ii == '\r') {
			openOneLineComment = false;
		}

		if (!openOneLineComment && !openComment && addChar)
			newCode.Append(ii);
	}
	codeFormatted = newCode;
}

void CodeFormatter::ChangeNewLines(const String Format) {
	if (Format == "LF") {
		codeFormatted.ReplaceAll("\r\n", "\n");
		codeFormatted.ReplaceAll("\r", "\n");
	}
	else if (Format == "CRLF") {
		for (size_t i = 0; i < codeFormatted.Size(); ++i) {
			if (codeFormatted[i - 1] != '\r' && codeFormatted[i] == '\n') {
				codeFormatted.InsertAt(i, '\r');
			}
			else if (codeFormatted[i] == '\r' && codeFormatted[i + 1] != '\n') {
				codeFormatted.InsertAt(i + 1, '\n');
			}
		}
	}
	else if (Format == "CR") {
		codeFormatted.ReplaceAll("\r\n", "\r");
		codeFormatted.ReplaceAll("\n", "\r");
	}
}

void CodeFormatter::ChangeIndentation(const String Format) {
	bool spaces = Format == "SPACES";
	bool tabs = Format == "TABS";
	if (!spaces && !tabs)
		return;
	String newCode; // New code stored here;
	bool openQuotes = false;
	bool openOneLineComment = false;
	bool openComment = false;
	bool openChar = false;
	char lastChar = '\n';
	bool addChar = true;
	char ii = 0;
	const int NUMBERSPACES = 4;
	String whiteSpace = String(' ') * NUMBERSPACES;

	for (size_t i = 0; i < codeFormatted.Size(); ++i) {
		ii = codeFormatted[i];
		lastChar = codeFormatted[i - 1];
		addChar = true;

		if (ii == '/' && !openQuotes && !openChar) {
			if (codeFormatted[i + 1] == '/' && !openOneLineComment) {
				openOneLineComment = true; // Open one line comment
			}
			else if (codeFormatted[i + 1] == '*' && !openComment) {
				openComment = true; // Open multiline comment
			}
			else if (lastChar == '*' && openComment) {
				openComment = false; // Close multiline comment
			}
		}
		else if (ii == '"' && !openOneLineComment && !openComment && !openChar) {
			if (!openQuotes && lastChar != '\\') {
				openQuotes = true; // Open Quotes
			}
			else if (openQuotes) {
				if (lastChar != '\\' || (lastChar == '\\' && codeFormatted[i - 2] == '\\'))
					openQuotes = false; // Close Quotes
			}
		}
		else if (ii == '\'' && !openQuotes && !openComment && !openOneLineComment) {
			if (!openChar && lastChar != '\\') {
				openChar = true; // Open char
			}
			else if (openChar) {
				if (lastChar != '\\' || (lastChar == '\\' && codeFormatted[i - 2] == '\\'))
					openChar = false; // Close char
			}
		}
		else if (ii == ' ' && tabs && !openOneLineComment && !openComment && !openQuotes && !openChar) {
			if (StrEquals(codeFormatted.Get() + i, whiteSpace.Get(), whiteSpace.Size())) {
				newCode.Append('\t');
				i += whiteSpace.Size() - 1;
				addChar = false;
			}
		}
		else if (ii == '\t' && spaces && !openOneLineComment && !openComment && !openQuotes && !openChar) {
			newCode.Append(whiteSpace);
			addChar = false;
		}

		if (ii == '\n' || ii == '\r') {
			openOneLineComment = false;
		}

		if (addChar)
			newCode.Append(ii);
	}
	codeFormatted = newCode;
}

void CodeFormatter::RemoveEmptyLines(bool OnlyWithSpaces) {
	codeFormatted.RemoveEmptyLines(!OnlyWithSpaces, true, true);
}

void CodeFormatter::Format() {
	RemoveEmptySpace();
	RemoveSpacesAroundBrackets();
	RemoveNewLinesAroundOperators();
	RemoveSpacesAroundOperators();
	StatementsOnNewLine();
	RemoveEmptyLines();
	TrimLines();
	RemoveNewLineBeforeOpenBrackets();
	AddSpaceBetweenKeywordsAndBrackets();
	Indent();
}

void CodeFormatter::RemoveEmptySpace() {
	Vector<Pair<size_t, size_t>> quotes = codeFormatted.IndexAllPairs("\"", "\"");
	codeFormatted.ReplaceAllOutsidePairs("\t", "", quotes);
	codeFormatted.ReplaceAll("  ", " ");
}

void CodeFormatter::StatementsOnNewLine() {
	Vector<Pair<size_t, size_t>> quotes = codeFormatted.IndexAllPairs("\"", "\"");
	Vector<Pair<size_t, size_t>> parentheses = codeFormatted.IndexAllPairsExcludingNested("(", ")");
	Vector<Pair<size_t, size_t>> comments1 = codeFormatted.IndexAllPairs("/*", "*/");
	Vector<Pair<size_t, size_t>> comments2 = codeFormatted.IndexAllPairs("//", "\n");
	quotes.Append(parentheses).Append(comments1).Append(comments2); // Combine the quotes and the brackets
	codeFormatted.ReplaceAllOutsidePairs(";", ";\n", quotes); // Make all statements after ; on a new line

	quotes = codeFormatted.IndexAllPairs("\"", "\"");
	parentheses = codeFormatted.IndexAllPairsExcludingNested("(", ")");
	comments1 = codeFormatted.IndexAllPairs("/*", "*/");
	comments2 = codeFormatted.IndexAllPairs("//", "\n");
	quotes.Append(parentheses).Append(comments1).Append(comments2);
	codeFormatted.ReplaceAllOutsidePairs("{", "\n{\n", quotes); // Make all statements after { on a new line

	quotes = codeFormatted.IndexAllPairs("\"", "\"");
	parentheses = codeFormatted.IndexAllPairsExcludingNested("(", ")");
	comments1 = codeFormatted.IndexAllPairs("/*", "*/");
	comments2 = codeFormatted.IndexAllPairs("//", "\n");
	quotes.Append(parentheses).Append(comments1).Append(comments2);
	codeFormatted.ReplaceAllOutsidePairs("}", "\n}\n", quotes); // Make all statements after } on a new line

	quotes = codeFormatted.IndexAllPairs("\"", "\"");
	parentheses = codeFormatted.IndexAllPairsExcludingNested("(", ")");
	comments1 = codeFormatted.IndexAllPairs("/*", "*/");
	comments2 = codeFormatted.IndexAllPairs("//", "\n");
	quotes.Append(comments1).Append(comments2);
	Vector<size_t> lastBracketIndexes;
	bool shouldAdd = true;
	for (size_t i = 0; i < parentheses.Size(); ++i) {
		shouldAdd = true;
		for (size_t j = 0; j < quotes.Size(); ++j) {
			if (parentheses[i].Key() >= quotes[j].Key() && parentheses[i].Value() <= quotes[j].Value()) {
				shouldAdd = false;
				break;
			}
		}
		if (shouldAdd && codeFormatted[parentheses[i].Value()] != ';')
			lastBracketIndexes.Add(parentheses[i].Value() - 1);
	}
	codeFormatted.ReplaceAllSafe(")", ")\n", lastBracketIndexes); // Make all statements after closing ) on a new line

	quotes = codeFormatted.IndexAllPairs("\"", "\"");
	parentheses = codeFormatted.IndexAllPairsExcludingNested("(", ")");
	comments1 = codeFormatted.IndexAllPairs("/*", "*/");
	comments2 = codeFormatted.IndexAllPairs("//", "\n");
	quotes.Append(comments1).Append(comments2);	Vector<size_t> firstBracketIndexes;
	for (size_t i = 0; i < brackets.Size(); ++i) {
		shouldAdd = true;
		for (size_t j = 0; j < quotes.Size(); ++j) {
			if (parentheses[i].Key() >= quotes[j].Key() && parentheses[i].Value() <= quotes[j].Value()) {
				shouldAdd = false;
				break;
			}
		}
		if (shouldAdd)
			firstBracketIndexes.Add(parentheses[i].Key());
	}
	codeFormatted.ReplaceAllSafe("(", "\n(", firstBracketIndexes); // Make all statements after opening ( on a new line
}

void CodeFormatter::TrimLines() {
	codeFormatted.ReplaceAll("\n ", "\n");
	codeFormatted.ReplaceAll(" \n", "\n");
}

void CodeFormatter::RemoveSpacesAroundBrackets() {
	LoadConfig();

	Vector<Pair<size_t, size_t>> quotes;

	for (size_t i = 0; i < brackets.Size(); ++i) {
		brackets[i].Trim();
		quotes = codeFormatted.IndexAllPairs("\"", "\"");
		codeFormatted.ReplaceAllOutsidePairs(brackets[i] + String(' '), brackets[i], quotes);
		codeFormatted.ReplaceAllOutsidePairs(String(' ') + brackets[i], brackets[i], quotes);
	}
}

void CodeFormatter::RemoveSpacesAroundOperators() {
	LoadConfig();

	Vector<Pair<size_t, size_t>> quotes;
	Vector<Pair<size_t, size_t>> preprocessor;

	for (size_t i = 0; i < operators.Size(); ++i) {
		operators[i].Trim();
		quotes = codeFormatted.IndexAllPairs("\"", "\"");
		preprocessor = codeFormatted.IndexAllPairs("#", "\n");
		quotes.Append(preprocessor);
		codeFormatted.ReplaceAllOutsidePairs(operators[i] + String(' '), operators[i], quotes);
		codeFormatted.ReplaceAllOutsidePairs(String(' ') + operators[i], operators[i], quotes);
	}
}

void CodeFormatter::AddSpacesAroundOperators() {
	// TODO
}

void CodeFormatter::RemoveNewLinesAroundOperators() {
	LoadConfig();

	Vector<Pair<size_t, size_t>> quotes;
	Vector<Pair<size_t, size_t>> preprocessor;

	for (size_t i = 0; i < operators.Size(); ++i) {
		operators[i].Trim();
		quotes = codeFormatted.IndexAllPairs("\"", "\"");
		preprocessor = codeFormatted.IndexAllPairs("#", "\n");
		quotes.Append(preprocessor);
		codeFormatted.ReplaceAllOutsidePairs(operators[i] + String('\n'), operators[i], quotes);
		codeFormatted.ReplaceAllOutsidePairs(String('\n') + operators[i], operators[i], quotes);
	}
}

void CodeFormatter::RemoveNewLineBeforeOpenBrackets() {
	String replacer = "(";
	codeFormatted.ReplaceAllSafe("\n(", replacer);
}

void CodeFormatter::AddSpaceBetweenKeywordsAndBrackets() {
	LoadConfig();

	Vector<Pair<size_t, size_t>> quotes;

	for (size_t i = 0; i < keywords_with_indent.Size(); ++i) {
		quotes = codeFormatted.IndexAllPairs("\"", "\"");
		codeFormatted.ReplaceAllOutsidePairs(keywords_with_indent[i] + String('('), keywords_with_indent[i] + " (", quotes);
	}
}

void CodeFormatter::Indent() {
	if (codeFormatted.Size() == 0) // Safety check
		return;

	LoadConfig();

	Vector<size_t> lines; // Indexes at which each line starts
	lines.Add(0);

	// Index all the starts of lines for use below
	size_t afterIndex = 0;
	size_t tmpIndex = 0;
	while (true) {
		tmpIndex = codeFormatted.Index("\n", afterIndex);
		if (tmpIndex == (size_t)-1)
			break;
		if (codeFormatted[tmpIndex + 1] != 0)
			lines.Add(tmpIndex + 1);
		afterIndex = tmpIndex + 1;
	}

	// Actual Indentation
	int indentLevel = 0; // Keeps the current indented level
	int indentOne = 0; // Keeps the indented level for if, for and the other statements which are without { }
	bool startsWithKeyWord;
	String tab("\t");
	for (size_t i = 0; i < lines.Size(); ++i) {
		startsWithKeyWord = false;
		for (size_t j = 0; j < keywords.Size(); ++j) {
			if (StrEquals(codeFormatted.Get() + lines[i], keywords[j].Get(), keywords[j].Size())) {
				startsWithKeyWord = true;
				break;
			}
		}
		// Check if the beginning is a special word
		if (startsWithKeyWord) {
			codeFormatted.InsertAt(lines[i], indentLevel * tab);
			for (size_t j = i; j < lines.Size(); ++j)
				lines[j] += indentLevel;
			if (codeFormatted[codeFormatted.Index("\n", lines[i]) - 1] != ';') {
				++indentLevel;
				++indentOne;
			}
		}
		else if (codeFormatted[lines[i]] == '{') {
			if (indentLevel > 0)
				--indentLevel;
			if (indentOne > 0)
				--indentOne;
			codeFormatted.InsertAt(lines[i], indentLevel * tab);
			for (size_t j = i; j < lines.Size(); ++j)
				lines[j] += indentLevel;

			++indentLevel;
		}
		else if (codeFormatted[lines[i]] == '}') {
			if (indentLevel > 0)
				--indentLevel;
			codeFormatted.InsertAt(lines[i], indentLevel * tab);
			for (size_t j = i; j < lines.Size(); ++j)
				lines[j] += indentLevel;
		}
		else {
			codeFormatted.InsertAt(lines[i], indentLevel * tab);
			for (size_t j = i; j < lines.Size(); ++j)
				lines[j] += indentLevel;
			indentLevel -= indentOne;
			if (indentOne > 0)
				indentOne = 0;
		}
	}
}

void CodeFormatter::Beautify() {
	String newCode; // New code stored here;
	bool preprocessorLine = false;
	bool openQuotes = false;
	bool openChar = false;
	bool openOneLineComment = false;
	bool openComment = false;
	char lastChar = '\n'; // Last char added to the new code
	bool addChar = true; // Should the loop add a char
	char ii = 0; // Current char
	int indent = 0;
	int bracketRound = 0;
	int bracketCurly = 0;
	size_t indentkeyword = 0;
	size_t bracketkeyword = 0;
	size_t operatorr = 0;
	String tab = "\t";
	Stack<size_t> ifIndents; // The indent of the last if is stored here
	bool bracketKeywordBrackets = false; // If the brackets () were after a for or if
	int noBracketBlock = 0; // How many if or for have openeded without brackets

	// Actual formatting
	for (size_t i = 0; i < codeFormatted.Size(); ++i) {
		ii = codeFormatted[i];
		addChar = true;
		lastChar = newCode[newCode.Size() - 1];

		//
		// Index without changing anyting in the string, only to set different flags
		//
		// Comments
		if (ii == '/' && !openQuotes && !openChar) {
			if (codeFormatted[i + 1] == '/' && !openOneLineComment) {
				openOneLineComment = true; // Open one line comment
				if (!IsEmptySpace(lastChar)) {
					newCode.Append(' ');
				}
			}
			else if (codeFormatted[i + 1] == '*' && !openComment) {
				openComment = true; // Open multiline comment
			}
			else if (lastChar == '*' && openComment) {
				openComment = false; // Close multiline comment
			}
		}
		// Quotes
		else if (ii == '"' && !openOneLineComment && !openComment && !openChar) {
			if (!openQuotes && lastChar != '\\') {
				openQuotes = true; // Open Quotes
			}
			else if (openQuotes) {
				if (lastChar != '\\' || (lastChar == '\\' && codeFormatted[i - 2] == '\\'))
					openQuotes = false; // Close Quotes
			}
		}
		// Char
		else if (ii == '\'' && !openQuotes && !preprocessorLine && !openComment && !openOneLineComment) {
			//std::cout << i << "\n";
			if (!openChar && lastChar != '\\') {
				openChar = true; // Open char
			}
			else if (openChar) {
				if (lastChar != '\\' || (lastChar == '\\' && codeFormatted[i - 2] == '\\'))
					openChar = false; // Close char
			}
		}
		// New line
		else if (ii == '\n' || ii == '\r') {
			if (preprocessorLine) {
				preprocessorLine = false; // Close preprocessor line
			}
			if (openOneLineComment) {
				openOneLineComment = false; // Close one line comment
			}
		}

		//
		// The logic for adding characters to the new code string, indenting, etc. goes here
		//
		if (!preprocessorLine && !openComment && !openOneLineComment && !openQuotes && !openChar) {
			indentkeyword = longestMatch(codeFormatted.Get() + i, keywords_with_indent);
			bracketkeyword = longestMatch(codeFormatted.Get() + i, keywords_with_brackets);
			operatorr = longestMatch(codeFormatted.Get() + i, operators, false);

			if (bracketkeyword < (size_t)-1) {
				bracketKeywordBrackets = true;
			}
			else if (indentkeyword < (size_t)-1) {
				// TODO
			}
			else if (operatorr < (size_t)-1) { // includes ;
				// TODO handle more operators
				if (operators[operatorr] == ";") { // Handle ;
					newCode.TrimEnd();
					if (bracketRound < 1) { // if not inside ( )
						if (noBracketBlock > 0) {
							--noBracketBlock;
							--indent;
						}

						for (; codeFormatted[i + 1] == '\t' || codeFormatted[i + 1] == ' '; ++i); // Ignore all white space until new line or // for a comment
						if (codeFormatted[i + 1] == '/') { // A comment is following
							newCode.Append(';');
						}
						else { // No comment following
							newCode.Append(";\n").Append(tab * indent);
						}
					}
					else { // if inside ( )
						newCode.Append("; ");
					}
					addChar = false;
				}
			}
			else if (ii == '(') {
				++bracketRound;
				addChar = false;
				newCode.TrimEnd();
				newCode.Append(" (");
				for (; IsEmptySpace(codeFormatted[i + 1]); ++i); // Ignore all white space
			}
			else if (ii == ')') {
				// TODO
				--bracketRound;
				addChar = false;
				newCode.TrimEnd();
				for (; IsEmptySpace(codeFormatted[i + 1]); ++i);
				if (bracketRound < 1) {
					newCode.Append(')');
					if (bracketKeywordBrackets) {
						bracketKeywordBrackets = false;
						if (codeFormatted[i + 1] != '{') { // If we're at the end of an if () or for() or something like that
							++indent;
							newCode.Append("\n" + tab * indent);
							++noBracketBlock;
						}
					}
					else {
						newCode.Append(' ');
					}
				}
				else {
					newCode.Append(")");
				}
			}
			else if (ii == '{') {
				++bracketCurly;
				++indent;
				addChar = false;
				newCode.TrimEnd();
				newCode.Append(" {\n");
				newCode.Append(tab * indent);
			}
			else if (ii == '}') {
				--bracketCurly;
				--indent;
				addChar = false;
				newCode.TrimEnd();
				newCode.Append("\n").Append(tab * indent).Append("}\n");
			}
			else if (IsEmptySpace(ii)) {
				if (IsEmptySpace(lastChar)) {
					addChar = false;
				}
			}
		}
		else if (preprocessorLine && !openComment && !openOneLineComment && !openQuotes && !openChar) {

		}

		// Add the char to the new code string if needed
		if (addChar) {
			if (lastChar == '\n') { // if we just moved to a new line
				newCode.Append(tab * indent);
			}
			newCode.Append(ii);
		}
	}
	codeFormatted = newCode.Trim().Append('\n');

	//String newCode; // New code stored here;
	//bool preprocessorLine = false;
	//bool openQuotes = false;
	//bool openChar = false;
	//bool openOneLineComment = false;
	//bool openComment = false;
	//char lastChar = '\n'; // Last char added to the new code
	//bool addChar = true; // Should the loop add a char
	//char lastCharbst = '\n'; // Last char before spaces and tabs
	//char ii = 0; // Current char
	//int indent = 0;
	//int opened;
	//bool openIf = false;
	//int bracketRound = 0;
	//int bracketCurly = 0;
	//bool shouldTrim = true;
	//size_t iskeyword = 0;
	//size_t isnumber = 0;
	//size_t isoperator = 0;
	//size_t isbracket = 0;
	//String tab = "\t";

	//// Actual formatting
	//for (size_t i = 0; i < code.Size(); ++i) {
	//	ii = code[i];
	//	addChar = true;
	//	iskeyword = longestMatch(code.Get() + i, specialkeywords);
	//	isnumber = isNumber(code.Get() + i);
	//	isoperator = longestMatch(code.Get() + i, operators, false);
	//	isbracket = isInVector(code.Get() + i, brackets, false);
	//	lastChar = newCode[newCode.Size() - 1];


	//	if (ii == '#' && lastChar == '\n') { // Preprocessor line
	//		preprocessorLine = true;
	//	}
	//	else if (lastChar == '\n') {
	//		newCode.Append(tab * indent);
	//		shouldTrim = false;
	//	}
	//	else if (ii == '/' && !openQuotes) { // Comments
	//		if (code[i + 1] == '/' && !openOneLineComment)
	//			openOneLineComment = true; // Open one line comment
	//		else if (code[i + 1] == '*' && !openComment)
	//			openComment = true; // Open multiline comment
	//		else if (lastChar == '*' && openComment)
	//			openComment = false; // Close multiline comment
	//	}
	//	else if (ii == '"' && !openOneLineComment && !openComment) { // Quotes
	//		if (!openQuotes && lastChar != '\\')
	//			openQuotes = true; // Open Quotes
	//		else if (openQuotes && lastChar != '\\')
	//			openQuotes = false; // Close Quotes
	//	}
	//	else if (ii == '\'' && !openQuotes && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes) { // Char
	//		if (!openChar && lastChar != '\\')
	//			openChar = true; // Open char
	//		else if (openChar && lastChar != '\\')
	//			openChar = false; // Close char
	//	}
	//	else if (ii == '\n') { // New line
	//		if (preprocessorLine) {
	//			preprocessorLine = false; // Close preprocessor line
	//		}
	//		if (openOneLineComment) {
	//			openOneLineComment = false; // Close one line comment
	//		}
	//		if (bracketRound > 0) {
	//			addChar = false;
	//		}
	//	}
	//	else if (!VarChar(lastChar) && iskeyword != (size_t)-1 && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes) { // Keywords
	//		i += specialkeywords[iskeyword].Size() - 1;
	//		newCode.Append(specialkeywords[iskeyword]);
	//		lastChar = '0';
	//		ii = ' ';
	//		if (specialkeywords[iskeyword] != "else" && specialkeywords[iskeyword] != "try") { // Has no ()
	//			openIf = true;
	//		}
	//	}
	//	else if (ii == '{' && !openQuotes && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes) {
	//		// TODO
	//		newCode.TrimEnd();
	//		newCode.Append("\n{\n");
	//		if (bracketCurly < 1) {
	//			indent++;
	//		}

	//		++bracketCurly;
	//		addChar = false;
	//	}
	//	else if (ii == '}' && !openQuotes && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes) {
	//		newCode.TrimEnd();
	//		newCode.Append("\n}\n");
	//		--bracketCurly;
	//		if (bracketCurly < 1) {
	//			newCode.Append('\n');
	//		}
	//		addChar = false;
	//	}
	//	else if (isoperator != (size_t)-1 && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes) { // Operators
	//		if (operators[isoperator] == ';') {
	//			newCode.TrimEnd();
	//			newCode.Append(';');
	//			if (bracketRound == 0)
	//			newCode.Append('\n');
	//			while (IsEmptySpace(code[i + 1]))
	//				++i;
	//			addChar = false;
	//		}
	//		//else if (operators[isoperator] == "++" || operators[isoperator] == "--") {
	//		//	
	//		//}
	//		else {
	//			if (!IsEmptySpace(lastChar)) {
	//				newCode.Append(' ');
	//			}
	//			newCode.Append(operators[isoperator]);
	//			i += operators[isoperator].Size() - 1;
	//			ii = ' ';
	//		}
	//	}
	//	else if (ii == '\t' && !openComment && !openOneLineComment && !openQuotes) { // Clean up extra spaces on preprocessor lines
	//		ii = ' ';
	//	}
	//	else if (ii == '(' && !openComment && !openOneLineComment && !openQuotes && !preprocessorLine) { // (
	//		++bracketRound;
	//	}
	//	else if (ii == ')' && !openComment && !openOneLineComment && !openQuotes && !preprocessorLine) { // )
	//		--bracketRound;
	//		if (bracketRound < 1 && openIf) {
	//			openIf = false;
	//			newCode.Append(")\n");
	//			addChar = false;
	//		}
	//	}
	//	//else if ((ii == ' ' || ii == '\t') && !openComment && !openOneLineComment && !openQuotes && !preprocessorLine) { // Clean up spaces and tabs
	//	//	//addChar = false;
	//	//}

	//	// Remove extra pauses, tabs, new lines

	//	if (ii == ' ' && lastChar == ' ') // Rmove extra spaces
	//		addChar = false;

	//	if (addChar) {
	//		newCode.Append(ii);
	//	}
	//	if (ii != ' ')
	//		lastCharbst = ii;
	//}
	//code = newCode.TrimEnd().Append('\n');
}

void CodeFormatter::Html() {
	LoadConfig();
	ChangeNewLines("LF");
	// Generate the html styles
	html = "<html>\n<head>\n<style>\n";

	String style;
	// Tabs
	html.Append(".tab { display:inline-block; margin-left: 40px; }\n");
	// Comments
	style = config.GetLine(config.Index("comment_formatting")).ReplaceFromTo("comment_formatting", "=", "").RemoveAll("\"");
	html.Append(".comment { " + style + " }\n");
	// Keyword
	style = config.GetLine(config.Index("keyword_formatting")).ReplaceFromTo("keyword_formatting", "=", "").RemoveAll("\"");
	html.Append(".keyword { " + style + " }\n");
	// String
	style = config.GetLine(config.Index("string_formatting")).ReplaceFromTo("string_formatting", "=", "").RemoveAll("\"");
	html.Append(".string { " + style + " }\n");
	// Preprocessor
	style = config.GetLine(config.Index("preprocessor_formatting")).ReplaceFromTo("preprocessor_formatting", "=", "").RemoveAll("\"");
	html.Append(".preprocessor { " + style + " }\n");
	// Number
	style = config.GetLine(config.Index("number_formatting")).ReplaceFromTo("number_formatting", "=", "").RemoveAll("\"");
	html.Append(".number { " + style + " }\n");
	// Char
	style = config.GetLine(config.Index("char_formatting")).ReplaceFromTo("char_formatting", "=", "").RemoveAll("\"");
	html.Append(".char { " + style + " }\n");
	// Operator
	style = config.GetLine(config.Index("operator_formatting")).ReplaceFromTo("operator_formatting", "=", "").RemoveAll("\"");
	html.Append(".operator { " + style + " }\n");
	// Brackets
	style = config.GetLine(config.Index("bracket_formatting")).ReplaceFromTo("bracket_formatting", "=", "").RemoveAll("\"");
	html.Append(".bracket { " + style + " }\n");
	// Body
	style = config.GetLine(config.Index("body_formattting")).ReplaceFromTo("body_formattting", "=", "").RemoveAll("\"");
	html.Append("body { " + style + "  }\n");
	// Raw CSS
	style = config.GetLine(config.Index("raw_css")).ReplaceFromTo("raw_css", "=", "").RemoveAll("\"");
	html.Append(style);

	html.Append("</style>\n</head>\n<body>\n");
	// End of generating style

	// html snippets
	String comment = "<span class=comment>";
	String keyword = "<span class=keyword>";
	String string = "<span class=string>";
	String preprocessor = "<span class=preprocessor>";
	String number = "<span class=number>";
	String charr = "<span class=char>";
	String bracket = "<span class=bracket>";
	String operatorr = "<span class=operator>";
	String tab = "<span class=tab></span>";
	String end = "</span>";


	// Actual code coloring
	bool preprocessorLine = false;
	bool openQuotes = false;
	bool openChar = false;
	bool openOneLineComment = false;
	bool openComment = false;
	bool openLessThan = false;
	char lastChar = '\n';
	bool addChar = true;
	char ii = 0; // Current char
	size_t iskeyword = 0;
	size_t isnumber = 0;
	size_t isoperator = 0;
	size_t isbracket = 0;
	for (size_t i = 0; i < codeFormatted.Size(); ++i) {
		addChar = true;
		ii = codeFormatted[i];
		iskeyword = isInVector(codeFormatted.Get() + i, keywords);
		isnumber = isNumber(codeFormatted.Get() + i);
		isoperator = isInVector(codeFormatted.Get() + i, operators, false);
		isbracket = isInVector(codeFormatted.Get() + i, brackets, false);
		if (ii == '/' && !openQuotes && !openChar) {
			if (codeFormatted[i + 1] == '/' && !openOneLineComment) { // One line comment
				html.Append(comment);
				openOneLineComment = true;
			}
			else if (codeFormatted[i + 1] == '*' && !openComment) { // Multiline comment
				html.Append(comment);
				openComment = true;
			}
			else if (lastChar == '*' && openComment) { // Close multiline comment
				html.Append("/").Append(end);
				addChar = false;
				openComment = false;
			}
		}
		else if (ii == '"' && !openOneLineComment && !openComment && !openChar) {
			if (!openQuotes && lastChar != '\\') { // Open Quotes
				html.Append(string);
				openQuotes = true;
			}
			else if (openQuotes) {
				if (lastChar != '\\' || (lastChar == '\\' && codeFormatted[i - 2] == '\\'))
					openQuotes = false; // Close Quotes
				html.Append("\"").Append(end);
				addChar = false;
			}
		}
		else if (ii == '<' && !openOneLineComment && !openComment && preprocessorLine && !openLessThan) { // Open < .. > for preprocessor lines
			html.Append(string);
			openLessThan = true;
		}
		else if (ii == '>' && !openOneLineComment && !openComment && preprocessorLine && openLessThan) { // Close < .. > for preprocessor lines {
			html.Append(">").Append(end);
			addChar = false;
			openLessThan = false;
		}
		else if (!openQuotes && ii == '\'' && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes) {
			if (!openChar && lastChar != '\\') { // Open char
				html.Append(charr);
				openChar = true;
			}
			else if (openChar) {
				if (lastChar != '\\' || (lastChar == '\\' && codeFormatted[i - 2] == '\\'))
					openChar = false; // Close char
				html.Append("\'").Append(end);
				addChar = false;
			}
		}
		else if (ii == '#' && lastChar == '\n') { // preprocessor
			preprocessorLine = true;
			html.Append(preprocessor);
		}
		else if (ii == '\n') { // New line
			if (preprocessorLine) { // Close preprocessor line
				html.Append(end);
				preprocessorLine = false;
			}
			if (openOneLineComment) { // Close one line comment
				html.Append(end);
				openOneLineComment = false;
			}
			html.Append("<br />");
		}
		else if (!VarChar(lastChar) && iskeyword != (size_t)-1 && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes) { // Keywords
			html.Append(keyword);
			html.Append(keywords[iskeyword]);
			i += keywords[iskeyword].Size() - 1;
			addChar = false;
			html.Append(end);
		}
		else if (isoperator != (size_t)-1 && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes) { // Operators
			html.Append(operatorr);
			html.Append(operators[isoperator]);
			i += operators[isoperator].Size() - 1;
			addChar = false;
			html.Append(end);
		}
		else if (!VarChar(lastChar) && isnumber < (size_t)-1 && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes) { // Numbers
			html.Append(number);
			for (size_t j = 0; j < isnumber; ++j)
				html.Append(codeFormatted[i + j]);
			i += isnumber - 1;
			addChar = false;
			html.Append(end);
		}

		ii = codeFormatted[i];
		if (addChar) { // Add the appropriate character from the string if need replace with something appropriate
			if (ii == ' ')
				html.Append("&nbsp;");
			else if (ii == '\t')
				html.Append(tab);
			else if (ii == '<')
				html.Append("&lt;");
			else if (ii == '>')
				html.Append("&gt;");
			else if (isbracket != (size_t)-1 && !preprocessorLine && !openComment && !openOneLineComment && !openQuotes)
				html.Append(bracket).Append(ii).Append(end);
			else
				html.Append(ii);
		}
		lastChar = codeFormatted[i];
	}

	html.Append("</body>\n</html>\n");
}

size_t CodeFormatter::isInVector(const char * Str, Vector<String>& Vect, bool VarCharAfter) {
	for (size_t i = 0; i < Vect.Size(); ++i) {
		if (VarCharAfter) {
			if (StrEqualsWithVarCharAfter(Vect[i].Get(), Str, Vect[i].Size()))
				return i;
		}
		else {
			if (StrBeginsWith(Vect[i].Get(), Str, Vect[i].Size()))
				return i;
		}
	}
	return -1;
}

size_t CodeFormatter::longestMatch(const char * Str, Vector<String>& Vect, bool VarCharAfter) {
	size_t longest = -1;
	for (size_t i = 0; i < Vect.Size(); ++i) {
		if (VarCharAfter) {
			if (StrEqualsWithVarCharAfter(Vect[i].Get(), Str, Vect[i].Size()))
				if (longest == (size_t)-1) {
					longest = i;
				}
				else {
					if (Vect[i].Size() > Vect[longest].Size())
						longest = i;
				}
		}
		else {
			if (StrBeginsWith(Vect[i].Get(), Str, Vect[i].Size()))
				if (longest == (size_t)-1) {
					longest = i;
				}
				else {
					if (Vect[i].Size() > Vect[longest].Size())
						longest = i;
				}
		}
	}
	return longest;
}

size_t CodeFormatter::isNumber(const char * Str) {
	size_t dot = -1;
	size_t e = -1;
	size_t i = 0;

	bool leadingZero = false; // Leading 2 zeros check
	if (Str[i] == '0')
		leadingZero = true;
	if (leadingZero && Str[1] == '0')
		return -1;
	for (; Str[i] != 0; ++i) {
		if (Str[i] == '.' && dot == (size_t)-1) {
			dot = i;
		}
		else if (Str[i] == 'e' && dot + 1 < i && e == (size_t)-1) {
			e = i;
		}
		else if (Str[i] < '0' || Str[i] > '9')
			break;
	}
	if (i == dot || i == e)
		return -1;
	if (!VarChar(Str[i]))
		return (Str[i - 1] >= '0' && Str[i - 1] <= '9') ? i : -1;
	return -1;
}

bool CodeFormatter::StrEqualsWithVarCharAfter(const char* What, const char* Where, size_t Limit) { // Returns true if the words are matched and there's not a letter or a number after the end in Where
	size_t i = 0;
	for (; i < Limit; ++i)
		if (Where[i] != What[i])
			return 0;
		else if (Where[i] == 0)
			return What[i] == 0;
		else if (What[i] == 0)
			break;
	return !VarChar(Where[i]); // || Where[i] == '(';
}

bool CodeFormatter::StrBeginsWith(const char * What, const char * Where, size_t Limit) { // Returns true if the words match
	size_t i = 0;
	for (; i < Limit; ++i)
		if (Where[i] != What[i])
			return 0;
		else if (Where[i] == 0)
			return What[i] == 0;
		else if (What[i] == 0)
			break;
	return 1;
	//return !!VarChar(Where[i]); // || Where[i] == '(';
}

bool CodeFormatter::VarChar(char Ch) {
	return  ((Ch >= 'a' && Ch <= 'z') || (Ch >= 'A' && Ch <= 'Z') || (Ch >= '0' && Ch <= '9') || Ch == '_');
}
