//Disclaimer:
//The base code for this assignment was written by Michael Chyziak and Andrew Nichol.
//Only the extra code for assignment 3 was written by Amar Masalmeh and Cem Sezer.


//Use only the following three libraries:
#include "parserClasses.h"
#include <iostream>
#include <fstream>

using namespace std;

//global variables and linked lists to be able to access them from all scopes
TokenList assigstats;
TokenList funcdecs;
int numAssignmentStatements = 0;
int numFunctionDeclarations = -1;
int numTokensParsed = 0;

//Removes all inline comments from the tokenList including the // marker
//Returns the number of comments removed
void printType(int typecode) //Used for printing the token type based on the enumerated type index
{
	string typestring = "";
	if (typecode == 0)
	{
		typestring = "Identifier";
	}
	if (typecode == 1)
	{
		typestring = "Operator";
	}
	if (typecode == 2)
	{
		typestring = "Punctuator";
	}
	if (typecode == 3)
	{
		typestring = "Keyword";
	}
	if (typecode == 4)
	{
		typestring = "Boolean";
	}
	if (typecode == 5)
	{
		typestring = "Integer Literal";
	}
	if (typecode == 6)
	{
		typestring = "Float Literal";
	}
	if (typecode == 7)
	{
		typestring = "String Literal";
	}
	if (typecode == 8)
	{
		typestring = "Unknown";
	}
	cout << " " << typestring;
}
int getTypes(TokenList &tokenList, int &typeindex) //checks for all user defined types and puts them into a table
{
	Token *ptr = tokenList.getFirst();
		while (ptr) {
			if ((ptr->getStringRep() == "class" || ptr->getStringRep() == "struct") && ptr->getNext()->getStringRep() != "")
			{
				tokenList.tableOfFunctionDetectors[typeindex] = ptr->getNext()->getStringRep();
				typeindex++;
			}
			if (ptr->getStringRep() == "typedef")
			{
				while (ptr->getStringRep() != ";" && ptr->getStringRep() != ",")
				{
					ptr = ptr->getNext();
				}
				tokenList.tableOfFunctionDetectors[typeindex] = ptr->getPrev()->getStringRep();
				typeindex++;
			}
			ptr = ptr->getNext();
		}
		return typeindex;
}
Token* getAssignmentStatements(TokenList &tokenList)
{
	Token* ptr = tokenList.getFirst(); //create a pointer pointing to the head of the full list of tokens from our cpp file
	while(ptr) //until the pointer is at the end
	{
		//if statement for checking any assignment / compound assignment operators
		if (ptr -> getStringRep() == "=" || ptr -> getStringRep() == "+=" || 
			ptr -> getStringRep() == "-=" || ptr -> getStringRep() == "*=" || 
			ptr -> getStringRep() == "/=" || ptr -> getStringRep() == "%=" || 
			ptr -> getStringRep() == "^=" || ptr -> getStringRep() == "<<=" || 
			ptr -> getStringRep() == ">>=" || ptr -> getStringRep() == "&=" || ptr -> getStringRep() == "|=")
		{
			numAssignmentStatements++;
			//since form is (Variable = Expression), only one token before the assignment operator would be taken into account
			ptr = ptr -> getPrev();
			while (ptr -> getStringRep() != ";") //until a semicolon is found
			{
				//append the token string (part of the assignment statement) to the assigstats TokenList
				assigstats.append(ptr->getStringRep()); 
				ptr = ptr -> getNext(); //move pointer to next token
			}
			assigstats.append(ptr->getStringRep()); //append semicolon to assigstats
		}
		ptr = ptr -> getNext(); //move pointer to next token
	}
		return assigstats.getFirst(); //return the head pointer so we can begin traversing through the list
}
Token* getFunctionDeclarations(TokenList &tokenList)
{
	int typeindex = 12;
	int numoffuncdetectors = getTypes(tokenList, typeindex);
	Token* ptr = tokenList.getFirst(); //create a pointer pointing to the head of the full list of tokens from our cpp file
	while(ptr) //until the pointer is at the end
	{
		if (ptr->getStringRep() == "(")
		{
			for (int i = 0; i < numoffuncdetectors; i++) 
			{
				if (ptr->getNext()->getStringRep() == tokenList.tableOfFunctionDetectors[i])
				{
					Token *checkingPtr = ptr;
					bool isPrototype = false;
					while(checkingPtr->getStringRep() != ";" && checkingPtr->getStringRep() != "{") //while not at end of declaration
					{
						checkingPtr = checkingPtr->getNext();
					}
					if (checkingPtr->getStringRep() == ";")
					{
						isPrototype = true; //if the function header is a prototype and not the beginning of a definition
					}
					///////////////////Append Line (token by token) to funcdecs Linked List////////////////////
					if (isPrototype)
					{
						while (ptr->getStringRep() != ";" && ptr->getStringRep() != "\n" && ptr->getStringRep() != "}") 
						{
							ptr = ptr->getPrev(); //go back as far as within same function declaration
						}
						ptr = ptr->getNext();
						while (ptr->getStringRep() != ";" && ptr->getStringRep() != "\n")
						{
							funcdecs.append(ptr->getStringRep()); //append the tokens relating to the function declaration to our funcdecs linked list
							ptr = ptr->getNext();
						}
						funcdecs.append(ptr->getStringRep());
						numFunctionDeclarations++; //for statistics at end of code
						break;
					}
					else
					{
						ptr = checkingPtr;
						break;
					}
					///////////////////////////////////////////////////////////////////////////////////////////
				}
			}
		}
		ptr = ptr -> getNext(); //move pointer to next token
	}
	
	return funcdecs.getFirst();
}
int removeInlineComments(TokenList &tokenList) {
	int count = 0; //counts the amount of inline comments removed
	Token* temp;
	//temp is going to be moved around the list looking for inline comments
	temp = tokenList.getFirst();
	//temp gets initialized to the first token in the list
	while (temp) {
		//this loop will run till you hit the end of the list (i.e NULL)
		if (temp->getStringRep() == "//") {
			count++;
			//count as one inline comment to delete
			Token* destroy1;
			destroy1 = temp;
			//make a token to use in destroying the //
			if (temp->getNext() != NULL) {
				//check to make sure you do not have a blank inline comment on the last line
				//deletes the comment included in the inline comment
				Token* destroy2;
				destroy2 = temp->getNext();
				temp = destroy2->getNext();
				tokenList.deleteToken(destroy1);
				tokenList.deleteToken(destroy2);
			}
			else {
				delete temp;
			}
		}
		else {
			temp = temp->getNext();

		}
	}
	return count;
}
void classifyIdentifiers(TokenList &tokenList)
{
	Token *ptr = tokenList.getFirst();
	while (ptr)
	{
		//if the token is an identifier, and the preceeding token is a type, add it to the array of identifier classifications
		if (ptr != tokenList.getFirst() && ptr->getStringType() == ensc251::T_Identifier && ptr->getPrev()->getStringType() == ensc251::T_Keyword)
		{
			//array of identifier classifications is ordered: name | type | name | type.....etc.
			tokenList.listedIdentifierTypes[tokenList.listedIdentifierTypesIndex] = ptr->getStringRep();
			tokenList.listedIdentifierTypesIndex++;
			tokenList.listedIdentifierTypes[tokenList.listedIdentifierTypesIndex] = ptr->getPrev()->getStringRep();
			tokenList.listedIdentifierTypesIndex++;
		}
		ptr = ptr->getNext();
	}
	//this portion actually goes through the linked list and updates the IdentifierType field with its detected value
	ptr = tokenList.getFirst();
	while (ptr)
	{
		if (ptr->getStringType() == ensc251::T_Identifier)
		{
			for (int i = 0; i < tokenList.listedIdentifierTypesIndex; i+=2)
			{
				if (ptr->getStringRep() == tokenList.listedIdentifierTypes[i])
				{
					ptr->setIdentifierType(tokenList.listedIdentifierTypes[i+1]);
				}
			}
		}
		ptr = ptr->getNext();
	}
	//does the same as above, but for the assigstats linked list
	Token *ptr2 = assigstats.getFirst();
	//////////////////////////Check if any token matches identifier in array and set identifierType////////////////////////////////
	while (ptr2)
	{
		if (ptr2->getStringType() == ensc251::T_Identifier)
		{
			for (int i = 0; i < tokenList.listedIdentifierTypesIndex; i+=2)
			{
				if (ptr2->getStringRep() == tokenList.listedIdentifierTypes[i])
				{
					ptr2->setIdentifierType(tokenList.listedIdentifierTypes[i+1]);
				}
			}
		}
		ptr2 = ptr2->getNext();
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
int typeMismatchChecker(TokenList &assigStats)
{
	int numErrors = 0;
	Token *ptr = assigStats.getFirst();
	while (ptr)
	{
		if (ptr -> getStringRep() == "=" || ptr -> getStringRep() == "+=" || 
			ptr -> getStringRep() == "-=" || ptr -> getStringRep() == "*=" || 
			ptr -> getStringRep() == "/=" || ptr -> getStringRep() == "%=" || 
			ptr -> getStringRep() == "^=" || ptr -> getStringRep() == "<<=" || 
			ptr -> getStringRep() == ">>=" || ptr -> getStringRep() == "&=" || ptr -> getStringRep() == "|=")
		{
			//the wanted type is the type of the variable on the left hand side of the equation
			string wantedType = ptr->getPrev()->getIdentifierType();
			ptr = ptr->getNext();
			//this next block checks if the checked token holds the same type as the wanted value to the left of the equation
			bool isInteger = ptr->getIdentifierType() == "int" || ptr->getStringType() == ensc251::T_IntegerLiteral;
			bool isDoubleOrFloat = ptr->getIdentifierType() == "float" || ptr->getIdentifierType() == "double" || ptr->getStringType() == ensc251::T_FloatLiteral;
			bool isString = ptr->getIdentifierType() == "string" || ptr->getStringType() == ensc251::T_StringLiteral;
			bool integermatch = wantedType == "int" && isInteger;
			bool doublefloatmatch = (wantedType == "double" || wantedType == "float") && isDoubleOrFloat;
			bool stringmatch = wantedType == "string" && isString;
			
			while (ptr->getStringRep() != ";" && ptr->getStringRep() != ",") //while we're still looking at the same assignment statement
			{
				//if nothing matches
				if (!integermatch && !doublefloatmatch && !stringmatch)
				{
					numErrors++;
					break; //breaking here is okay, because we won't stop looking until we hit another equals sign
				}
				ptr = ptr->getNext();
			}
		}
		ptr = ptr->getNext();
	}
	return numErrors;
}
int bracketMismatchChecker(TokenList &assigstats)
{
	int numErrors;
	int netRoundBrackets = 0;
	int netCurlyBrackets = 0;
	int netSquareBrackets = 0;
	Token *ptr = assigstats.getFirst();
	while (ptr)
	{
		//increment every time a bracket is opened, decrement every time a bracket is closed (of the same type)
		if (ptr->getStringRep() == "(") {netRoundBrackets++;}
		if (ptr->getStringRep() == ")") {netRoundBrackets--;}
		if (ptr->getStringRep() == "{") {netCurlyBrackets++;}
		if (ptr->getStringRep() == "}") {netCurlyBrackets--;}
		if (ptr->getStringRep() == "[") {netSquareBrackets++;}
		if (ptr->getStringRep() == "]") {netSquareBrackets--;}
		ptr = ptr->getNext();
	}
	numErrors = netRoundBrackets + netCurlyBrackets + netSquareBrackets; //number of errors is the total number of brackets opened, but not closed
	return numErrors;
}


//Remove all block comments from the tokenList including /* and */ markers
//Returns the number of block comments removed (where each pair of /* and */ count as one comment)
int removeBlockComments(TokenList &tokenList) {
	//this funtion deletes the block comments in the tokenlist
	int count = 0;
	bool deleted = false;
	Token* temp, *destroy;
	temp = tokenList.getFirst();
	while (temp) {
		//loop check if list is empty
		deleted = false;
		if (temp->getStringRep() == "/*") {
			//upon finding a block entry comment you keep deleating tokens till you find the exit token
			count++;
			while (!deleted) {
				destroy = temp;
				temp = temp->getNext();
				tokenList.deleteToken(destroy);
				if (temp->getStringRep() == "*/") {
					//once the exit block token is found stop delete looping and continue searching through the list for block entry symbols
					destroy = temp;
					temp = temp->getNext();
					deleted = true;
					tokenList.deleteToken(destroy);
				}
			}
		}
		else {
			temp = temp->getNext();
		}
	}
	return count;
}


////Input: a list of tokens
////Output: head pointer to the list of assignment statements
////NOTE: Assignment statement must end with a semi-colon
////@ description: extract all the assignment statements from input token list, prepare a new token list (assignment list)
////using extracted statements and return the head pointer to it
//Token* getAssignmentStatements(TokenList &tokenList) { }

//Example Test code for interacting with your Token, TokenList, and Tokenizer classes
//Add your own code to further test the operation of your Token, TokenList, and Tokenizer classes
int main() {
	ifstream sourceFile;
	TokenList tokens;
	Tokenizer tokenizer;

	// Setting default function detectors to standard types and const, short long, signed, unsigned qualifiers
	tokens.tableOfFunctionDetectors[0] = ")";
	for (int i = 1; i <= 11; i++)
	{
		tokens.tableOfFunctionDetectors[i] = ensc251::tableOfKeywords[i];
	}
	tokens.tableOfFunctionDetectors[11] = "string";
	int typeindex = 12;


	//Read in a file line-by-line and tokenize each line
	sourceFile.open("test.cpp");
	if (!sourceFile.is_open()) {
		cout << "Failed to open file" << endl;
		return 1;
	}

	while (!sourceFile.eof()) {
		string lineA, lineB;

		getline(sourceFile, lineA);
		//while the current line ends with a line-continuation \ append the next line to the current line
		while (lineA.length() > 0 && lineA[lineA.length() - 1] == '\\') {
			lineA.erase(lineA.length() - 1, 1);
			getline(sourceFile, lineB);
			lineA += lineB;
		}
		tokenizer.setString(&lineA);
		while (!tokenizer.isComplete()) {
			tokens.append(tokenizer.getNextToken());
		}
		//Re-insert newline that was removed by the getline function
		tokens.append("\n");
	}

	//comment out for keeping comments in code
	cout << "Inline comments removed: " << removeInlineComments(tokens) << endl;
	cout << "Block comments removed: " << removeBlockComments(tokens) << endl;

	/*Test your tokenization of the file by traversing the tokens list and printing out the tokens*/
	Token *t = tokens.getFirst();
	cout << "************************* Tokens Printed With Types *************************" << endl << endl << endl;
		while (t) {
			cout << t->getStringRep() << " ";
			if (t->getStringRep() != "\n")
			{
			printType(t->getStringType()); //--------comment at beginning of line to remove token types after each token--------------
			cout << " ";
			}
			t = t->getNext();
			numTokensParsed++; //Counts how many tokens there are in the whole cpp file
		}


	//Test your assignment statements (prints assignment statements at the end)
	cout << endl << endl << "*************************** Assignment Statements ***************************" << endl << endl << endl;
	Token *aListPtr = getAssignmentStatements(tokens);
	classifyIdentifiers(tokens);
	while(aListPtr) {
		cout << aListPtr->getStringRep() << " " << aListPtr->getIdentifierType();
		if (aListPtr->getStringRep() == ";") //separate assignment statements by new lines
		{ 
			cout << endl;
		}
		aListPtr = aListPtr->getNext();
	}

	cout << endl << endl << "*************************** Function Declarations ***************************" << endl << endl << endl;
	Token *FuncListPtr = getFunctionDeclarations(tokens);
	while(FuncListPtr) {
		cout << FuncListPtr->getStringRep() << " ";
		if (FuncListPtr->getStringRep() == ";") //separate assignment statements by new lines
		{ 
			cout << endl;
		}
		FuncListPtr = FuncListPtr->getNext();
	}

	cout << endl << endl << "********************************** Errors ***********************************" << endl << endl;
	cout << "There are: " << typeMismatchChecker(assigstats) << " type mismatch error(s).\n";
	cout << "There are: " << bracketMismatchChecker(assigstats) << " bracket mismatch error(s).\n";

	int choice = 0;
	cout << endl << endl << endl << endl << "********************* Welcome to Amar and Cem's Parser **********************" << endl;
	cout << "To begin, choose a mode: " << endl << "(1) Non-Verbose" << endl << "(2) Verbose" << endl << endl;
	cout << "Choice: ";
	while (!(cin >> choice) || (choice < 1 || choice > 2))
	{
		//if an invalid character is read in
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),'\n');
		cout << "Invalid entry; please try again: " << endl << "Choice: ";
	}
	if (choice == 1)
	{
		cout << endl << "The number of assignment statements is: " << numAssignmentStatements;
		cout << endl << "The number of function declarations is: ";
		if (numFunctionDeclarations == -1) {numFunctionDeclarations = 0;}
		cout << numFunctionDeclarations;
		cout << endl << "The number of tokens parsed is: " << numTokensParsed;
		cout << endl << "The number of each type of token is: " << endl;
		for (int i = 5; i < typeindex+2; i++)
		{
			cout << tokens.tableOfFunctionDetectors[i] << endl;
		}
		for (int i = 5; i < typeindex+2; i++)
		{
			cout << tokens.tableOfFunctionDetectors[i] << ": ";
			int countofspecifictoken = 0;
			Token *t = tokens.getFirst();
			while (t)
			{
			//checking if current token is of same type we're looking for
			bool isInteger = t->getIdentifierType() == "int" || t->getStringType() == ensc251::T_IntegerLiteral;
			bool isDoubleOrFloat = t->getIdentifierType() == "float" || t->getIdentifierType() == "double" || t->getStringType() == ensc251::T_FloatLiteral;
			bool isString = t->getIdentifierType() == "string" || t->getStringType() == ensc251::T_StringLiteral;
			bool integermatch = tokens.tableOfFunctionDetectors[i] == "int" && isInteger;
			bool doublefloatmatch = (tokens.tableOfFunctionDetectors[i] == "double" || tokens.tableOfFunctionDetectors[i] == "float") && isDoubleOrFloat;
			bool stringmatch = tokens.tableOfFunctionDetectors[i] == "string" && isString;
				if (integermatch || doublefloatmatch || stringmatch)
				{
					//if it is of the same type, increase the count of that specific type
					countofspecifictoken++;
				}
				t = t->getNext();
			}
			cout << countofspecifictoken << endl;
		}
	}
	if (choice == 2)
	{
		cout << endl << "The number of assignment statements is: " << numAssignmentStatements;
		cout << endl << "The number of function declarations is: " << numFunctionDeclarations;
		cout << endl << "The number of tokens parsed is: " << numTokensParsed;
		cout << endl << "The tokens of each type are as follows: " << endl;
		for (int i = 5; i < typeindex+2; i++)
		{
			cout << tokens.tableOfFunctionDetectors[i] << ": \n";
			int countofspecifictoken = 0;
			Token *t = tokens.getFirst();
			while (t)
			{
			//checks if the current token is of the same type we're looking for
			bool isInteger = t->getIdentifierType() == "int" || t->getStringType() == ensc251::T_IntegerLiteral;
			bool isDoubleOrFloat = t->getIdentifierType() == "float" || t->getIdentifierType() == "double" || t->getStringType() == ensc251::T_FloatLiteral;
			bool isString = t->getIdentifierType() == "string" || t->getStringType() == ensc251::T_StringLiteral;
			bool integermatch = tokens.tableOfFunctionDetectors[i] == "int" && isInteger;
			bool doublefloatmatch = (tokens.tableOfFunctionDetectors[i] == "double" || tokens.tableOfFunctionDetectors[i] == "float") && isDoubleOrFloat;
			bool stringmatch = tokens.tableOfFunctionDetectors[i] == "string" && isString;
			if ((integermatch || doublefloatmatch || stringmatch) && t->getIdentifierType() != "double")
				{
					//if it is of the same type, print it out to the screen with commas as separation
					cout << t->getStringRep() << ", ";
				}
				t = t->getNext();
			}
			cout << endl;
		}
	}

	cout << endl;
	return 0;
}
