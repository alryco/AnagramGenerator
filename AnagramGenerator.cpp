#include "AnagramGenerator.h"

#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

static const int MAX_WORD_LENGTH = 50;


bool ANAGRAM_GENERATOR::ReadInDictionary( string filename ) 
{
	ifstream inFile( filename );

	if ( inFile.is_open() ) {

		cout << "populating dictionary from file: " << filename << "\n";

		while ( !inFile.eof() ) 
		{
			char buffer[MAX_WORD_LENGTH];
			inFile.getline( buffer, MAX_WORD_LENGTH );
			string word( buffer );
			_Dictionary.push_back( word );

			// print progress indicator
			if ( _Dictionary.size() % 10000 == 0 ) {
				cout << ".";
			}
		}

		cout << "\ndictionary populated with " << _Dictionary.size() << " words\n";

		return true;
	}

	cout << "failed to open file\n";
	return false;
}


bool ANAGRAM_GENERATOR::IsWordSpellable( const string& word, const vector<char>& letters ) const
{
	if ( letters.size() < word.length() ) {
		return false;
	}

	vector<bool> usedLetters;
	usedLetters.resize( letters.size(), false );

	for ( int i = 0; i < word.length(); ++i ) 
	{
		bool foundLetter = false;
		for ( int j = 0; j < letters.size(); ++j )
		{
			// todo: if _InputLetters is sorted we can add a check here to bail out early
			if ( word.at(i) == letters[j] && !usedLetters[j] ) {
				usedLetters[j] = true;
				foundLetter = true;
				break;
			}
		}

		if ( !foundLetter ) return false;
	}

	return true;
}


static bool IsSpace( char c ) 
{
	return ( c == ' ' );
}

void ANAGRAM_GENERATOR::FindSpellableWords( string input )
{
	// clean up and save input as list of characters
	//input.erase( remove_if( input.begin(), input.end(), IsSpace ), input.end() ); // remove any spaces
	transform( input.begin(), input.end(), input.begin(), ::tolower ); // convert all characters to lowercase
	sort( input.begin(), input.end() ); // alphabetize
	for ( int i = 0; i < input.length(); ++i ) 
	{
		if ( input.at(i) >= 'a' && input.at(i) <= 'z' ) {
			_InputLetters.push_back( input.at(i) );
		}
		
	}

	cout << "\nFinding words spellable with input letters: " << input << "...\n";

	for ( int i = 0; i < _Dictionary.size(); ++i ) 
	{
		if ( IsWordSpellable( _Dictionary[i], _InputLetters ) ) {
			_pSpellableWords.push_back( &_Dictionary[i] );
			//cout << _Dictionary[i] << "\n";
		}
	}

	cout << "found " << _pSpellableWords.size() << " words\n";

}

void ANAGRAM_GENERATOR::FindAnagrams( 	const vector<char>& letters, // list letters that haven't been used yet
										const vector<string*>& words, // list of words we can spell with letters
										vector<string*> sentence )	// the current anagram sentence
{
	for ( int w = 0; w < words.size(); ++w ) // for each word in our list of words 
	{
		vector<char> remainingLetters = letters;
		vector<string*> remainingWords;
		vector<string*> newSentence = sentence;

		// grab our word
		string* word = words[w];

		// add the word to the sentence 
		newSentence.push_back( word );

		// remove letters in word from list of remaining letters
		for ( int l = 0; l < word->length(); ++l ) // for each letter in the word 
		{
			for ( vector<char>::iterator it = remainingLetters.begin(); it != remainingLetters.end(); ++it ) // for each letter in our list of letters
			{
				// remove 
				if ( word->at(l) == *it ) {
					remainingLetters.erase( it );
					break;
				}
			}
		}

		// remove words in word list we can no longer spell
		for ( int x = 0; x < words.size(); ++x ) 
		{
			if ( IsWordSpellable( *words[x], remainingLetters ) ) {
				remainingWords.push_back( words[x] );
			}
		}

		// if we're out of letters, add our finished sentence to the list of anagrams
		if ( remainingLetters.empty() ) {
			_Anagrams.push_back( newSentence );
			for ( int i = 0; i < newSentence.size(); ++i ) {
				cout << *newSentence[i] << " ";
			}
			cout << endl;
		} else if ( !remainingWords.empty() ) { // if we still have words left, keep going
			FindAnagrams( remainingLetters, remainingWords, newSentence );
		}
	}		
}

void ANAGRAM_GENERATOR::GenerateAnagrams( string inputSentence ) 
{
	// populate our letter bank and list of spellable words
	FindSpellableWords( inputSentence );

	// populate list of anagram sentences
	vector<string*> sentence;
	FindAnagrams( _InputLetters, _pSpellableWords, sentence );
}


void ANAGRAM_GENERATOR::PrintAnagrams( void ) const
{
	cout << "\n\nPRINTING ANAGRAMS... \n";

	for ( int i = 0; i < _Anagrams.size(); ++i )
	{
		for ( int j = 0; j < _Anagrams[i].size(); ++j )
		{
			cout << *_Anagrams[i][j] << " ";
		}

		cout << "\n";
	}
}