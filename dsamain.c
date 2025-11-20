#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

#define DICTIONARY_FILE "dictionary.txt"

void removeFromFile(const char *wordToRemove);
void loadDictionary(TrieNode *root);
void toLowerCase(char *str);

int main() {
    TrieNode *root = createNode();
    if (!root) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    
    loadDictionary(root);

    int choice;
    char word[100];
    char prefix[100];
    int freq;

    printf("=== AUTOCOMPLETE SYSTEM ===\n");

    while (1) {
        printf("\nMENU:\n");
        printf("1. Insert Word\n");
        printf("2. Remove Word\n");
        printf("3. Autocomplete (show all suggestions)\n");
        printf("4. Count Words\n");
        printf("5. Count words with prefix [Modification 1]\n");
        printf("6. Suggest longest word with prefix[Modification 2]\n");
        printf("7. Exit\n");
        

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter word to insert: ");
                scanf("%s", word);
                toLowerCase(word);

                printf("Enter frequency (for ranking): ");
                if (scanf("%d", &freq) != 1) {
                    printf("Invalid frequency.\n");
                    break;
                }

                insertWord(root, word, freq);
                printf("Inserted '%s' (freq=%d).\n", word, freq);

                FILE *fp;
                fp = fopen(DICTIONARY_FILE, "a");
                if (fp == NULL) {
                    perror("Error opening dictionary.txt");
                } else {
                    fprintf(fp, "%s %d\n", word, freq);
                    fclose(fp);
                    printf("Saved to dictionary.txt\n");
                }
                break;

            case 2:
                printf("Enter word to remove: ");
                scanf("%s", word);
                toLowerCase(word);

                if (removeWord(root, word)) {
                    printf("'%s' removed successfully.\n", word);
                    removeFromFile(word);
                } else {
                    printf("Word not found.\n");
                }
                break;

            case 3:
                printf("Enter prefix: ");
                scanf("%s", prefix);
                toLowerCase(prefix);
                autocomplete(root, prefix);
                break;
            
            case 4:
                printf("Total words in dictionary: %d\n", countWords(root));
                break;
            case 5:
                printf("Enter prefix: ");
                scanf("%s", prefix);
                toLowerCase(prefix);
                printf("Total words with prefix '%s': %d\n", prefix, countPrefix(root, prefix));
                break;

            case 6:
               printf("Enter prefix: ");
               scanf("%s", prefix);
               toLowerCase(prefix);
               suggestLongestWord(root, prefix);
               break;
            case 7:
                printf("Exiting... freeing memory.\n");
                freeTrie(root);
                return 0;

            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    freeTrie(root);
    return 0;
}


void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}


void loadDictionary(TrieNode *root) {
    FILE *fp = fopen(DICTIONARY_FILE, "r");
    if (fp == NULL) {
        printf("(No existing dictionary found, starting fresh)\n");
        return;
    }

    char word[100];
    int freq;
    while (fscanf(fp, "%s %d", word, &freq) == 2) {
        toLowerCase(word);
        insertWord(root, word, freq);
    }

    fclose(fp);
    printf("Loaded words from dictionary.txt successfully.\n");
}


void removeFromFile(const char *wordToRemove) {
    FILE *fp = fopen(DICTIONARY_FILE, "r");
    if (fp == NULL) {
        printf("dictionary.txt not found.\n"); 
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        perror("Error creating temp file");
        fclose(fp);
        return;
    }

    char fileWord[100];
    int freq;
    int found = 0;

    while (fscanf(fp, "%s %d", fileWord, &freq) == 2) {
        if (strcmp(fileWord, wordToRemove) != 0)
            fprintf(temp, "%s %d\n", fileWord, freq);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(DICTIONARY_FILE);
    rename("temp.txt", DICTIONARY_FILE);

    if (found)
        printf("Removed '%s' from dictionary.txt\n", wordToRemove);
    else
        printf("'%s' not found in dictionary.txt\n", wordToRemove);
}

