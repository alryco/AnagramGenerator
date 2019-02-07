
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
	void FindAnagrams(  const std::vector<char>&		 		letters, 
						const std::vector<std::string*>& 		words, 
						std::vector<std::string*> 				sentence 	);

	
	bool IsWordSpellable( const std::string& word, const std::vector<char>& letters ) const;

	void FindSpellableWords( std::string input );

public:

	bool ReadInDictionary( std::string filename );

	void GenerateAnagrams( std::string inputSentence );

	void PrintAnagrams( void ) const;
};