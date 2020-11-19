#include<bits/stdc++.h> 
using namespace std;

const int CHARACTERS = 95;


struct TrieNode ///Creating a TRIE struct
{
    bool isWord;/// Struct data members
    TrieNode *next[CHARACTERS];

    TrieNode()/// TrieNode struct constructor
    {
        isWord = false;

        for(int i=0; i<CHARACTERS; i++){
            next[i] = NULL;
        }
    }
};


class LFUCache ///LFU Cache class for word frequency related calculations
{
    private:

        int capacity, minFreq;/// Private data members

        unordered_map<string, int> word_freq;
        unordered_map<int, list<string>> freq_wordList;
        unordered_map<string, list<string>::iterator> word_listIter;

    public:

        LFUCache()/// Class constructor
        {
            capacity = 1000;
        }

        int get(string word);/// Get frequency of word

        void put(string word);/// Insert word into cache 

        void print();/// Print cache
};


int LFUCache::get(string word) ///used to get the word from the cache
{
    if(word_freq.find(word) == word_freq.end())    
        return 0;

    return word_freq[word];
}


void LFUCache::put(string word) ///used to update the cache
{
    if(capacity <= 0)  return;
    
    if(get(word) != 0)
    {
        int freq = word_freq[word]; ///used to store the fruquency for a particular word
        
        freq_wordList[freq++].erase(word_listIter[word]);///used to erase words
        freq_wordList[freq].push_front(word); ///used to enter words or push them
        word_listIter[word] = freq_wordList[freq].begin();
        word_freq[word] = freq;
        
        if (freq_wordList[minFreq].empty()) minFreq = freq;

        return;
    }

    if (word_freq.size() == capacity) /**checks if max capacity is reached*/ {
        string delWord = freq_wordList[minFreq].back();///delete the word list which has minimum frequency
        freq_wordList[minFreq].pop_back();
        word_freq.erase(delWord);
        word_listIter.erase(delWord);
    }

    minFreq = 1; ///checks that new word isn't already present in the cache
    word_freq[word] = minFreq;
    freq_wordList[minFreq].push_front(word);
    word_listIter[word] = freq_wordList[minFreq].begin();
}


void LFUCache::print()///prints the number of occurences of a word
{    
    unordered_map<string, int>::iterator it; ///mapping the word to the counter and printing it

    if(word_freq.size() == 0)/**checks for 0 size cache, i.e counter is 0*/{
        cout << "Empty cache\n" << endl;
        return;
    }

    for(it=word_freq.begin(); it!=word_freq.end(); it++){
        cout << it->first << " : " << it->second << endl;
    }
}


class Trie ///The class TRIE with all the operations as public member functions
{
    private:

        TrieNode *root;/// Private data member - root node of trie

    public:

        Trie()/// Trie Class constructor
        {
            root = new TrieNode();
        }
        
        /// PUBLIC METHODS : 
        
        void insert(string word);/// Inserts word into Trie

        
        bool search(string word);/// Searches for word in trie

        
        vector<string> get_all_words(string word);/// Gets all words with given prefix

        
        void autosuggest(string word, LFUCache &cache);/// Suggests valid words
};


void Trie::insert(string word)///Inserts words in the TRIE
{
    TrieNode* temp = root;
    
    for(char &c:word)
    {
        if(temp->next[c - ' '] == NULL)
        {
            temp->next[c - ' '] = new TrieNode();
        }

        temp = temp->next[c - ' '];
    }
    
    temp->isWord = true;/// Make isWord true to indicate complete word
}


bool Trie::search(string word)///Searches the TRIE for the word
{   
    /// If Trie is empty
    if(root == NULL)    return false;

    TrieNode* temp = root; ///Assign a temporary variable as TRIE root
    
    for(int i=0; i<word.length(); i++)///Iterate through the DS and see if the matching word is found
    {
        int ind = int(word[i]) - ' ';

        if(temp->next[ind] == NULL)    return false;

        temp = temp->next[ind];
    }
    
    return temp->isWord;
}


