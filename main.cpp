#include "AnagramGenerator.h"

#include <algorithm>
#include <iostream>

#define MAX_WORD_LENGTH 50

using namespace std;

static const int MAX_INPUT_LENGTH = 100;

int main( int argc, char** argv )
{
	ANAGRAM_GENERATOR TheGenerator;

	cout << "\nTHIS IS ANAGRAM GENERATOR!\n\n";

	// read in dictionary words from text file
	TheGenerator.ReadInDictionary( "words50.txt" );

	// get input sentence from user
	cout << "\nEnter sentence: \n";

	char buffer[MAX_INPUT_LENGTH];
	cin.getline( buffer, MAX_INPUT_LENGTH );

	// generate list of anagram sentences
	TheGenerator.GenerateAnagrams( string( buffer ) );

	// print list of anagrams
	TheGenerator.PrintAnagrams();

	
	return 0;
}