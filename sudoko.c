//Name: Lucas Ryan
//FileName: sudoko.c
//Date: Jan. 21, 2016
//checks if a given sudoko solution is valid

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 9
#define TRUE 1
#define FALSE 0

int isValidRow(int[][SIZE], int row);
int isValidColumn(int[][SIZE], int column);
int isValidCubic(int array[][SIZE], int cubic);
void checkSudokoSolution(int array[][SIZE]);
int checkFrequency(int freq[]);

int main() {
	int cases, i;

	scanf("%d", &cases);

	for (i = 1; i <= cases; i++) {
		int row, column;
		int array[SIZE][SIZE];
		for (row = 0; row < SIZE; row++) {
			char line[SIZE + 1]; //need 10 for null character
			scanf("%s", line);
			for (column = 0; column < SIZE; column++) { //foreach character scanned from line
				array[row][column] = line[column] - '0'; //convert from ASCII to int
			}
		}
		checkSudokoSolution(array);
	}
	return 0;
}

//checks if a sudoko solution is valid
//by checking rows, columns, and cubics
void checkSudokoSolution(int array[][SIZE]) {
	int i;
	char* word = malloc(sizeof(char) * 4);
	strcpy(word, "YES"); //assume given array is correct

	for (i = 0; i < SIZE; i++) {
            //if any of the rows, columns, or cubics are valid then break
		if(!isValidColumn(array, i) || !isValidRow(array, i) || !isValidCubic(array, i)) {
			strcpy(word, "NO"); //since not solution then set to NO
			break;
		}
	}
	printf("%s\n", word);
	free(word); //free up space
}

//checks to see if a cubic is valid using a frequency array
int isValidCubic(int array[][SIZE], int cubic) {
	int row, column, i, j;
	int freq[SIZE + 1];  //frequency array is size + 1 so value in array at the location maps an index in freq
	row = (cubic % (int)sqrt(SIZE)) * sqrt(SIZE); //coordinates to a given of either 0, 3, 6
	column = cubic - (row / sqrt(SIZE)); //coordinate to a given column of either 0, 3, 6

	for (i = 0; i < SIZE + 1; i++) freq[i] = 0; //must set to zero since each element is set to a random value

	for(i = 0; i < sqrt(SIZE); i++) //foreach row in cubic
		for (j = 0; j < sqrt(SIZE); j++) //foreach column in row of cubic
			freq[array[row + i][column + j]]++;

	return checkFrequency(freq);
}

//checks to see if a row is valid using a frequency array
int isValidRow(int array[][SIZE], int row) {
	int i;
	int freq[SIZE + 1];
	for(i = 0; i < SIZE + 1; i++) freq[i] = 0;

	for (i = 0; i < SIZE; i++)
		freq[array[row][i]]++;

	return checkFrequency(freq);
}

//checks to see if a column is valid using a frequency array
int isValidColumn(int array[][SIZE], int column) {
	int i;
	int freq[SIZE + 1];
	for (i = 0; i < SIZE + 1; i++) freq[i] = 0;

	for (i = 0; i < SIZE; i++)
		freq[array[i][column]]++;

	return checkFrequency(freq);
}

//checks frequency of a row, column, or cubic is valid
int checkFrequency(int freq[]) {
	int i;
	//start at 1 since 0 isn't a valid number in sudoko
	for (i = 1; i < SIZE + 1; i++)
		if(freq[i] != 1) return FALSE; //if value at index {1 - 9} isn't 1 then there was either no occurance or more than one occurance

	return TRUE;
}
