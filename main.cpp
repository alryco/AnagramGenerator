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
	PRINT,
	QUIT,
};

static COMMANDS CommandStringToEnum( string str )
{
	if ( str == "include" )		return INCLUDE;
	if ( str == "uninclude" )	return UNINCLUDE;
	if ( str == "ignore" )		return IGNORE;
	if ( str == "unignore" )	return UNIGNORE;
	if ( str == "generate" )	return GENERATE;
	if ( str == "print" )		return PRINT;
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
	cout << ">";
	char buffer[MAX_INPUT_LENGTH];
	cin.getline( buffer, MAX_INPUT_LENGTH );

	// populate our letter bank and list of spellable words
	TheGenerator.FindSpellableWords( string( buffer ) );

	// *** MAIN PROGRAM LOOP ***
	while ( 1 ) 
	{
		cout << "\nEnter command:\n";
		cout << ">";

		memset( buffer, 0, MAX_INPUT_LENGTH );
		cin.getline( buffer, MAX_INPUT_LENGTH );
		vector<string> tokens = TokenizeInput( string( buffer ) );

		if ( tokens.size() < 1 ) {
			cout << "Invalid Input\n";
			continue;
		}

		string commandStr = tokens[0];
		COMMANDS command = CommandStringToEnum( commandStr );
		vector<string> args = tokens;
		args.erase( args.begin() );

		switch ( command )
		{
		case INCLUDE: 
			{
				// include [word]		- add [word] to list of words we want included in the generated anagrams
				if ( !args.empty() ) {
					for ( int i = 0; i < args.size(); ++i ) {
						TheGenerator.AddIncludedWord( args[i] );
					}
				} else {
					cout << "ERROR: at least 1 arg expected for command \'" << commandStr << "\'. " << args.size() << " provided.\n";
				}
			} break;

		case UNINCLUDE:
			{
				// uninclude -all		- removes all words from the list of words we want included in the generated anagrams
				// uninclude [word]		- removes [word] from the list of words we want included in the generated anagrams
				if ( !args.empty() ) {
					if ( args.size() == 1 && args[0] == "-all" ) {
						TheGenerator.ClearIncludedWords();
					} else { 
						for ( int i = 0; i < args.size(); ++i ) {
							TheGenerator.RemoveIncludedWord( args[i] );
						}
					}
				} else {
					cout << "ERROR: expected at least 1 arg for command \'" << commandStr << "\'. " << args.size() << " provided.\n";
				}
			} break;

		case IGNORE:
			{
				// ignore -all			- adds all words to the list of words we DON'T want included in the generated anagrams
				// ignore [word]		- adds [word] to the list of words we DON'T want included in the generated anagrams
				if ( !args.empty() ) {
					if ( args.size() == 1 && args[0] == "-all" ) {
						TheGenerator.IgnoreAllWords();
					} else {
						for ( int i = 0; i < args.size(); ++i ) {
							TheGenerator.IgnoreWord( args[i] );
						}
					}
				} else {
					cout << "ERROR: expected at least 1 arg for command \'" << commandStr << "\'. " << args.size() << " provided.\n";
				}
			} break;

		case UNIGNORE:
			{
				// unignore [word]		- removes [word] from the list of words we DON'T want included in the generated anagrams
				// unignore -all		- removes all words from the list of words we DON'T want included in the generated anagrams
				if ( !args.empty() ) {
					if ( args.size() == 1 && args[0] == "-all" ) {
						TheGenerator.UnIgnoreAllWords();
					} else {
						for ( int i = 0; i < args.size(); ++i ) {
							TheGenerator.UnIgnoreWord( args[i] );
						}
					}
				} else {
					cout << "ERROR: expected at least 1 arg for command \'" << commandStr << "\'. " << args.size() << " provided.\n";
				}
			} break;

		case GENERATE:
			{
				// generate				- generates a list of anagram sentences that include the words in the included word list and do not include any words in the ignored word list
				if ( args.empty() ) {
					TheGenerator.GenerateAnagrams();
				} else {
					cout << "ERROR: expected 0 args for command \'" << commandStr << "\'. " << args.size() << " provided.\n";
				}
			} break;

		case PRINT:
			{
				// print [arg]
				// where [arg] is one of:
				// -spellable			- prints list of spellable words (including ignored words)
				// -included			- prints list of included words
				// -ignored				- prints list of ignored words
				// -available			- prints list of spellable words (excluding ignored words)

				if ( args.size() == 1 ) {
					if ( args[0] == "spellable" ) {
						TheGenerator.PrintSpellableWords();
					} else if ( args[0] == "included" ) {
						TheGenerator.PrintIncludedWords();
					} else if ( args[0] == "ignored" ) {
						TheGenerator.PrintIgnoredWords();
					} else if ( args[0] == "available" ) {
						TheGenerator.PrintAvailableWords();
					} else {
						cout << "ERROR: invalid argument provided for command \'" << commandStr << "\' \n";
					}
				} else {
					cout << "ERROR: expected 1 arg for command \'" << commandStr << "\'. " << args.size() << " provided.\n";
				}
			} break;

		case QUIT:
			{
				// quit					- exits the program
				if ( args.size() == 0 ) {
					cout << "\nMischief Managed...\n\n";
					return 0;
				} else {
					cout << "ERROR: expected 0 args for command \'" << commandStr << "\'. " << args.size() << " provided.\n";
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