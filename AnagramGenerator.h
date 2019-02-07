
#include <string>
#include <vector>



class ANAGRAM_GENERATOR 
{
private:
	std::vector<std::string>				_Dictionary;

	std::vector<char>						_InputLetters;	
	std::vector<std::string*>				_pSpellableWords; // list of ptrs to words that we can spell with _InputLetters
	std::vector<std::vector<std::string*>>	_Anagrams;


private:
	// Recursive function that finds all possible anagram sentences that can be made using all letters. Once a full anagram is found, it is saved into the _Anagrams list.
	void FindAnagrams(  const std::vector<char>&		 		letters, 
						const std::vector<std::string*>& 		words, 
						std::vector<std::string*> 				sentence 	);

	
	bool IsWordSpellable( const std::string& word, const std::vector<char>& letters ) const;

public:

	bool ReadInDictionary( std::string filename );

	void FindSpellableWords( std::string inputSentence );

	void GenerateAnagrams( void );

	void PrintAnagram( std::vector<std::string*> anagram ) const;

	void PrintAnagrams( void ) const;
};