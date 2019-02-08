#include "AnagramGenerator.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string.h>

using namespace std;

static const string DICTIONARY_FILENAME = "words50.txt";

static const int MAX_WORD_LENGTH = 50;
static const int MAX_INPUT_LENGTH = 100;

enum COMMANDS 
{
	NONE,
	INCLUDE,
	UNINCLUDE,
	IGNORE,
	UNIGNORE,
	GENERATE,
	QUIT,
};

static COMMANDS CommandStringToEnum( string str )
{
	if ( str == "include" )		return INCLUDE;
	if ( str == "uninclude" )	return UNINCLUDE;
	if ( str == "ignore" )		return IGNORE;
	if ( str == "unignore" )	return UNIGNORE;
	if ( str == "generate" )	return GENERATE;
	if ( str == "quit" )		return QUIT;

	return NONE;
}


static vector<string> TokenizeInput( string input )
{
	vector<string> tokens;
	stringstream lineStream( input );
	string temp;

	while ( getline( lineStream, temp, ' ' ) )
	{
		tokens.push_back( temp );
	}

	return tokens;
}

int main( int argc, char** argv )
{
	cout << "\nTHIS IS ANAGRAM GENERATOR!\n\n";

	// create our anagram generator object
	ANAGRAM_GENERATOR TheGenerator( DICTIONARY_FILENAME );

	// get input sentence from user
	cout << "\nEnter sentence: \n";
	char buffer[MAX_INPUT_LENGTH];
	cin.getline( buffer, MAX_INPUT_LENGTH );

	// populate our letter bank and list of spellable words
	TheGenerator.FindSpellableWords( string( buffer ) );

	// *** MAIN PROGRAM LOOP ***
	while ( 1 ) 
	{
		cout << "Enter command:\n";

		memset( buffer, 0, MAX_INPUT_LENGTH );
		cin.getline( buffer, MAX_INPUT_LENGTH );
		vector<string> tokens = TokenizeInput( string( buffer ) );

		if ( tokens.size() < 1 ) {
			cout << "Invalid Input\n";
			continue;
		}

		COMMANDS command = CommandStringToEnum( tokens[0] );
		vector<string> args = tokens;
		args.erase( args.begin() );

		switch ( command )
		{
		case INCLUDE: 
			{
				// include [word]		- add [word] to list of words we want included in the generated anagrams
				for ( int i = 0; i < args.size(); ++i ) {
					TheGenerator.AddIncludedWord( args[i] );
				}
			} break;

		case UNINCLUDE:
			{
				// uninclude [word]		- removes [word] from the list of words we want included in the generated anagrams
				// uninclude -all		- removes all words from the list of words we want included in the generated anagrams
			} break;

		case IGNORE:
			{
				// ignore [word]		- adds [word] to the list of words we DON'T want included in the generated anagrams
				// ignore -all			- adds all words to the list of words we DON'T want included in the generated anagrams
			} break;

		case UNIGNORE:
			{
				// unignore [word]		- removes [word] from the list of words we DON'T want included in the generated anagrams
				// unignore -all		- removes all words from the list of words we DON'T want included in the generated anagrams
			} break;

		case GENERATE:
			{
				// generate				- generates a list of anagram sentences that include the words in the included word list and do not include any words in the ignored word list
			} break;

		case QUIT:
			{
				// quit					- exits the program
				if ( args.size() == 0 ) {
					cout << "\nMischief Managed...\n\n";
					return 0;
				} else {
					cout << "ERROR: Invalid number of args provided for command: quit\n";
				}
			} break;

		default:
			{
				cout << "ERROR: Unrecognized command\n";
			} break;
		}
	}
	
	return 0;
}