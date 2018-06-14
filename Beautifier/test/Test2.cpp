void CodeFormatter::Beautify() {
  String newCode; // New code stored here;
  String tab = "\t";
  // Comment
  for (size_t i = 0; i < codeFormatted.Size(); ++i) {
    ii = codeFormatted[i];
    // Comments
    if (ii == '/' && !openQuotes) {
      if (codeFormatted[i + 1] == '/' && !openOneLineComment)
        openOneLineComment = true; // Open one line comment
      else if (codeFormatted[i + 1] == '*' && !openComment)
        openComment = true; // Open multiline comment
      else
        openComment = false; // Close multiline comment
    }
    // Quotes
    else if (ii == '"' && !openOneLineComment && !openComment) {
      if (!openQuotes && lastChar != '\\')
        openQuotes = true; // Open Quotes
      else if (openQuotes && lastChar != '\\')
        openQuotes = false; // Close Quotes
    } 
	else {
      if ((a == 5) || (a == 6))
        if (b == 3)
          flag = false;
        else if (b == 4)
          flag = true;
        else if (b == 2) {
          flag = NULL;
        } 
		else
          flag = 2;
      else if (a == 2)
        flag = 1;
      else {
        flag = 0;
      }
    }
  }
}

void main() {
  int x, y;
  if (1 > 2)
    return;
  if (1 > 2)
    return;
  else {
    int x, y;
    int z;
  }
}