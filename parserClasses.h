#ifndef PARSERCLASSES_H_
#define PARSERCLASSES_H_

//Use only the following libraries:
#include <string>
#include "lexemesTypes.h"

using namespace std;

//Declare your variables for storing delimiters here:


//typedef for custom data type defined in header file "lexemesTypes.h"
typedef ensc251::LexemeType TokenClass;

//Token class for a doubly-linked list of string tokens
class Token {
private:
	Token *next; //Next pointer for doubly linked list
	Token *prev; //Previous pointer for doubly linked list
	string stringRep; //Token value
	string identifierType; //if it's an identifier, check what data type it is of
	TokenClass stringType; //Token type (to which class this token belongs to, for e.g. identifier, keywords...etc)
	bool comment; //true if this token is a comment

	//Allow TokenList class to access Token member variables marked private
	friend class TokenList;

public:
	//Default Constructor, pointers initialized to NULL
	Token() : next(NULL), prev(NULL) { }

	//Constructor with string initialization, pointers initialized to NULL
	Token(const string &stringRep) : next(NULL), prev(NULL), stringRep(stringRep) { }

	//Returns the Token's identifierType member
	string getIdentifierType() const {
		return identifierType;
	}

	//Sets the Token's identifierType member
	void setIdentifierType(string identifierType) {
		this->identifierType = identifierType;
	}

	//Returns the Token's *next member 
	Token* getNext() const {
		return next;
	}

	//Sets the Token's *next member
	void setNext(Token* next) {
		this->next = next;
	}

	//Returns the Token's *prev member 
	Token* getPrev() const {
		return prev;
	}

	//Sets the Token's *prev member
	void setPrev(Token* prev){
		this->prev = prev;
	}

	//Returns a reference to the Token's stringRep member variable
	const string& getStringRep() const {
		return stringRep;
	}

	//Sets the token's stringRep variable
	void setStringRep(const string& stringRep) {
		this->stringRep = stringRep;
	}

	//Returns a reference to the Token's stringType member variable
	const TokenClass getStringType ( ) const { return stringType; }

	//Sets the token's stringType variable
	void setStringType (const TokenClass& stringType ) { this->stringType = stringType; }
};

//A doubly-linked list class consisting of Token elements
class TokenList {
private:
	Token *head; //Points to the head of the token list (doubly linked)
	Token *tail; //Points to the tail of the function list (doubly linked)

public:
	//Default Constructor, Empty list with pointers initialized to NULL
	TokenList() : head(NULL), tail(NULL), tableOfFunctionDetectors(new string[100]), listedIdentifierTypes(new string[100]),
				  listedIdentifierTypesIndex(0) { }

	//Returns a pointer to the head of the list
	Token* getFirst() const {
		return head;
	}

	string *tableOfFunctionDetectors; //list of possible things after a bracket that ensure a function declaration
	string *listedIdentifierTypes; //list of all identifiers with their types; tabulated
	int listedIdentifierTypesIndex; //how many identifiers there are (x2 with their types in the neighboring field)

	//Returns a pointer to the tail of the list
	Token* getLast() const {
		return tail;
	}

	//Creates a new token for the string input, str
	//Appends this new token to the TokenList
	//On return from the function, it will be the last token in the list
	void append(const string &str); //example comment

	//Appends the token to the TokenList if not null
	//On return from the function, it will be the last token in the list
	void append(Token *token);

	//Removes the token from the linked list if it is not null
	//Deletes the token
	//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
	void deleteToken(Token *token);

	//Input: a pointer to a token
	//Output: it won't return anything, but within function, it should set the token class (i.e. token->stringType)
	//Note: one can invoke this function before adding token to the token list
	void setTokenClass(Token *token);
};

//A class for tokenizing a string of C++  code into tokens
class Tokenizer {
private:
	/*State tracking variables for processing a single string*/
	bool inlineFlag;
	bool complete; //True if finished processing the current string
	bool hashFlag; //checks if have read in an a hashtag (for include statement)
	bool blockFlag; //checks if we are inside a block comment
	bool includeFlag;

	size_t offset; //Current position in string
	size_t tokenLength; //Current token length
	string *str; //A pointer to the current string being processed

	//Include any helper functions here
	//e.g. trimming whitespace, comment processing

	//Computes a new tokenLength for the next token
	//Modifies: size_t tokenLength, and bool complete
	//(Optionally): may modify offset
	//Does NOT modify any other member variable of Tokenizer
	void prepareNextToken();

public:
	//Default Constructor- YOU need to add the member variable initializers.
	Tokenizer() {
		//set all of the variables to initial values of false, 0, or Null
		inlineFlag = false;
		blockFlag = false;
		complete = false;
		hashFlag = false;
		includeFlag = false;

		offset = 0;
		tokenLength = 0;
		str = NULL;
	}

	//Sets the current string to be tokenized
	//Resets all Tokenizer state variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	void setString(string *str);

	//Returns true if all possible tokens have been extracted from the current string (string *str)
	bool isComplete() const {
		return complete;
	}

	//Returns the next token. Hint: consider the substr function
	//Updates the tokenizer state
	//Updates offset, resets tokenLength, updates processingABC member variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	string getNextToken();
};


#endif /* PARSERCLASSES_H_ */
