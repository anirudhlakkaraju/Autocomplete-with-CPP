#include<bits/stdc++.h> 
using namespace std;

const int CHARACTERS = 95;  ///< ASCII range of characters.
const int CAPACITY = 100;   ///< Max capacity of the LFU cache.

/// Creating a TrieNode structure, used to construct Trie data structure.
struct TrieNode
{
    bool isWord;    ///< Checks if word is valid word
    TrieNode *next[CHARACTERS]; ///< Array of pointers to next TrieNodes.
    
    /// TrieNode struct constructor.
    TrieNode()
    {
        isWord = false;

        for(int i=0; i<CHARACTERS; i++){
            next[i] = NULL;
        }
    }
};


/// Least Frequently Used (LFU) Cache class to store most frequently selected words.
class LFUCache
{
    private:

        int capacity;   ///< Max capacity of the LFU cache
        int minFreq;    ///< Current smallest word frequency in the cache

        unordered_map<string, int> word_freq;   ///< Word -> Frequency of usage
        unordered_map<int, list<string>> freq_wordList; ///< Frequency -> DLL containing words with that frequency of usage
        unordered_map<string, list<string>::iterator> word_listIter;    ///< Word -> Pointer to DLL node that contains it 

    public:
        
        LFUCache()
        {
            /// LFU cache Class constructor.
            /// Default maximum capacity is \link CAPACITY \endlink.

            capacity = CAPACITY;
        }
        
        int get(string word);   
        
        void put(string word);
 
        void print();
};


int LFUCache::get(string word) 
{
    /// Gets frequency of word, if it exists in the cache. @n
    /// @param word whose usage is to be checked. @n

    /// Time Complexity - O(1).

    if(word_freq.find(word) == word_freq.end())    
        return 0;

    return word_freq[word];
}


void LFUCache::put(string word) 
{
    /// Inserts word into the cache and updates its frequency. @n
    /// If word already exists in the cache then simply increments its frequency. @n
    /// If word does not exist in the cache, then two cases arise -
    /// 1. The max capacity is reached - Ejects word(s) with \link minFreq \endlink to make space and then inserts given word.
    /// 2. The max capacity is not reached - Simply inserts the given word which now has \link minFreq \endlink. @n
    
    /// @param word to be inserted in the cache. @n

    /// Time Complexity - O(1).

    if(capacity <= 0)  return;

    if(get(word) != 0)
    {
        int freq = word_freq[word];
        
        freq_wordList[freq++].erase(word_listIter[word]);
        freq_wordList[freq].push_front(word);
        word_listIter[word] = freq_wordList[freq].begin();
        word_freq[word] = freq;
        
        if (freq_wordList[minFreq].empty()) minFreq = freq;

        return;
    }


    if (word_freq.size() == capacity) {
        string delWord = freq_wordList[minFreq].back();
        freq_wordList[minFreq].pop_back();
        word_freq.erase(delWord);
        word_listIter.erase(delWord);
    }

    minFreq = 1;
    word_freq[word] = minFreq;
    freq_wordList[minFreq].push_front(word);
    word_listIter[word] = freq_wordList[minFreq].begin();
}


void LFUCache::print()
{    
    /// Prints contents of the LFU cache with corresponding frequency of usage. @n
    /// Time Complexity - O(N), where N is the size of the cache.
    
    unordered_map<string, int>::iterator it;

    if(word_freq.size() == 0){
        cout << "The LFU cache is EMPTY\n" << endl;
        return;
    }

    cout << "\nContents of the LFU cache along with corresponding usage :" << endl;

    for(it=word_freq.begin(); it!=word_freq.end(); it++){
        cout << it->first << " -> " << it->second << endl;
    }
    cout << endl;
}


/// The class for Trie data structure, whose objects contain loaded dictionary.
class Trie 
{
    private:

        TrieNode *root; ///< Root node 

    public:

        /// Trie Class constructor.
        Trie()
        {
            root = new TrieNode();
        }
        
        void insert(string word);

        bool search(string word);

        vector<string> get_all_words(string word);

        void autosuggest(string word, LFUCache &cache);
};


void Trie::insert(string word)
{
    /// Inserts given word in the Trie, then marks \link isWord \endlink as True. @n

    /// @param word to be added to dictionary.

    /// Time Complexity - O(M), where M is length of given word.
    
    TrieNode* temp = root;
    
    for(char &c:word)
    {
        if(temp->next[c - ' '] == NULL)
        {
            temp->next[c - ' '] = new TrieNode();
        }

        temp = temp->next[c - ' '];
    }
    
    temp->isWord = true;
}


bool Trie::search(string word)
{   
    /// Searches the Trie for given word, if it exists. @n

    /// @param word to be searched in dictionary. @n
    /// @return True is \link isWord \endlink. @n

    /// Time Complexity - O(M), where M is length of given word.

    if(root == NULL)    return false;

    TrieNode* temp = root; 
    
    for(int i=0; i<word.length(); i++)
    {
        int ind = int(word[i]) - ' ';

        if(temp->next[ind] == NULL)    return false;

        temp = temp->next[ind];
    }
    
    return temp->isWord;
}


