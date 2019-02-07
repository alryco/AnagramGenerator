#include "AnagramGenerator.h"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

static const string DICTIONARY_FILENAME = "words50.txt";

static const int MAX_WORD_LENGTH = 50;
static const int MAX_INPUT_LENGTH = 100;


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
		cout << "\nEnter command:\n";

		memset( buffer, 0, MAX_INPUT_LENGTH );
		cin.getline( buffer, MAX_INPUT_LENGTH );
		vector<string> tokens = TokenizeInput( string( buffer ) );

		if ( tokens.size() < 1 ) {
			cout << "Invalid Input\n";
			continue;
		}

		string command = tokens[0];
		vector<string> args = tokens;
		args.erase( args.begin() );

		// TODO: Apparently you can't switch on a string in c++...gotta map commands to an enum or use if-else-if statements (gross)

		//switch ( command )
		//{
		//case "include": 
		//	{
		//		// include [word]		- add [word] to list of words we want included in the generated anagrams
		//	} break;

		//case "uninclude":
		//	{
		//		// uninclude [word]		- removes [word] from the list of words we want included in the generated anagrams
		//		// uninclude -all		- removes all words from the list of words we want included in the generated anagrams
		//	} break;

		//case "ignore":
		//	{
		//		// ignore [word]		- adds [word] to the list of words we DON'T want included in the generated anagrams
		//		// ignore -all			- adds all words to the list of words we DON'T want included in the generated anagrams
		//	} break;

		//case "unignore":
		//	{
		//		// unignore [word]		- removes [word] from the list of words we DON'T want included in the generated anagrams
		//		// unignore -all		- removes all words from the list of words we DON'T want included in the generated anagrams
		//	} break;

		//case "generate":
		//	{
		//		// generate				- generates a list of anagram sentences that include the words in the included word list and do not include any words in the ignored word list
		//	} break;

		//case "quit":
		//	{
		//		// quit					- exits the program
		//		if ( args.size() == 0 ) {
		//			cout << "\nMischief Managed...\n\n";
		//		} else {
		//			cout << "ERROR: Invalid number of args provided for command: quit\n";
		//		}
		//	} break;

		//default:
		//	{
		//		cout << "ERROR: Unrecognized command\n";
		//	} break;
		//}
	}

	
	return 0;
}