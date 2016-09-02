//Name: Lucas Ryan
//Date: Feb. 18, 2016
//Filename: mastermind.c

#include <stdio.h>
#include <stdlib.h>

int checkBlackPeg(int* correct, int* guess);
int* colorFrequency(int* guess);
int totalNumberPegs(int* correctFreq, int* guessFreq);
int numberWhitePegs(int* correct, int* guess);
int solveGame(int* combination, int position);
int min(int a, int b);
int verifyPlay(int* combo);

int** guesses;
int* blackPegs;
int* whitePegs;
int numSlots, numColors, numPlays;

int main() {
    int numCases, i;

    scanf("%d", &numCases);
        for (i=0; i<numCases; i++) {

        int j, k;
        scanf("%d%d%d", &numSlots, &numColors, &numPlays);

        //borrow memory for guesses, and pegs
        guesses = calloc(numPlays, sizeof(int*));
        blackPegs = calloc(numPlays, sizeof(int));
        whitePegs = calloc(numPlays, sizeof(int));

        //request and clear out memory for each guess
        for(j = 0; j < numPlays; j++)
            guesses[j] = calloc(numSlots, sizeof(int));

        //read in guess(inner-for loop), black, white pegs (outter-for loop)
        for (j=0; j<numPlays; j++) {
            for (k=0; k<numSlots; k++)
                scanf("%d", &guesses[j][k]);
            scanf("%d%d", &blackPegs[j], &whitePegs[j]);
        }

        //create array to store combination
        int* initialGuess = calloc(numSlots, sizeof(int));

        //call function that begins to solve combination
        printf("%d\n", solveGame(initialGuess, 0));

        //give back the borrow memory
        for (j=0; j<numPlays; j++)
            free(guesses[j]);
        free(guesses);
        free(blackPegs);
        free(whitePegs);
        free(initialGuess);
    }
    return 0;
}

//returns the number of perfect matches(black pegs)
//only have perfect match if the same color is in the
//same spot in correct and guess
int checkBlackPeg(int* correct, int* guess) {
    int i, blackCount;
    for(i = 0, blackCount = 0; i < numSlots; i++)
        if(correct[i] == guess[i]) blackCount++;
    return blackCount;
}

//returns array holding frequency of data
//each index is a color and the value at the
//index represents how many times the color is
//in the given combination
int* colorFrequency(int* combination) {
    int* freq = calloc(numColors, sizeof(int));
    int i;
    for(i = 0; i < numSlots; i++) freq[combination[i]]++;
    return freq;
}

//returns a count of the total amount
//(black and white) pegs guess has to correct
int totalNumberPegs(int* correct, int* guess){
    //generate frequency table for correct and guessed combination
    int* correctFreq = colorFrequency(correct);
    int* guessFreq = colorFrequency(guess);
    int i;
    int inCorrectComb = 0;

    for(i = 0; i < numColors; i++){
        //if no freq match then 0, if frequency go with lower one
        inCorrectComb += min(correctFreq[i], guessFreq[i]);
    }
    //give back borrowed memory
    free(correctFreq);
    free(guessFreq);
    return inCorrectComb;
}

//return number of white pegs in guessed combination
//finds total number pegs and subtracts number black pegs
//(whitePegs + blackPegs) - blackPegs = whitePegs
int numberWhitePegs(int* correct, int* guess) {
    return totalNumberPegs(correct, guess) - checkBlackPeg(correct, guess);
}

//like printOdometer from in-class
//sequentially generate a combination
//once have a combination check to see if valid guess
//return numberTimes had to run
int solveGame(int* combination, int position) {
    //base case: array is full
    if(position == numSlots) {
        return verifyPlay(combination); //check combination
    }

    int i, sum;
    for (i = 0, sum = 0; i < numColors; i++) {
        combination[position] = i; //generate combinations
        sum += solveGame(combination, position + 1); //recursive call
    }
    return sum;
}

//returns weather or not given combination is the correct one
//by comparing to each guess' response(black and white pegs)
int verifyPlay(int* combo) {

    int i;

    //loop through each guess given
    for (i = 0; i < numPlays; i++) {

        //compare combination to guess
        int numBlackPegs = checkBlackPeg(guesses[i], combo);
        int numWhitePegs = numberWhitePegs(guesses[i], combo);

        //if number blackPegs and whitePegs don't match associated guesses's pegs, then not a match
        if (numBlackPegs != blackPegs[i] || numWhitePegs != whitePegs[i])
            return 0;
    }
    //since matches same black and white pegs for each guess then must be correct combination
    return 1;
}

//find minimum number between 2 numbers
int min(int a, int b) {
    return (a <= b) ? a : b;
}