vector<string> Trie::get_all_words(string word)
{
    /// Returns a vector all words with given word as prefix. @n
    /// These are the suggestions the user expects for their input in the entry field. @n
    /// Breadth First Search (BFS) is performed from the end of the prefix (given word) to find all valid 
    /// words by checking for \link isWord \link. @n 

    /// @param word which is a prefix to other words in dictionary.
    /// @returns Vector of all words for which given word is prefix.

    if(root == NULL)    return {};

    TrieNode* temp = root;

    for(int i=0; i<word.length(); i++)
    {
        int ind = (int)word[i] - ' ';

        if(temp->next[ind] == NULL) return {};

        temp = temp->next[ind];  
    }

    vector<string> suggestions;

    queue<pair<TrieNode*, string>> q;
    
    q.push(make_pair(temp, word));
    string currWord;

    while(!q.empty())
    {
        temp = q.front().first;
        currWord = q.front().second;

        if(temp->isWord)
        {
            suggestions.push_back(currWord);
        }

        q.pop();

        for(int i=0; i<CHARACTERS; i++)
        {
            if(temp->next[i] != NULL)
            {
                string nextWord = currWord + char(i + ' ');
                q.push(make_pair(temp->next[i], nextWord));
            }
        }
    }

    return suggestions;
}


void Trie::autosuggest(string word, LFUCache &cache)
{
    /// Prints the suggestions to the inputs of user in the entry field. @n
    /// Gets all valid words which have prefix as given word from get_all_words() method. @n
    /// Displays the suggestions according to their usage (frequencies), which are obtained from the LFU cache. @n
    /// Then lets the user select their desired suggestion and updates the cache.
    
    /// @param word which is inputs in entry field.
    /// @param cache which is the LFU cache to store and optimise suggestions. 

    if(word.size() == 0)    return;

   
    vector<string> suggestions = get_all_words(word); 

    if(suggestions.size() == 0)
    {
        cout << "\nThere are NO suggestions.\n" << endl;
        return;
    }

    cout << "\nThere are " << suggestions.size() << " suggestions for the prefix \"" << word << "\" : " << endl;

    int i;
    vector<pair<int, string>> rank;
    for(i=0; i<suggestions.size(); i++)
    {
        int rnk = cache.get(suggestions[i]);
        rank.push_back(make_pair(rnk, suggestions[i]));
    }
    
    sort(rank.rbegin(), rank.rend());

    for(int i=0; i<suggestions.size(); i++)
    {
        cout << i+1 << ". " << rank[i].second << endl;
    }

    int sel = -1;
    do {
        cout << "\nSelect suggestion (" << 1 << "-" << i << ") : ";
        if (cin >> sel && (sel >= 1 && sel <= i)) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while(true);

    string selection = rank[sel-1].second;

    cout << "\nSelected suggestion : " << selection << endl << endl;

    cache.put(selection);

    return;
}


void createDict(Trie*  trie){
    
    /// Load the dictionary into Trie data structure using insert() method. @n

    /// @param trie data structure to store loaded dictionary.

    int size;
    cout << "Enter dictionary size (100 for given dictionary) : ";
    cin >> size;

    string word;
    for(int i=0; i<size; i++){
        cin >> word;
        trie->insert(word); 
    }
    cout << endl;

    return;
}


void printOperations(){

    /// Displays choices of operations.

    cout    << "Operations - \n" 
            << "1. Load Dictionary \n"
            << "2. Entry Field \n"
            << "3. Check the LFU Cache \n"
            << "4. Exit \n";
    
    return;
}


int main() 
{
    /// The main method. @n
    /// Trie object is made to store and search the dictionary. The LFU cache object is made to optimise suggestions. @n
    /// Choices of following operations are given to user - 
    /// 1. Load dictionary -  Allows user to load desired dictionary into Trie object.
    /// 2. Entry Field - To enter prefixes.
    /// 3. Check the LFU cache - Allows user to check the cache contents.
    /// 4. Exit - Terminate loop.

    Trie *trie = new Trie();
    LFUCache cache;

    while(1) 
    {

        printOperations();
        int operation;
        do {
            cout << "\nSelect Operation : ";
            if (cin >> operation && (operation >= 1 && operation <= 4)) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        while(true);
        cout << endl; 
        
        switch (operation)
        {
        case 1:
        {
            createDict(trie);
            break;
        }

        case 2:
        {
            string word; 
            cout << "Entry Field : ";
            cin >> word;

            trie->autosuggest(word, cache); 

            break;
        }
        
        case 3:
        {
            cache.print(); 
            break;
        }
        
        case 4:
            exit(1);
            
        }
    }

    return 0;
}