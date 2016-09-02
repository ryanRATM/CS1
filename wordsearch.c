//fileName: wordsearch.c
//Name: Lucas Ryan
//Date: Jan. 31 2016
//Purpose: Searches and returns all words in a
//given wordsearch.  Words returned are found
//in dictionary with files read in from a file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_LENGTH 4
#define MAX_LENGTH 19

const int DX_SIZE = 8;
const int DY[] = {-1, -1, -1,  0, 0,  1, 1, 1};
const int DX[] = {-1,  0,  1, -1, 1, -1, 0, 1};

int isValidLocation(int x, int y);
int searchDictionary(char** dictionary,  char* word, int low, int high);
char* generateWord(char** wordSearch, int startRow, int startCol, int directY, int directX, int length);
char** readInDictionary(char fileName[]);
void freeSpace(char** words, int size); //frees up borrowed memory

int numRows, numColumns, numWords;


int main() {

    //read from dictionary
    char** dictionary = readInDictionary("dictionary.txt");

    int numCases, i;
    scanf("%d", &numCases);
    for(i = 0; i < numCases; i++) {
        //get data
        int j;
        scanf("%d", &numRows);
        scanf("%d", &numColumns);
        char** wordSearch = malloc(sizeof(char*) * numRows);
        for(j = 0; j < numRows; j++) {
            wordSearch[j] = malloc(sizeof(char) * (numColumns + 1));
            scanf("%s", wordSearch[j]);
        }

        printf("Words Found Grid #%d:\n", (i+1));

        int x, y;
        for(y = 0; y < numRows; y++) {
            for(x = 0; x < numColumns; x++) {
                for(j = 0; j < DX_SIZE; j++) {
                    int k;
                    for(k = MIN_LENGTH; k <= MAX_LENGTH; k++){
                        char* possibleWord = generateWord(wordSearch, y, x, DY[j], DX[j], k); //generates a word if possible
                        //only want to print to console if not null and word is the dictionary
                        if((possibleWord != NULL) && searchDictionary(dictionary, possibleWord, 0, numWords - 1)) {
                            printf("%s\n", possibleWord);
                            free(possibleWord); //free up borrowed space
                        }
                    }
                }
            }
        }
        freeSpace(wordSearch, numRows); //free up borrowed space
    }

    freeSpace(dictionary, numWords); //free up borrowed space
    return 0;
}

//free up borrowed space
void freeSpace(char** words, int size) {
    int i;
    for(i = 0; i < size; i++) free(words[i]);
    free(words);
}

//read all words from dictionary
char** readInDictionary(char fileName[]) {
    FILE *file;
    file = fopen(fileName, "r");
    if(file == NULL) return NULL;

    int i;
    fscanf(file, "%d", &numWords); //read number of words in file
    char** dictionary = malloc(sizeof(char*) * numWords);
    for(i = 0; i < numWords; i++) {
        dictionary[i] = malloc(sizeof(char) * (MAX_LENGTH + 1)); //set to max possible string length
        fscanf(file, "%s", dictionary[i]);  //store word in dynamic array
    }
    fclose(file);  //close file
    return dictionary;
}

//check if x & y is a location in 2D char array
//x checks columns
//y checks rows
int isValidLocation(int x, int y) {
    return ((0 <= x) && (x < numColumns)) && ((0 <= y) && (y < numRows));
}


char* generateWord(char** wordSearch, int startRow, int startCol, int directY, int directX, int length) {
    //check if even possible to reach desired length
    if(isValidLocation(startCol + (length - 1) * directX, startRow + (length - 1) * directY)) {
      int i;
      char* words = malloc(sizeof(char) * (length + 1));
      for(i = 0; i < length; i++) {
        words[i] = wordSearch[startRow + (i * directY)][startCol + (i * directX)]; //next location
      }
      words[length] = '\0';
      return words;
    }
    return NULL; //when we can't make a word of desired length
}

//recursive binary search
//compare = -1 : search (mid + 1)|----->(high)
//compare = 1: search (low)<-----|(mid-1)
//compare = 0: have match
int searchDictionary(char** dictionary,  char* word, int low, int high) {
    if(low > high) return 0; //base case

    int mid = (low + high) / 2;  //location to compare
    int compare = strcmp(dictionary[mid], word);
    if(compare > 0) return searchDictionary(dictionary, word, low, mid - 1); //check words before word checked
    else if(compare < 0) return searchDictionary(dictionary, word, mid + 1, high); //check words after word checked
    return 1; //then matched
}
