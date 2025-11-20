#ifndef TRIE_H
#define TRIE_H
#include <stdbool.h>
#define ALPHABET_SIZE 26
#define MAX_WORD_LEN 100
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
    int frequency;
} TrieNode;
typedef struct PQNode {
    char word[MAX_WORD_LEN];
    int freq;
} PQNode;
typedef struct PriorityQueue {
    PQNode *array;
    int size;
    int capacity;
} PriorityQueue;
// Trie functions
TrieNode *createNode();
void insertWord(TrieNode *root, const char *word, int freq);
bool removeWord(TrieNode *root, const char *word);
void autocomplete(TrieNode *root, const char *prefix);
void freeTrie(TrieNode *root);
int countWordsUtil(TrieNode *node);
int countWords(TrieNode *root);
//Modifications
int countPrefix(TrieNode *root, const char *prefix);
void suggestLongestWord(TrieNode *root, const char *prefix);

// PQ functions
PriorityQueue *createPQ(int capacity);
void pqInsert(PriorityQueue *pq, const char *word, int freq);
void pqSort(PriorityQueue *pq);
void freePQ(PriorityQueue *pq);
#endif
