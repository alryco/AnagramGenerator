#include "AnagramGenerator.h"

#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

static const int MAX_WORD_LENGTH = 50;





//************************************************************************************
ANAGRAM_GENERATOR::ANAGRAM_GENERATOR( string dictionaryFilename )
{
	ReadInDictionary( dictionaryFilename );
}

//************************************************************************************
bool ANAGRAM_GENERATOR::ReadInDictionary( string filename ) 
{
	ifstream inFile( filename );

	if ( inFile.is_open() ) {

		cout << "populating dictionary from file: " << filename << "\n";

		while ( !inFile.eof() ) 
		{
			char buffer[MAX_WORD_LENGTH];
			inFile.getline( buffer, MAX_WORD_LENGTH );
			string str( buffer );
			_Dictionary.push_back( WORD( str ) );

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
		if ( _Dictionary[i].CanSpellWithLetters( _InputLetters ) ) {
			_SpellableWords.push_back( &_Dictionary[i] );
			//cout << _Dictionary[i].GetString() << "\n";
		}
	}

	cout << "found " << _SpellableWords.size() << " words\n";
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
			for ( int m = 0; m < remainingLetters.size(); ++m )
			{
				if ( word->GetCharAt( l ) == remainingLetters[m] ) {
					// remove the letter from our list of remaining letters
					remainingLetters.erase( remainingLetters.begin() + m );
					break;
				}
			}
			// if we can't find the letter in our list of remaining letters, this combination of included words is not possible.
			cout << "This combination of included words cannot be spelled with the letters in the input sentence\n\n";
			return;
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
				cout << "WARNING: \'" << wordStr << "\' no longer being ignored\n";
			}
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
			return;
		}
	}

	cout << "ERROR: \'" << wordStr << "\' not found in the list of included words\n";
}


//************************************************************************************
void ANAGRAM_GENERATOR::ClearIncludedWords( void )
{
	_IncludedWords.clear();
}


//************************************************************************************
void ANAGRAM_GENERATOR::IgnoreWord( string wordStr )
{
	transform( wordStr.begin(), wordStr.end(), wordStr.begin(), ::tolower ); // convert all characters to lowercase

	for ( int i = 0; i < _SpellableWords.size(); ++i )
	{
		if ( wordStr == _SpellableWords[i]->GetString() ) {
			_SpellableWords[i]->SetIgnored( true );
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
}


//************************************************************************************
void ANAGRAM_GENERATOR::UnIgnoreAllWords( void )
{
	for ( int i = 0; i < _SpellableWords.size(); ++i )
	{
		_SpellableWords[i]->SetIgnored( false );
	}
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