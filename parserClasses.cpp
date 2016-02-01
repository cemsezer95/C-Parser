//Use only the following libraries:
#include "parserClasses.h"
#include <string>

//Complete the implementation of the following member functions:

//****TokenList class function definitions******


//Creates a new token for the string input, str
//Appends this new token to the TokenList
//On return from the function, it will be the last token in the list
void TokenList::append(const string &str) {
	//works when given a string to add to the bottom of the list
	if (str.empty()) {
		//do nothing if the given string is empty. Ignore it basically
	}
	else {
		Token *temp;
		temp = new Token(str);
		setTokenClass(temp);
		//allocates memory to make new token in the list
		if (tail) {
			tail->next = temp;
			temp->prev = tail;
			tail = temp;
			//inserts the token in the list the tail does not point to null and you have an empty list
		}
		else {
			tail = temp;
			head = temp;
			//if you have and empty list then start the new list and appended token is the list
		}
	}
}

//Appends the token to the TokenList if not null
//On return from the function, it will be the last token in the list
void TokenList::append(Token *token) {
	//add a token to the bottom of the list when a empty list does not get passed in
	if (token) {
		Token * new_tail;
		new_tail = token;
		tail->next = new_tail;
		new_tail->prev = tail;
		new_tail->next = NULL;
		tail = new_tail;
	}
	else {
		//token is NULL, do nothing
	}
}

//Removes the token from the linked list if it is not null
//Deletes the token
//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
void TokenList::deleteToken(Token *token) {
	if (token) {
		Token *temp, *after, *before;
		temp = token;
		if (head == token && tail == token) {
			delete temp;
			head = NULL;
			tail = NULL;
			//case where you only have one token in a list
		}
		else if (token->getPrev() == NULL) {
			head = head->getNext();
			head->setPrev(NULL);
			delete temp;
			//case where you are deleating the head of the list
		}
		else if (token->getNext() == NULL) {
			tail = token->getPrev();
			tail->setNext(NULL); \
				delete temp;
			//case where you are deleting the tail of the list
		}
		else {
			after = token->getNext();
			before = token->getPrev();
			before->setNext(after);
			after->setPrev(before);
			delete temp;
			//case where you are deleting a token in the middle of the list
		}

	}
	else {
		//token is null, do nothing
	}
}

//Input: a pointer to a token
//Output: it won't return anything, but within function, it should set the token class (i.e. token->stringType)
//Note: one can invoke this function before adding token to the token list
void TokenList::setTokenClass(Token *token) 
{
	using namespace ensc251;

	string stringtochar = token->getStringRep(); //makes new variable for stringRep (to be able to extract first character)
	char finalchar = stringtochar[0]; //sets first character of stringRep as its own char variable to use as isPunctuator() argument
	if (ensc251::isOperator(token->getStringRep()) == true)
	{
		token->setStringType(TokenClass::T_Operator);
	}
	else if (ensc251::isPunctuator(finalchar) == true)
	{
		token->setStringType(TokenClass::T_Punctuator);
	}
	else if (ensc251::isKeyword(token->getStringRep()) == true)
	{
		token->setStringType(TokenClass::T_Keyword);
	}
	else if (ensc251::isBooleanValue(token->getStringRep()) == true)
	{
		token->setStringType(TokenClass::T_Boolean);
	}
	else if (ensc251::isIntegerLiteral(token->getStringRep()) == true)
	{
		token->setStringType(TokenClass::T_IntegerLiteral);
	}
	else if (ensc251::isFloatLiteral(token->getStringRep()) == true)
	{
		token->setStringType(TokenClass::T_FloatLiteral);
	}
	else if (ensc251::isStringLiteral(token->getStringRep()) == true)
	{
		token->setStringType(TokenClass::T_StringLiteral);
	}
	else if (ensc251::isIdentifier(token->getStringRep()) == true)
	{
		token->setStringType(TokenClass::T_Identifier);
	}
	else
	{
		token->setStringType(TokenClass::T_Unknown);
	}
}


//****Tokenizer class function definitions******

