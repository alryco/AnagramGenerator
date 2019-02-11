
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
	std::vector<WORD*>						_Dictionary;

	std::vector<char>						_InputLetters;		// alphabetized list of letters from the input sentence	
	std::vector<WORD*>						_SpellableWords;	// list of words that we can spell with _InputLetters

	std::vector<WORD*>						_IncludedWords;		// list of words set by the user that will be included in the generated anagrams (if possible)
	std::vector<char>						_AvailableLetters;	// list of input letters exluding all letters in the included words list
	std::vector<WORD*>						_AvailableWords;	// list of words we can make using available letters

	std::vector<std::vector<WORD*>>			_Anagrams;			// list of anagram sentences. Each anagram sentence stored as a list of ptrs to words.

private:
	/** Recursive function that finds all possible anagram sentences that can be made using all letters. Once a full anagram is found, it is saved into the _Anagrams list. */
	void FindAnagrams(	const std::vector<char>&		letters,
						const std::vector<WORD*>& 		words,
						std::vector<WORD*> 				sentence );

	/** Removes each letter in word from a list of letters returning true if successful.
		If a letter does not exist in letters, function returns false and letters list is unmodified. */
	bool RemoveLettersInWordFromList( const WORD* word, std::vector<char>& letters );

	/** Adds each letter in word to a list of letters */
	void AddLettersInWordToList( const WORD* word, std::vector<char>& letters );

public:
	ANAGRAM_GENERATOR( std::string dictionaryFilename );

	~ANAGRAM_GENERATOR( void );

	void FindSpellableWords( std::string inputSentence );

	void GenerateAnagrams( void );
	
	void AddIncludedWord( std::string wordStr );
	
	void RemoveIncludedWord( std::string wordStr );

	void ClearIncludedWords( void );

	void IgnoreWord( std::string wordStr );

	void UnIgnoreWord( std::string wordStr );

	void IgnoreAllWords( void );

	void UnIgnoreAllWords( void );

public:
	// Methods for printing shit 
	void PrintSpellableWords( void ) const;
	void PrintIncludedWords( void ) const;
	void PrintIgnoredWords( void ) const;
	void PrintAvailableWords( void ) const;
	void PrintAnagram( std::vector<WORD*> anagram ) const;
	void PrintAnagrams( void ) const;
};