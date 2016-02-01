/*
 * lexemesTypes.cpp
 *
 * Created on: Oct 27, 2014
 *
 */

#include "lexemesTypes.h"

namespace ensc251{

  //***Complete the implementation of the following functions: ***

  //This function returns TRUE if the input is identifier else FALSE
  bool isIdentifier(const string &lexeme) 
  {
	 bool allowedcharacter = true; //allowedcharacter checks if the identifier only has valid characters (not colons, commas, etc...)
	 for (int i = 0; i < lexeme.size(); i++)
	 {
		 if (lexeme[i] < 48 || (lexeme[i] > 57 && lexeme[i] < 65) || (lexeme[i] > 90 && lexeme[i] < 97) 
			 && lexeme[i] != 95 || lexeme[i] > 122)
		 {
			 allowedcharacter = false;
		 }
	 }
	 return !(isKeyword(lexeme) || isBooleanValue(lexeme)) && ((lexeme[0] >= 64 && lexeme[0] <= 90) 
		 || (lexeme[0] >= 97 && lexeme[0] <= 122) || (lexeme[0] == 95)) && allowedcharacter; //if string is not keyword or boolean word
																							 //and contains no illegal characters
																							 //return true
  }

  //This function returns TRUE if the input is one of the operators defined in the variable
  //"tableOfOperators" in "lexemesTypes.h" else it returns FALSE
  bool isOperator(const string &lexeme) 
	{
		for (int i = 0; i < 19; i++) //straightforward search through the table for matches
		{
			if (lexeme == tableOfOperators[i])
			{
				return true;
			}
		}
		return false;
    }

  //This function returns TRUE if the input is one of the punctuators defined in the variable
  //"tableOfPunctuators" in "lexemesTypes.h" else it returns FALSE
  bool isPunctuator(char lexeme) 
	{
		for (int i = 0; i < 7; i++)
		{
			if (lexeme == tableOfPunctuators[i]) //straightforward search through the table for matches
			{
				return true;
			}
		}
		return false;
    }

  //This function returns TRUE if the input is one of the keywords defined in the variable
  //"tableOfKeywords" in "lexemesTypes.h" else it returns FALSE
  bool isKeyword(const string &lexeme) 
  {
		for (int i = 0; i < 60; i++)
		{
			if (lexeme == tableOfKeywords[i]) //straightforward search through the table for matches
			{
				return true;
			}
		}
		return false;
    }

  //This function returns TRUE if the input is one of the boolean defined in the variable
  //"booleanValues" in "lexemesTypes.h" else it returns FALSE
  bool isBooleanValue(const string &lexeme) 
  {
		for (int i = 0; i < 19; i++)
		{
			if (lexeme == booleanValues[i]) //straightforward search through the table for matches
			{
				return true;
			}
		}
		return false;
    }

  //This function returns TRUE if the input is integer literal else FALSE
  bool isIntegerLiteral(const string &lexeme) 
  {
	  int firstnotdigit = lexeme.find_first_not_of("-0123456789");
	  if (firstnotdigit != std::string::npos) //if there EXISTS a character not valid in integers, return false
	  {
		  return false;
	  }
	  else {
	  return true;
	  }
  }

  //This function returns TRUE if the input is float literal else FALSE
  bool isFloatLiteral(const string &lexeme) 
  {
	  int decimalcount = 0;
	  int firstnotfloatchar = lexeme.find_first_not_of(".0123456789Ee-+"); //includes scientific notation characters
	  if (firstnotfloatchar != std::string::npos) //if there EXISTS a character not valid in floats, return false
	  {
		  return false;
	  }
	  else
	  {
		  for (int i = 0; i < lexeme.size(); i++)
		  {
			  if (lexeme[i] == '.')
			  {
				  decimalcount++;
			  }
		  }
		  if (decimalcount > 1)
		  {
			  return false;
		  }
	  }
	  return true;
  }

  //This function returns TRUE if the input is string literal else FALSE
  bool isStringLiteral(const string &lexeme) 
  {
	if (lexeme[0] == '\"' && lexeme[lexeme.size() - 1] == '\"') //searches for quotation marks at very beginning and very end
	{
		return true;
	}
	else {
	return false;
	}
  }
}