//Computes a new tokenLength for the next token
//Modifies: size_t tokenLength, and bool complete
//(Optionally): may modify offset
//Does NOT modify any other member variable of Tokenizer
void Tokenizer::prepareNextToken(){
	if (offset == str->length()) {
		complete = true;
		//this is the case where you are at the end of the list
	}
	if (!complete) {
		bool found = false;
		int i = offset;
		int length = str->length();
		if (inlineFlag) {
			//this executes when the inline comment flag is thrown up
			tokenLength = length - offset;
			//you ALWAYS take the whole line after the // as a token 
		}
		else if (blockFlag) {
			//you have a block processing flag thrown up since you found a /*
			if (str->find("*/", i) == -1) {
				//this execute when find() returns a -1 , which is when you have a line without a block end
				tokenLength = length - offset;
				//if you do not find a */ in a line, after throwing up the flag then you take the whole line as a comment
			}
			else {
				i = str->find("*/") - offset;
				tokenLength = i;
				//if you find a block end in a line then you you make everything up to a */ is a token 
			}
		}
		else {
			while (!found && i < length) {
				switch (str->at(i)) {
				case ' ':
				case '\t':
					//covers tabs and whitespace
					tokenLength = i - offset;
					found = true;
					if (tokenLength == 0) {
						if (i == length){
							complete = true;
							found = true;
							//end of string condition, you hit the end of line and are thus complete
						}
						else {
							offset++;
							i++;
							found = false;
							//you should keep moving forward till you hit the a character of importance or you end
						}
					}
					break;
				case '(':
				case ')':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					found = true;
					break;
				case '&':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && (str->at(i + 1) == '=') && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					if ((i < length - 1) && (str->at(i + 1) == '&') && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					found = true;
					break;
					//includes cases where I find { &, &= , &&}
				case '*':
					tokenLength = i - offset;
					if ((i < length - 1) && (str->at(i + 1) == '/') && tokenLength == 0) {
						tokenLength = 2;
					}
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
					}
					found = true;
					break;
					//includes cases where I find { *= , */}
				case '!':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
						found = true;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					break;
					//includes cases where I find { !, != }
				case '"':
					while (i < length - 1 && !(str->at(i) != '\\' && str->at(i + 1) == '"')) {
						i++;
					}
					tokenLength = i + 2 - offset;
					found = true;
					break;
					//includes the case where anything between "..." is 1 token (including the quotation marks)
				case '+':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
					}
					if ((i < length - 1) && str->at(i + 1) == '+' && ((i - offset) == 0)) {
						tokenLength = 2;
					}

					if (2 <= (length) && ((i - offset) != 0) && (str->at(i - 1) == 'e' || str->at(i - 1) == 'E') && ((str->at(i - 2) == '0' || str->at(i - 2) == '1' || str->at(i - 2) == '2' || str->at(i - 2) == '3' || str->at(i - 2) == '4' || str->at(i - 2) == '5' || str->at(i - 2) == '6' || str->at(i - 2) == '7' || str->at(i - 2) == '8' || str->at(i - 2) == '9'))) {
						i++;
						break;
					}
					found = true;
					break;
					//includes cases where I find { +, +=, ++ }
				case '-':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
					}
					if ((i < length - 1) && str->at(i + 1) == '>' && ((i - offset) == 0)) {
						tokenLength = 2;
					}
					if ((i < length - 2) && (str->at(i + 1) == '>') & (str->at(i + 2) == '*') && ((i - offset) == 0)){
						tokenLength = 3;

					}
					if ((i < length - 1) && str->at(i + 1) == '-' && ((i - offset) == 0)) {
						tokenLength = 2;
					}
					if (2 <= (length) && ((i - offset) != 0) && (str->at(i - 1) == 'e' || str->at(i - 1) == 'E') && ((str->at(i - 2) == '0' || str->at(i - 2) == '1' || str->at(i - 2) == '2' || str->at(i - 2) == '3' || str->at(i - 2) == '4' || str->at(i - 2) == '5' || str->at(i - 2) == '6' || str->at(i - 2) == '7' || str->at(i - 2) == '8' || str->at(i - 2) == '9'))) {
						i++;
						break;
					}
					found = true;
					break;
					//includes cases where I find { -, -=, ->, ->*, -- }
				case '^':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					found = true;
					break;
					//includes cases where I find {^=, ^ }
				case '=':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					found = true;
					break;
					//includes cases where I find {==, = }
				case ';':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					found = true;
					break;
					//includes cases where I find {; }
				case ':':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == ':' && (offset - i == 0)) {
						tokenLength = 2;
						found = true;
					}
					found = true;
					break;
					//includes cases where I find {:,:: }
				case '/':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					if ((i < length - 1) && str->at(i + 1) == '/' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					if ((i < length - 1) && str->at(i + 1) == '*' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					//includes cases where I find {/,/=,//,/* }
					found = true;
					break;
				case '#':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					found = true;
					break;
					//includes cases where I find {#}
				case '[':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					found = true;
					break;
					//includes cases where I find {[ }
				case ']':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					found = true;
					break;
					//includes cases where I find { ] }
				case '%':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && (str->at(i + 1) == '=') && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					found = true;
					break;
					//includes cases where I find {% , %= }
				case '|':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '|' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					found = true;
					break;
					//includes cases where I find {|,||,|=}
				case '.':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '*' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					if ((i < length - 1) && (str->at(i + 1) == '0' || str->at(i + 1) == '1' || str->at(i + 1) == '2' || str->at(i + 1) == '3' || str->at(i + 1) == '4' || str->at(i + 1) == '5' || str->at(i + 1) == '6' || str->at(i + 1) == '7' || str->at(i + 1) == '8' || str->at(i + 1) == '9')) {
						i++;
						break;
					}

					if ((i > 0) && (str->at(i - 1) == '0' || str->at(i - 1) == '1' || str->at(i - 1) == '2' || str->at(i - 1) == '3' || str->at(i - 1) == '4' || str->at(i - 1) == '5' || str->at(i - 1) == '6' || str->at(i - 1) == '7' || str->at(i - 1) == '8' || str->at(i - 1) == '9' || str->at(i - 1) == ' ')) {
						if ((i < length - 1) && (str->at(i + 1) == '0' || str->at(i + 1) == '1' || str->at(i + 1) == '2' || str->at(i + 1) == '3' || str->at(i + 1) == '4' || str->at(i + 1) == '5' || str->at(i + 1) == '6' || str->at(i + 1) == '7' || str->at(i + 1) == '8' || str->at(i + 1) == '9' || str->at(i + 1) == ' ')) {
							if (i == 1) {
								i++;
								break;
							}
							if ((str->at(i - 2) == '0' || str->at(i - 2) == '1' || str->at(i - 2) == '2' || str->at(i - 2) == '3' || str->at(i - 2) == '4' || str->at(i - 2) == '5' || str->at(i - 2) == '6' || str->at(i - 2) == '7' || str->at(i - 2) == '8' || str->at(i - 2) == '9' || str->at(i - 2) == ' ')) {
								i++;
								break;
							}
						}
					}
					found = true;
					break;
					//includes cases where I find {.,.*}
				case '?':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == ':' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					found = true;
					break;
					//includes cases where I find {?,?:}
				case ',':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					found = true;
					break;
					//includes cases where I find {,}
				case '>':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					if ((i < length - 1) && str->at(i + 1) == '>' && ((i - offset) == 0)) {
						tokenLength = 2;
						found = true;
					}
					if ((i < length - 2) && (str->at(i + 1) == '>') && (str->at(i + 2) == '=') && ((i - offset) == 0)){
						tokenLength = 3;
						found = true;
					}

					found = true;
					break;
					//includes cases where I find {>, >= , >> , >>=}
				case '<':
					tokenLength = i - offset;
					if (includeFlag) {
						while ((i < length - 1) && (str->at(i + 1) != '>')) {
							i++;
						}
						tokenLength = i + 2 - offset;
					}
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					if ((i < length - 1) && str->at(i + 1) == '=' && ((i - offset) == 0)) {
						tokenLength = 2;
					}

					if ((i < length - 1) && str->at(i + 1) == '<' && ((i - offset) == 0)) {
						tokenLength = 2;
					}

					if ((i < length - 2) && (str->at(i + 1) == '<') && (str->at(i + 2) == '=') && ((i - offset) == 0)){
						tokenLength = 3;
					}
					found = true;
					break;
					//includes cases where I find {<,<= , <<= , <<}
				case '~':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					found = true;
					break;
					//includes cases where I find {~}
				case '{':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					found = true;
					break;
					//includes cases where I find { { }
				case '}':
					tokenLength = i - offset;
					if (tokenLength == 0) {
						tokenLength = 1;
					}
					found = true;
					break;
					//includes cases where I find { } }
				default:
					i++;
					//by defult if non of these cases apply, increment by one till you either reach the end of the string or you find a specified character
				}
			}
		}
		if (i == length) {
			tokenLength = length - offset;
		}
	}
}

