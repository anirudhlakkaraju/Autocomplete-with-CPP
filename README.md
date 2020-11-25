# Autocomplete-with-CPP

Our project is to implement Auto Complete using trie data structure. Our application targets coding text editiors. Many text editors, especially web browser editors, like Jdoodle do not have fast autocomplete. 

Trie is a tree data structure which is used to store a key (a string). The main advantage of trie is efficient searching done in O(length_of_key) which is very fast compared to naive search where we check every word in the dictionary for a match. Using a preloaded dictionary, the algorithm can suggest words and improve the suggestions by storing words that were frequently selected by user in an LFU cache. The code works for all words, but we're loading C++ syntax words in our test cases. 


### Members
1. Anirudh Lakkaraju  (2017B4AA1376H)
2. Akash Singh Chauhan (2017B4AA1484H)
3. Vishesh Jain  (2017B5AA1158H)
4. Kirti Vatshal Mishra (2017B4AA0995H)


#### Sample Test Cases
Please follow procedure as explained and notice the outputs. There is no concrete output, as it is more of an interactive code. The working will be explained properly in the viva.  
The code performs the following operations. The index will select the corresponding operation. 

1. Load dictionary - Allows user to load desired dictionary into Trie object. First line requires size of dictionary (n). Next n lines are the words to be loaded.  
2. Entry Field - To enter prefixes.  
3. Check the LFU cache - Allows user to check the cache contents.  
4. Exit -Terminate loop.

#### Input
1  
5  
make_pair  
map  
max_element  
make_tuple  
malloc  

#### Output
2  
ma  
Select "make_pair" using the index. 

2  
ma  
Notice that the previously selected suggestion is on top now. Select "make_tuple" using the index. 

2  
ma  
Notice that "make_tuple" is ranked above "make_pair", even though the latter is lexicographically smaller. 
This is because if multiple words share the max frequency, the cache return the most recently used word.

The cache can be checked anytime for frequency of usage (Operation 3).


#### INPUT
1  
4   
int  
int8_t  
int16_t  
int32_t  
int64_t

#### Output
2  
int  
Select "int16_t" using the index. 

2  
int  
Notice that the previously selected suggestion is on top now. Select "int64_t" using the index. 

2  
int  
Notice that "int64_t" is ranked above "int8_t", even though the latter is lexicographically smaller. 
This is because if multiple words share the max frequency, the cache return the most recently used word.

Again the cache can be checked anyime for frequency of usage(Operation 3).


#### Note - 
Load the dictionary only ONCE for a test case.