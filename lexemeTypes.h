/*
 * lexemesTypes.h
 *
 * Created on: Oct 27, 2014
 *
 */

#ifndef LEXEMESTYPES_H_
#define LEXEMESTYPES_H_

//Use only the following libraries:
#include <string>
#include <cctype>

using namespace std;

namespace ensc251{

  //tables
  const string tableOfOperators[]={"+","-","/","*","=","<",">","++","--","<<",">>","!=","&","&&","|","||","[","]","=="};
  const char tableOfPunctuators[]={';',',','.','(',')','{','}'};
  const string tableOfKeywords[]={"signed","const","unsigned","long","short","bool","char","double","float","void","int",
		  "auto","break","case","continue","default","do","else",
		  "enum","extern","for","goto","if","register","return","sizeof","static",
		  "struct","switch","typedef","union","volatile","while","asm","_ss","interrupt","_cs","cdecl",
		  "near","_ds","far","pascal","_es","huge","catch","class","delete","friend","inline","new","operator","overload",
		  "private","protected","public","template","this","throw","try","virtual"};
  const string booleanValues[]={"true","false"};


  //custom data type:
  enum LexemeType {
	  T_Identifier,
	  T_Operator,
	  T_Punctuator,
	  T_Keyword,
	  T_Boolean,
	  T_IntegerLiteral,
	  T_FloatLiteral,
	  T_StringLiteral,
	  T_Unknown			             //if token does not belong to above classes then place it in this category
  };

  //helper functions: for all of them
  //Input: a string value (char in case of isPunctuator() function)
  //Output: true/false depending on whether the input value is of type as denoted by the corresponding function name.
  //NOTE:- You need to DEFINE these functions in source file "lexemesTypes.cpp".
  bool isIdentifier(const string&);
  bool isOperator(const string&);
  bool isPunctuator(char);
  bool isKeyword(const string&);
  bool isBooleanValue(const string&);
  bool isIntegerLiteral(const string&);
  bool isFloatLiteral(const string&);
  bool isStringLiteral(const string&);
}

#endif /* LEXEMESTYPES_H_ */
