/*
 * Name: Lucas Ryan
 * Date: April 24, 2016
 * Filename: gameofgames.c
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX_GAMES 8     // can have between 2 and 8 games
#define MAX_TIME 100000 // max time to travel to each game


int openTimes[MAX_GAMES + 1][2]; //holds opening and closing time
int travelTimes[MAX_GAMES + 1][MAX_GAMES + 1]; // stores time to get to locations
int numGames;

int solve(int gameSeq[], int playedGame[], int game);
int isPossiblePath(int gameSeq[], int playTime);
int findTime(int gameSeq[], int lowTime, int highTime);

int main() {
    int numCases, i;
    scanf("%d", &numCases);
    for(i = 0; i < numCases; i++) {
        int j;
        int gameSeq[MAX_GAMES] = {0, 0, 0, 0, 0, 0, 0, 0};    // generate possible games
        // keep track of game locations traveled to, set first element to one so know have been to that location
        int playedGame[MAX_GAMES + 1] = {1, 0, 0, 0, 0, 0, 0, 0, 0};

        scanf("%d", &numGames); // get number games to play
        // get the times each games is open to play
        for(j = 1; j <= numGames; j++)
            scanf("%d%d", &openTimes[j][0], &openTimes[j][1]);

        // get all times takes to get to each game
        for(j = 0; j <= numGames; j++) {
            int k;
            for(k = 0; k <= numGames; k++)
                scanf("%d", &travelTimes[j][k]);
        }
        printf("%d\n", solve(gameSeq, playedGame, 0));
    }
    return 0;
}

// uses associated array to keep track of the games that we
// played and generate the different ways to play the games
int solve(int gameSeq[], int playedGame[], int game) {
    if(game == numGames) return findTime(gameSeq, 0, MAX_TIME); // if reached all

    int i, maxPlayTime = 0;
    for(i = 0; i <= numGames; i++) {
        if(!playedGame[i]) {
            playedGame[i] = 1;      // flag game as played
            gameSeq[game] = i;      // store next which game playing in current game sequence

            int tryTime = solve(gameSeq, playedGame, game + 1);
            maxPlayTime = (maxPlayTime < tryTime) ? tryTime : maxPlayTime;  // update max time can be at game if needed
            playedGame[i] = 0;    // unflag game as being played
        }
    }
    return maxPlayTime;
}

// implement recersive binary search to find max average amount
// of time player can stay at the games
int findTime(int gameSeq[], int lowTime, int highTime) {
    if(highTime - 1 <= lowTime) return isPossiblePath(gameSeq, lowTime + 1) ? lowTime + 1 : lowTime;
    int middleTime = (lowTime + highTime) / 2;
    // check to see if we should increase or decrease amount of time can play each game
    if(isPossiblePath(gameSeq, middleTime)) return findTime(gameSeq, middleTime, highTime); // increment time if came back as a win
    else return findTime(gameSeq, lowTime, middleTime - 1); // else lower score
}

// checks if path we are using and time alloted self can be done
// where the real knapsack algorithm is taken care of
int isPossiblePath(int gameSeq[], int playTime) {
    int currentTime = travelTimes[0][gameSeq[0]], i; //not sure about this

    for(i = 0; i < numGames; i++) {
        if(openTimes[gameSeq[i]][1] < currentTime) return 0;    // if arrive after specific game closes then not valid path
        if(i == (numGames - 1)) return 1;       // reached the last game and its open to play
        if(currentTime < openTimes[gameSeq[i]][0]) currentTime = openTimes[gameSeq[i]][0];  // if get to game before opens change time to open time
        currentTime += (playTime + travelTimes[gameSeq[i]][gameSeq[i + 1]]);    // increment count time took to play this game and get to the next game
    }
    return 1;
}