vector<string> Trie::get_all_words(string word)
{
    if(root == NULL)    return {};/// If TRIE DS is empty

    TrieNode* temp = root;

    for(int i=0; i<word.length(); i++)
    {
        /// Index of char word[i] in TrieNode;
        int ind = (int)word[i] - ' ';

        /// If child node at ind of TrieNode is NULL, then word is not a valid prefix
        if(temp->next[ind] == NULL) return {};

        temp = temp->next[ind];  
    }

    /// Array to store valid suggestions
    vector<string> suggestions;

    /// If this word is a valid prefix then BFS to find all words
    queue<pair<TrieNode*, string>> q;
    
    q.push(make_pair(temp, word));
    string currWord;

    while(!q.empty())///Now We perform the Breadth-First-Search (BFS)
    {
        temp = q.front().first;
        currWord = q.front().second;

        /// If this word is a valid word, then add to all_words
        if(temp->isWord)
        {
            suggestions.push_back(currWord);
        }

        q.pop();

        for(int i=0; i<CHARACTERS; i++)
        {
            if(temp->next[i] != NULL)
            {
                string nextWord = currWord + char(i + ' ');/// Next word with prefix as given word found
                q.push(make_pair(temp->next[i], nextWord));
            }
        }
    }

    return suggestions;
}


void Trie::autosuggest(string word, LFUCache &cache)
{
    
    if(word.size() == 0)    return;/// If empty word then no suggestions would be sent

   
    vector<string> suggestions = get_all_words(word); /// If valid word, then print all words for which given word is prefix

    if(suggestions.size() == 0)///Checks if there are no suggestions
    {
        cout << "There are no suggestions." << endl;
        return;
    }

    cout << "There are " << suggestions.size() << " suggestions for the prefix \"" << word << "\" : " << endl;/// Print all suggestions

    int i;
    vector<pair<int, string>> rank;
    for(i=0; i<suggestions.size(); i++)/// Rank suggestions according to cache
    {
        int rnk = cache.get(suggestions[i]);
        rank.push_back(make_pair(rnk, suggestions[i]));
    }
    
    sort(rank.rbegin(), rank.rend());

    for(int i=0; i<suggestions.size(); i++)/// Display suggestions available
    {
        cout << i+1 << ". " << rank[i].second << " " << rank[i].first << endl;
    }

    int sel = -1;/// Select valid suggestion hence we run the following do-while loop
    do {
        cout << "Select suggestion (" << 1 << "-" << i << ") : ";
        if (cin >> sel && (sel >= 1 && sel <= i)) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while(true);

    string selection = rank[sel-1].second;/// Stores the selected suggestion

    cout << "\nSelected suggestion : " << selection << endl << endl;

    cache.put(selection);/// Update cache counter for the words

    return;
}


void createDict(Trie*  trie)/**Load the dictionary in to the code */{

    int size; ///Number of words
    cout << "Enter dictionary size (100 for given dictionary) : ";
    cin >> size;

    string word;///temporary variable to store words
    for(int i=0; i<size; i++){
        cin >> word;
        trie->insert(word); /// This helps inserting the words in the TRIE Data Structure
    }
    cout << endl;

    return;
}


void printOperations()/**Operations that can be performed*/{
    
    cout    << "Operations - \n" 
            << "1. Load Dictionary \n"
            << "2. Entry Field \n"
            << "3. Check Cache \n"
            << "4. Exit \n";
    
    return;
}


int main() /// The main method
{
    Trie *trie = new Trie();
    LFUCache cache;

    while(1) /// Looping through the statements till terminating condition is met
    {
        printOperations(); /**Shows the Options available to the user */

        int operation;
        do {
            cout << "\nSelect Operation : ";
            if (cin >> operation && (operation >= 1 && operation <= 4)) break; /// Checks if the operation is Valid
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        while(true);///if the operation is true

        switch (operation)///Switching case options based on the operaton chosen
        {
        case 1:
        {
            createDict(trie);/// Calls the method to create the dictionary
            break;
        }

        case 2:
        {
            string word; ///Takes a word input from the user
            cout << "Entry Field : ";
            cin >> word;

            trie->autosuggest(word, cache); ///Makes a function call to make suggestions based on the word entered

            break;
        }
        
        case 3:
        {
            cache.print(); ///Prints the selected cache sequence, the word counter
            break;
        }
        
        case 4:
            exit(1); /// exit condition
            
        }
    }

    return 0;
}