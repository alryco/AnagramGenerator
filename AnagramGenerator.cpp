#include "AnagramGenerator.h"

#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

static const int MAX_WORD_LENGTH = 50;



//************************************************************************************
ANAGRAM_GENERATOR::ANAGRAM_GENERATOR( string dictionaryFilename ) 
{
	ifstream inFile( dictionaryFilename );

	// read in dictionary from file
	if ( inFile.is_open() ) {

		cout << "populating dictionary from file: " << dictionaryFilename << "\n";

		while ( !inFile.eof() ) 
		{
			// print progress indicator
			if ( _Dictionary.size() % 1000 == 0 ) {
				cout << ".";
			}

			char buffer[MAX_WORD_LENGTH];
			inFile.getline( buffer, MAX_WORD_LENGTH );
			string str( buffer );
			WORD* pWord = new WORD( str );
			_Dictionary.push_back( pWord );
		}

		cout << "\ndictionary populated with " << _Dictionary.size() << " words\n";

	} else {

		cout << "failed to open dictionary file \'" << dictionaryFilename << "\'\n";
	}
}


//************************************************************************************
ANAGRAM_GENERATOR::~ANAGRAM_GENERATOR( void )
{
	for ( int i = 0; i < _Dictionary.size(); ++i ) 
	{
		delete _Dictionary[i];
	}
}


//************************************************************************************
static bool IsSpace( char c ) 
{
	return ( c == ' ' );
}


//************************************************************************************
void ANAGRAM_GENERATOR::FindSpellableWords( string input )
{
	cout << "\nFinding words spellable with input letters: ";

	// clean up and save input as list of characters
	//input.erase( remove_if( input.begin(), input.end(), IsSpace ), input.end() ); // remove any spaces
	transform( input.begin(), input.end(), input.begin(), ::tolower ); // convert all characters to lowercase
	sort( input.begin(), input.end() ); // alphabetize
	for ( int i = 0; i < input.length(); ++i ) 
	{
		if ( input.at(i) >= 'a' && input.at(i) <= 'z' ) {
			_InputLetters.push_back( input.at(i) );
			cout << input.at(i);
		}	
	}
	cout << "\n";

	for ( int i = 0; i < _Dictionary.size(); ++i ) 
	{
		if ( _Dictionary[i]->CanSpellWithLetters( _InputLetters ) ) {
			_SpellableWords.push_back( _Dictionary[i] );
		}

		// print progress indicator
		if ( i % 1000 == 0 ) {
			cout << ".";
		}
	}

	cout << "\nfound " << _SpellableWords.size() << " words\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::FindAnagrams( 	const vector<char>& letters, // list of letters that haven't been used yet
										const vector<WORD*>& words, // list of words we can spell with letters
										vector<WORD*> sentence )	// the current anagram sentence
{
	for ( int w = 0; w < words.size(); ++w ) // for each word in our list of words 
	{
		vector<char> remainingLetters = letters;
		vector<WORD*> remainingWords;
		vector<WORD*> newSentence = sentence;

		// grab our word
		WORD* word = words[w];

		// skip ignored words
		if ( word->IsIgnored() ) {
			continue;
		}

		// add the word to the sentence 
		newSentence.push_back( word );

		// remove letters in our word from list of remaining letters
		for ( int l = 0; l < word->GetLength(); ++l ) // for each letter in our word 
		{
			for ( int m = 0; m < remainingLetters.size(); ++m ) // for each letter in our list of remaining letters
			{
				if ( word->GetCharAt(l) == remainingLetters[m] ) {
					// remove the letter from our list of remaining letters
					remainingLetters.erase( remainingLetters.begin() + m );
					break;
				}
			}
		}

		// determine which words we can still spell
		for ( int i = 0; i < words.size(); ++i ) 
		{
			if ( words[i]->CanSpellWithLetters( remainingLetters ) ) {
				remainingWords.push_back( words[i] );
			}
		}

		// if we're out of letters, add our finished sentence to the list of anagrams
		if ( remainingLetters.empty() ) {
			_Anagrams.push_back( newSentence );
			PrintAnagram( newSentence );
		} else if ( !remainingWords.empty() ) { // if we still have words left, keep going
			FindAnagrams( remainingLetters, remainingWords, newSentence );
		}

		// if we get here it means we have letters left but they don't spell anything.
		// Let's ignore this cases for now and just say that anagrams have to use all the letters.
	}		
}


