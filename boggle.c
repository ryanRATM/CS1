/*
 * Name: Lucas Ryan
 * Date: April 10, 2016
 * Filename: boggle.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 4
#define DIRECTION 8
#define MAX_LENGTH 16

// possible movements
const int DX[] = {-1,-1,-1,0,0,1,1,1};
const int DY[] = {-1,0,1,-1,1,-1,0,1};

struct trie {
    int isWord;
    struct trie* nextLetter[26];    // use array so don't have to worry about memory allocation
};

struct trie* loadDictionary();
struct trie* insert(struct trie* root, char word[], int index);
int inbounds(int x, int y);
int isWord(char word[], struct trie* dictionary);
int isPrefix(char word[], struct trie* dictionary);
void result(char board[][SIZE], struct trie* dictionary, int round);
void solve(char prefix[], int used[][SIZE], int curX, int curY, char board[][SIZE], struct trie* dictionary);
void freetree(struct trie* root);


int main() {
    struct trie* dictionary = loadDictionary();
    int numRounds, i;
    scanf("%d", &numRounds);
    for (i = 0; i < numRounds; i++) {
        char board[SIZE][SIZE];
        int j, k;
        char line[SIZE + 1];
        for (j = 0; j < SIZE; j++) {
            scanf("%s", line);  // read in line
            for(k = 0; k < SIZE; k++) {
                board[j][k] = line[k];  // copy each character read in into boggle board
            }
        }
        result(board, dictionary, i + 1);   // find all possible words
    }
    freetree(dictionary);   // return borrowed memory
    return 0;
}

// return trie holding all the words found in
// dictionary.txt that are within bounds of boggle
// length words to shorten time between inserting
// and searching through words that can't be used
struct trie* loadDictionary() {
    FILE* dictFile = fopen("dictionary.txt", "r");
    struct trie* dictionary = malloc(sizeof(struct trie));  // Set up the dictionary structure.
    int numWords;
    int i;

    for (i = 0; i < 26; i++) {
        dictionary->nextLetter[i] = NULL;   // set each child to null
    }
    dictionary->isWord = 0; // set default to not being a word

    fscanf(dictFile, "%d", &numWords);
    for (i = 0; i < numWords; i++) {
        char word[100];
        fscanf(dictFile, "%s", word);

        int wordLength = strlen(word);  // get word length
        if (3 <= wordLength && wordLength <= MAX_LENGTH)
            dictionary->nextLetter[(int)(word[0] - 'a')] = insert(dictionary->nextLetter[(int)(word[0] - 'a')], word, 1);
    }
    fclose(dictFile);    // close file
    return dictionary;
}

// insert one letter at a time until reach end of
// word read from dictionary.txt, then flag as a word
// return root of child just inserted
struct trie* insert(struct trie* root, char word[], int index) {
    if (root == NULL) { // if empty
        struct trie* newnode = malloc(sizeof(struct trie));
        int i;
        for (i = 0; i < 26; i++)
            newnode->nextLetter[i] = NULL;  // set all children to null

        if (index == strlen(word)) {
            newnode->isWord = 1;    // if index matches length of word full stored it into dictionary
        } else {
            newnode->isWord = 0;    // don't have a match, continue inserting letters
            newnode->nextLetter[(int)(word[index]-'a')] = insert(newnode->nextLetter[(int)(word[index]-'a')], word, index + 1);
        }
        return newnode;
    }
    // if dictionary isnt empty
    if (index == strlen(word))
        root->isWord = 1;   // fully stored word into dictionary
    else                    // else, keep working on it, store next letter in word
        root->nextLetter[(int)(word[index]-'a')] = insert(root->nextLetter[(int)(word[index]-'a')], word, index+1);
    return root;
}

// solves boggle to check all possible words to be made
// from boggle board, go through each row and column to
// start word from each location and mark letters already
// used during making current word
void result(char board[][SIZE], struct trie* dictionary, int round) {
    printf("Words for Game #%d:\n", round);
    char word[MAX_LENGTH + 1];    // extra location for terminating 0
    int usedLetter[SIZE][SIZE];
    int row, column;
    word[0] = '\0';

    // mark all flags of letters as not being used yet
    for (row = 0; row < SIZE; row++) {
        for (column = 0; column < SIZE; column++)
            usedLetter[row][column] = 0;
    }

    // loop through entire boggle board so each index can be beginning of word
    for (row = 0; row < SIZE; row++) {
        for (column = 0; column < SIZE; column++) {
            word[0] = board[row][column];
            word[1] = '\0';   // set index to null so doesn't read past
            usedLetter[row][column] = 1; // set as being used again
            solve(word, usedLetter, row, column, board, dictionary);
            usedLetter[row][column] = 0; // set as not being used again
        }
    }
    printf("\n\n");

}

// where get word if there is a word, if not word
// check if prefix, if prefix then continue search
// to make a word, else don't have word so do nothing
void solve(char word[], int used[][SIZE], int currX, int currY, char board[][SIZE], struct trie* dictionary) {

    if (isWord(word, dictionary)) printf("%s\n", word); // if word then print
    if (isPrefix(word, dictionary)) {     // break out since no words can be made
        int length = strlen(word), i;
        // floodfill to go through neighboring pieces
        for (i = 0; i < DIRECTION; i++) {
            int nextX = currX+DX[i];
            int nextY = currY+DY[i];
            // continue only if valid location and letter isn't being used
            if (inbounds(nextX, nextY) && !used[nextX][nextY]) {
                word[length] = board[nextX][nextY];
                word[length + 1] = '\0';  // set next location to be end of word for string operations
                used[nextX][nextY] = 1; // set location as being used
                solve(word, used, nextX, nextY, board, dictionary);   // continue search for word
                word[length] = '\0';    // set terminating 0
                used[nextX][nextY] = 0; // set letter as free to use
            }
        }
    }
}

// searches to end of word, if get to end of trie
// before reaching end of word then know no word can
// be made, if get to end of word before getting to
// leaf node then word can be made from this prefix
int isPrefix(char word[], struct trie* dictionary) {
    int i;
    int length = strlen(word);
    // Go through each letter.
    for (i = 0; i < length; i++) {
        // if no children then can't be a word
        if (dictionary->nextLetter[(int)(word[i]-'a')] == NULL)
            return 0;
        dictionary = dictionary->nextLetter[(int)(word[i]-'a')];    // move onto next child
    }
    return 1;   // if got to end of word then must be a prefix for word in dictionary
}

// returns weather or not a given word is found in
// dictionary using depth-first-search to traverse
// dictionary trie once get to end of word return
// that node's status of being a word in trie
int isWord(char word[], struct trie* dictionary) {
    int i;
    int length = strlen(word);

    for (i = 0; i < length; i++) {
        // if no children then can't be a word
        if (dictionary->nextLetter[(int)(word[i]-'a')] == NULL)
            return 0;
        dictionary = dictionary->nextLetter[(int)(word[i]-'a')];    // move onto next child
    }
    return dictionary->isWord;  // if get to end return weather or not we have a word
}

// check if within bounds of boggle board
int inbounds(int column, int row) {
    return (0 <= column && column < SIZE) && ( 0 <= row && row < SIZE);
}

// give back memory borrowed for the dictionary
// using depth first search
void freetree(struct trie* root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < 26; i++)    // recursively free up children
            freetree(root->nextLetter[i]);
        free(root);     // free up current node after no children
    }
}
