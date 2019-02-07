
#include <string>
#include <vector>


/** Class representing a single word in the dictionary, and any associated metadata. */
class WORD
{
	const std::string	_Str;
	bool				_Ignored;	// whether this word should be ignored when generating anagrams (true) or not (false)

public:
	WORD( std::string str ) : _Str(str), _Ignored(false) {};

	// Accessor functions
	inline std::string GetString( void ) const { return _Str; }
	inline void SetIgnored( const bool& v ) { _Ignored = v; }
	inline bool IsIgnored( void ) const { return _Ignored; }
	inline const char& GetCharAt( const int& index ) const { return _Str.at( index ); }
	inline size_t GetLength( void ) const { return _Str.length(); }

	bool CanSpellWithLetters( const std::vector<char>& letters ) const;
};


class ANAGRAM_GENERATOR 
{


private:
	std::vector<WORD>						_Dictionary;

	std::vector<char>						_InputLetters;		// alphabetized list of letters from the input sentence	
	std::vector<WORD*>						_SpellableWords;	// list of words that we can spell with _InputLetters

	std::vector<WORD*>						_IncludedWords;		// list of words set by the user that will be included in the generated anagrams (if possible)

	std::vector<std::vector<WORD*>>			_Anagrams;			// list of anagram sentences. Each anagram sentence stored as a list of ptrs to words.

private:
	bool ReadInDictionary( std::string filename );

	// Recursive function that finds all possible anagram sentences that can be made using all letters. Once a full anagram is found, it is saved into the _Anagrams list.
	void FindAnagrams(  const std::vector<char>&		letters, 
						const std::vector<WORD*>& 		words, 
						std::vector<WORD*> 				sentence 	);

public:

	ANAGRAM_GENERATOR( std::string dictionaryFilename );

	void FindSpellableWords( std::string inputSentence );

	void GenerateAnagrams( void );

	void PrintAnagram( std::vector<WORD*> anagram ) const;

	void PrintAnagrams( void ) const;
	
	void AddIncludedWord( std::string wordStr ); // Add word to the list of words we want included in the generated anagrams.
	
	void RemoveIncludedWord( std::string wordStr ); // Remove word from the list of words we want included in the generated anagrams.

	void ClearIncludedWords( void ); // Clear list of included words

	void IgnoreWord( std::string wordStr ); // Set a word to be ignored when generating anagrams

	void UnIgnoreWord( std::string wordStr ); // Set a previously ignored word to be eligible again when generating anagrams

	void IgnoreAllWords( void );

	void UnIgnoreAllWords( void );

};