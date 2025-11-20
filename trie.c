#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

// Create Trie Node
TrieNode *createNode() {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    node->isEndOfWord = false;
    node->frequency = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}                                                    

// Insert word into Trie
void insertWord(TrieNode *root, const char *word, int freq) {
    TrieNode *curr = root;
    for (int i = 0; word[i]; i++) {
        char c = tolower(word[i]);
        if (c < 'a' || c > 'z') continue;
        int idx = c - 'a';
        if (!curr->children[idx])
            curr->children[idx] = createNode();
        curr = curr->children[idx];
    }
    curr->isEndOfWord = true;
    curr->frequency = freq;
}

// Remove a word 
bool removeWord(TrieNode *root, const char *word) {
    if (!root) return false;
    TrieNode *stack[MAX_WORD_LEN];
    int path[MAX_WORD_LEN], len = 0;
    TrieNode *curr = root;

    for (int i = 0; word[i]; i++) {
        int idx = tolower(word[i]) - 'a';
        if (!curr->children[idx]) return false;
        stack[len] = curr;
        path[len++] = idx;
        curr = curr->children[idx];
    }

    if (!curr->isEndOfWord) return false;
    curr->isEndOfWord = false;

    // Cleanup unused nodes
    for (int i = len - 1; i >= 0; i--) {
        TrieNode *parent = stack[i];
        int idx = path[i];
        TrieNode *child = parent->children[idx];

        bool empty = true;
        for (int j = 0; j < ALPHABET_SIZE; j++)
            if (child->children[j]) empty = false;

        if (empty && !child->isEndOfWord) {
            free(child);
            parent->children[idx] = NULL;
        } else break;
    }
    return true;
}

// PQ Helpers
PriorityQueue *createPQ(int capacity) {
    PriorityQueue *pq = malloc(sizeof(PriorityQueue));
    pq->array = malloc(sizeof(PQNode) * capacity);
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void pqInsert(PriorityQueue *pq, const char *word, int freq) {
    if (pq->size >= pq->capacity) return;
    strcpy(pq->array[pq->size].word, word);
    pq->array[pq->size].freq = freq;
    pq->size++;
}

void pqSort(PriorityQueue *pq) {
    for (int i = 0; i < pq->size - 1; i++) {
        for (int j = 0; j < pq->size - i - 1; j++) {
            if (pq->array[j].freq < pq->array[j + 1].freq) {
                PQNode tmp = pq->array[j];
                pq->array[j] = pq->array[j + 1];
                pq->array[j + 1] = tmp;
            }
        }
    }
}

void freePQ(PriorityQueue *pq) {
    free(pq->array);
    free(pq);
}

// Helper recursive function
static void collectWords(TrieNode *node, char *prefix, PriorityQueue *pq) {
    if (!node) return;
    if (node->isEndOfWord)
        pqInsert(pq, prefix, node->frequency);

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            int len = strlen(prefix);
            char newPrefix[MAX_WORD_LEN];
            strcpy(newPrefix, prefix);
            newPrefix[len] = 'a' + i;
            newPrefix[len + 1] = '\0';
            collectWords(node->children[i], newPrefix, pq);
        }
    }
}

// Autocomplete
void autocomplete(TrieNode *root, const char *prefix) {
    TrieNode *curr = root;
    for (int i = 0; prefix[i]; i++) {
        int idx = tolower(prefix[i]) - 'a';
        if (!curr->children[idx]) {
            printf("No suggestions found for '%s'.\n", prefix);
            return;
        }
        curr = curr->children[idx];
    }

    PriorityQueue *pq = createPQ(100);
    collectWords(curr, (char *)prefix, pq);
    pqSort(pq);

    printf("\nSuggestions for '%s':\n", prefix);
    if (pq->size == 0)
        printf("  (none found)\n");
    for (int i = 0; i < pq->size; i++)
        printf("  %s (freq=%d)\n", pq->array[i].word, pq->array[i].freq);

    freePQ(pq);
}

// Free entire Trie
void freeTrie(TrieNode *root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        freeTrie(root->children[i]);
    free(root);
}
int countWordsUtil(TrieNode *node) {
    if (!node) return 0;
    int count = node->isEndOfWord ? 1 : 0;
    for (int i = 0; i < 26; i++)
        count += countWordsUtil(node->children[i]);
    return count;
}

int countWords(TrieNode *root) {
    return countWordsUtil(root);
}

//MODIFICATIONS

int countPrefixUtil(TrieNode *node) {
    if (!node) return 0;

    int count = node->isEndOfWord ? 1 : 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        count += countPrefixUtil(node->children[i]);

    return count;
}

int countPrefix(TrieNode *root, const char *prefix) {
    TrieNode *curr = root;

    for (int i = 0; prefix[i]; i++) {
        char c = tolower(prefix[i]);
        if (c < 'a' || c > 'z')
            continue; 

        int idx = c - 'a';
        if (!curr->children[idx])
            return 0; 

        curr = curr->children[idx];
    }

    
    return countPrefixUtil(curr);
}

void findLongestWord(TrieNode *node, char *currentWord, char *longestWord) {
    if (!node) return;

    if (node->isEndOfWord && strlen(currentWord) > strlen(longestWord))
        strcpy(longestWord, currentWord);

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            int len = strlen(currentWord);
            currentWord[len] = 'a' + i;
            currentWord[len + 1] = '\0';

            findLongestWord(node->children[i], currentWord, longestWord);

            currentWord[len] = '\0'; 
        }
    }
}

void suggestLongestWord(TrieNode *root, const char *prefix) {
    TrieNode *curr = root;

    
    for (int i = 0; prefix[i]; i++) {
        char c = tolower(prefix[i]);
        if (c < 'a' || c > 'z')
            continue;

        int idx = c - 'a';
        if (!curr->children[idx]) {
            printf("No words found with prefix '%s'.\n", prefix);
            return;
        }
        curr = curr->children[idx];
    }

    char currentWord[MAX_WORD_LEN];
    char longestWord[MAX_WORD_LEN];

    strcpy(currentWord, prefix);
    strcpy(longestWord, prefix);

    findLongestWord(curr, currentWord, longestWord);

    if (strcmp(longestWord, prefix) == 0)
        printf("No longer word found beyond prefix '%s'.\n", prefix);
    else
        printf("Longest word for prefix '%s' is: %s\n", prefix, longestWord);
}
