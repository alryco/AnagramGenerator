# AnagramGenerator

## Ideas
- the little plastic letters on sydney's board are all caps...so M and W can be used interchangably. Implement this somehow to be able to use certain letters interchangebly.

- Favoring certain words that lead to funnier sentences (booby, butthole, etc.). Or maybe keep a separate list of 'funny' words that user can do stuff with.

- I think our word list is too broad, getting a better one with only practical words that you'd actually use would make this whole thing work a lot better probably

- <del>Add ability to add words made up words at runtime since you might want to include words that aren't 'real' (e.g. 'Toki', 'fuckton') </del>

- Right now the algorithm for generating anagrams not only gives you every list of words you can spell, but also every combination of each list of words you can spell which is resulting in a fuckton of results. I think it's probably more helpful just to see each unique set of words so the results don't get too cluttered. (e.g. if one of the anagrams generated is 'toki is good', I don't also need to see 'toki good is', 'is toki good', 'is good toki', 'good toki is', 'good is toki', etc.).

- I feel like there's probably a better data structure we can use for this than vectors...