//************************************************************************************
void ANAGRAM_GENERATOR::GenerateAnagrams( void ) 
{
	// clear out our list of anagrams
	_Anagrams.clear();

	vector<WORD*> startingSentence = _IncludedWords;

	// assemble list of remaining letters we have to work with
	vector<char> remainingLetters = _InputLetters;
	for ( int s = 0; s < startingSentence.size(); ++s )
	{
		WORD* word = startingSentence[s];
		for ( int l = 0; l < word->GetLength(); ++l )
		{
			bool foundLetter = false;
			for ( int m = 0; m < remainingLetters.size(); ++m )
			{
				if ( word->GetCharAt( l ) == remainingLetters[m] ) {
					// remove the letter from our list of remaining letters
					remainingLetters.erase( remainingLetters.begin() + m );
					foundLetter = true;
					break;
				}
			}

			// if we can't find the letter in our list of remaining letters, this combination of included words is not possible.
			if ( !foundLetter ) {
				cout << "This combination of included words cannot be spelled with the letters in the input sentence\n";
				return;
			}

		}
	}

	// assemble our list of remaining spellable words
	vector<WORD*> remainingWords;
	for ( int i = 0; i < _SpellableWords.size(); ++i )
	{
		if ( _SpellableWords[i]->CanSpellWithLetters( remainingLetters ) ) {
			remainingWords.push_back( _SpellableWords[i] );
		}
	}

	if ( remainingLetters.empty() ) {
		_Anagrams.push_back( startingSentence );
	} else if ( !remainingWords.empty() ) {
		FindAnagrams( remainingLetters, remainingWords, startingSentence );
	}

	cout << "Found " << _Anagrams.size() << " anagrams\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::PrintAnagram( vector<WORD*> anagram ) const
{
	for ( int i = 0; i < anagram.size(); ++i )
	{
		cout << anagram[i]->GetString() << " ";
	}
	cout << "\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::PrintAnagrams( void ) const
{
	cout << "\n\nPRINTING ANAGRAMS... \n";

	for ( int i = 0; i < _Anagrams.size(); ++i )
	{
		PrintAnagram( _Anagrams[i] );
	}
}


//************************************************************************************
void ANAGRAM_GENERATOR::AddIncludedWord( string wordStr )
{
	transform( wordStr.begin(), wordStr.end(), wordStr.begin(), ::tolower ); // convert all characters to lowercase

	// find word in the list of spellable words
	for ( int i = 0; i < _SpellableWords.size(); ++i ) 
	{
		if ( wordStr == _SpellableWords[i]->GetString() ) {
			_IncludedWords.push_back( _SpellableWords[i] );

			// if ignore flag was previously set, clear it and notify the user
			if ( _SpellableWords[i]->IsIgnored() ) { 
				_SpellableWords[i]->SetIgnored( false );
				cout << "Removed \'" << wordStr << "\' from the list of ignored words\n";
			}

			cout << "Added \'" << wordStr << "\' to the list of included words\n";
			return;
		}
	}

	cout << "ERROR: \'" << wordStr << "\' not found in the list of spellable words\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::RemoveIncludedWord( string wordStr )
{
	transform( wordStr.begin(), wordStr.end(), wordStr.begin(), ::tolower ); // convert all characters to lowercase

	for ( int i = 0; i < _IncludedWords.size(); ++i ) 
	{
		if ( wordStr == _IncludedWords[i]->GetString() ) {
			_IncludedWords.erase( _IncludedWords.begin() + i );
			cout << "Removed \'" << wordStr << "\' from the list of included words\n";
			return;
		}
	}

	cout << "ERROR: \'" << wordStr << "\' not found in the list of included words\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::ClearIncludedWords( void )
{
	_IncludedWords.clear();

	cout << "Removed all words from the list of included words\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::IgnoreWord( string wordStr )
{
	transform( wordStr.begin(), wordStr.end(), wordStr.begin(), ::tolower ); // convert all characters to lowercase

	for ( int i = 0; i < _SpellableWords.size(); ++i )
	{
		if ( wordStr == _SpellableWords[i]->GetString() ) {
			_SpellableWords[i]->SetIgnored( true );
			cout << "Added \'" << wordStr << "\' to the list of ignored words\n";
			return;
		}
	}

	cout << "ERROR: \'" << wordStr << "\' not found in the list of spellable words\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::UnIgnoreWord( string wordStr )
{
	transform( wordStr.begin(), wordStr.end(), wordStr.begin(), ::tolower ); // convert all characters to lowercase

	for ( int i = 0; i < _SpellableWords.size(); ++i )
	{
		if ( wordStr == _SpellableWords[i]->GetString() ) {
			_SpellableWords[i]->SetIgnored( false );
			cout << "Removed \'" << wordStr << "\' from the list of ignored words\n";
			return;
		}
	}

	cout << "ERROR: \'" << wordStr << "\' not found in the list of spellable words\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::IgnoreAllWords( void )
{
	for ( int i = 0; i < _SpellableWords.size(); ++i )
	{
		_SpellableWords[i]->SetIgnored( true );
	}

	cout << "Added all spellable words to the list of ignored words\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::UnIgnoreAllWords( void )
{
	for ( int i = 0; i < _SpellableWords.size(); ++i )
	{
		_SpellableWords[i]->SetIgnored( false );
	}

	cout << "Removed all words from the list of ignored words\n";
}


static const int NUM_COLUMNS = 3;
static const int COLUMN_WIDTH = 15; // number of chars per column when printing word lists

//************************************************************************************
// Helper function for printing a list of words
// words:			list of words.
// onlyPrintIfFunc:	(optional) function ptr returning true if word should be printed and false if it should be skipped. 
//					(if no function ptr is provided all words will be printed)
static void PrintWordList( const vector<WORD*>& words, bool (*onlyPrintIfFunc)(const WORD*)=nullptr ) 
{
	for ( int i = 0; i < words.size(); ++i )
	{
		if ( onlyPrintIfFunc != nullptr && !onlyPrintIfFunc( words[i] ) ) {
			continue;
		}

		// print word
		cout << words[i]->GetString();

		// print padding
		if ( i % NUM_COLUMNS == NUM_COLUMNS-1 ) {
			cout << "\n"; // if it's the last word in the row just print a newline
		} else {
			if ( words[i]->GetLength() >= COLUMN_WIDTH ) {
				cout << "\n"; // if word is longer than the column width just start a new row so we don't run into the next word
			} else {
				// print spaces to fill rest of column 
				for ( int j = 0; j < COLUMN_WIDTH - words[i]->GetLength(); ++j ) {
					cout << " ";
				}
			}
		}
	}

	cout << "\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::PrintSpellableWords( void ) const
{
	cout << "\n****************************************\n";
	cout << "Spellable Words:";
	cout << "\n****************************************\n";

	PrintWordList( _SpellableWords );
}


//************************************************************************************
void ANAGRAM_GENERATOR::PrintIncludedWords( void ) const
{
	cout << "\n****************************************\n";
	cout << "Included Words:";
	cout << "\n****************************************\n";

	PrintWordList( _IncludedWords );
}


static bool IsWordIgnored( const WORD* word )
{
	return word->IsIgnored();
}

//************************************************************************************
void ANAGRAM_GENERATOR::PrintIgnoredWords( void ) const
{
	cout << "\n****************************************\n";
	cout << "Ignored Words:";
	cout << "\n****************************************\n";

	PrintWordList( _SpellableWords, IsWordIgnored );
}


static bool IsWordAvailable( const WORD* word )
{
	return !word->IsIgnored();
}

//************************************************************************************
void ANAGRAM_GENERATOR::PrintAvailableWords( void ) const
{
	cout << "\n****************************************\n";
	cout << "Available Words:";
	cout << "\n****************************************\n";

	PrintWordList( _SpellableWords, IsWordAvailable );
}


////////////////////////////// WORD CLASS METHODS /////////////////////////////////////////////

//************************************************************************************
bool WORD::CanSpellWithLetters( const vector<char>& letters ) const
{
	if ( letters.size() < _Str.length() ) {
		return false;
	}

	vector<bool> usedLetters;
	usedLetters.resize( letters.size(), false );

	for ( int i = 0; i < _Str.length(); ++i ) 
	{
		bool foundLetter = false;
		for ( int j = 0; j < letters.size(); ++j )
		{
			// todo: if letters is sorted we can add a check here to bail out early
			if ( _Str.at(i) == letters[j] && !usedLetters[j] ) {
				usedLetters[j] = true;
				foundLetter = true;
				break;
			}
		}

		if ( !foundLetter ) return false;
	}

	return true;
}