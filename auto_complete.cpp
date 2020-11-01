#include<bits/stdc++.h>
using namespace std;

const int CHARACTERS = 95;


struct TrieNode
{
    // Struct data members
    bool isWord;
    TrieNode *next[CHARACTERS];

    // TrieNode struct constructor
    TrieNode()
    {
        isWord = false;

        for(int i=0; i<CHARACTERS; i++){
            next[i] = NULL;
        }
    }
};


class LFUCache 
{
    private:

        // Private data members
        int capacity, minFreq;

        unordered_map<string, int> word_freq;
        unordered_map<int, list<string>> freq_wordList;
        unordered_map<string, list<string>::iterator> word_listIter;

    public:

        // Class constructor
        LFUCache()
        {
            capacity = 1000;
        }

        // PUBLIC METHODS :

        // Get frequency of word
        int get(string word);

        // Insert word into cache 
        void put(string word);

        // Print cache
        void print();
};


int LFUCache::get(string word) 
{
    if(word_freq.find(word) == word_freq.end())    
        return 0;

    return word_freq[word];
}


void LFUCache::put(string word) 
{
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
    unordered_map<string, int>::iterator it;

    if(word_freq.size() == 0){
        cout << "Empty cache\n" << endl;
        return;
    }

    for(it=word_freq.begin(); it!=word_freq.end(); it++){
        cout << it->first << " : " << it->second << endl;
    }
}


class Trie
{
    private:

        // Private data member - root node of trie
        TrieNode *root;

    public:

        // Trie Class constructor
        Trie()
        {
            root = new TrieNode();
        }
        
        // PUBLIC METHODS : 

        // Inserts word into Trie
        void insert(string word);

        // Searches for word in trie
        bool search(string word);

        // Gets all words with given prefix
        vector<string> get_all_words(string word);

        // Suggests valid words
        void autosuggest(string word, LFUCache &cache);
};


void Trie::insert(string word)
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
    
    // Make isWord true to indicate complete word
    temp->isWord = true;
}


bool Trie::search(string word)
{   
    // If Trie is empty
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
    // If Trie is empty
    if(root == NULL)    return {};

    TrieNode* temp = root;

    for(int i=0; i<word.length(); i++)
    {
        // Index of char word[i] in TrieNode;
        int ind = (int)word[i] - ' ';

        // If child node at ind of TrieNode is NULL, then word is not a valid prefix
        if(temp->next[ind] == NULL) return {};

        temp = temp->next[ind];  
    }

    // Array to store valid suggestions
    vector<string> suggestions;

    // If this word is a valid prefix then BFS to find all words
    queue<pair<TrieNode*, string>> q;
    
    q.push(make_pair(temp, word));
    string currWord;

    // BFS  
    while(!q.empty())
    {
        temp = q.front().first;
        currWord = q.front().second;

        // If this word is a valid word, then add to all_words
        if(temp->isWord)
        {
            suggestions.push_back(currWord);
        }

        q.pop();

        for(int i=0; i<CHARACTERS; i++)
        {
            if(temp->next[i] != NULL)
            {
                // Next word with prefix as given word found
                string nextWord = currWord + char(i + ' ');
                q.push(make_pair(temp->next[i], nextWord));
            }
        }
    }

    return suggestions;
}


void Trie::autosuggest(string word, LFUCache &cache)
{
    // If empty word then no suggestions
    if(word.size() == 0)    return;

    // If valid word, then print all words for which given word is prefix
    vector<string> suggestions = get_all_words(word);

    // Print all suggestions

    if(suggestions.size() == 0)
    {
        cout << "There are no suggestions." << endl;
        return;
    }

    cout << "There are " << suggestions.size() << " suggestions for the prefix \"" << word << "\" : " << endl;

    // Rank suggestions according to cache
    int i;
    vector<pair<int, string>> rank;
    for(i=0; i<suggestions.size(); i++)
    {
        int rnk = cache.get(suggestions[i]);
        rank.push_back(make_pair(rnk, suggestions[i]));
    }
    
    sort(rank.rbegin(), rank.rend());

    // Display suggestions
    for(int i=0; i<suggestions.size(); i++)
    {
        cout << i+1 << ". " << rank[i].second << " " << rank[i].first << endl;
    }

    // Select valid suggestion
    int sel = -1;
    do {
        cout << "Select suggestion (" << 1 << "-" << i << ") : ";
        if (cin >> sel && (sel >= 1 && sel <= i)) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while(true);

    // Selected suggestion
    string selection = rank[sel-1].second;

    cout << "\nSelected suggestion : " << selection << endl << endl;

    // Update cache
    cache.put(selection);

    return;
}


void createDict(Trie*  trie){

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
    
    // Operations
    cout    << "Operations - \n" 
            << "1. Load Dictionary \n"
            << "2. Entry Field \n"
            << "3. Check Cache \n"
            << "4. Exit \n";
    
    return;
}


int main()
{
    Trie *trie = new Trie();
    LFUCache cache;

    while(1)
    {
        // Operations
        printOperations();

        int operation;
        do {
            cout << "\nSelect Operation : ";
            if (cin >> operation && (operation >= 1 && operation <= 4)) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        while(true);

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