//Sets the current string to be tokenized
//Resets all Tokenizer state variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
void Tokenizer::setString(string *str) {
	if (str->length() == 0) {
		//this is executed if a blank string is passed in 
		includeFlag = false;
		complete = true;
	}
	else {
		//if the string has characters in it then you initialize some variables and prepare the next token for the get next token funtion 
		includeFlag = false;
		complete = false;
		offset = 0;
		tokenLength = 0;
		this->str = str;
		prepareNextToken();
	}
}

//Returns the next token. Hint: consider the substr function
//Updates the tokenizer state
//Updates offset, resets tokenLength, updates processingABC member variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
string Tokenizer::getNextToken() {
	if (offset == str->length()) {
		complete = true;
		return "";
		//if at end of list return complete
	}
	string temp = str->substr(offset, tokenLength);
	//substr takes a string and outputs a string of a specified length starting at a point
	offset = offset + tokenLength;
	//move to the new position to seek forward
	tokenLength = 0;
	//reset token length
	if (inlineFlag) {
		inlineFlag = false;
	}
	if (temp == "//")
	{
		inlineFlag = true;
		//if their is a comment on a line every character after is a comment
		//inline case
	}
	if (blockFlag && offset < str->length()) {
		blockFlag = false;
		//turn off the block flag if it is on and your offsett is less then the length
		//this means a */ has been found before the end of the line
	}
	if (temp == "/*")
	{
		blockFlag = true;
		//if a block comment gets started activate the processing block comment flag
		//block line case
	}
	if (hashFlag) {
		hashFlag = false;
		//flag is turned off but a include flag is turned on if the next token is include
		//this is in anticipation of a <filename> or "filename"
		if (temp == "include") {
			includeFlag = true;
		}
	}
	if (temp == "#") {
		hashFlag = true;
		//flag to set up special case for directive command 
		//this is based upon whether the last token read in is a #
	}
	prepareNextToken();
	//get next token length for next call to get next token

	return temp;
}
