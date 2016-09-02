//Name: Lucas Ryan
//Date: Feb 23, 2016
//Filename: riskpowmillion.c

#include <stdio.h>
#include <stdlib.h>

void mergeSort(int* array, int low, int high);
void merge(int* array, int low, int middle, int high);
int maxDefWins(int* defender, int* attacker, int length);

int main() {
    int numCases, i, j;
    int *defenders, *attackers;

    scanf("%d", &numCases);
    for(i = 0; i < numCases; i++) {
        int numBattles, j;
        scanf("%d", &numBattles);

        //borrow some memory
        defenders = calloc(numBattles, sizeof(int));
        attackers = calloc(numBattles, sizeof(int));

        //read in input
        for(j = 0; j < numBattles; j++) scanf("%d", &attackers[j]);
        for(j = 0; j < numBattles; j++) scanf("%d", &defenders[j]);

        //sort both
        mergeSort(attackers, 0, numBattles - 1);
        mergeSort(defenders, 0, numBattles - 1);

        //print max possible of wins
        printf("%d\n", maxDefWins(defenders, attackers, numBattles));
    }
    //free up borrowed space
    free(defenders);
    free(attackers);
    return 0;
}

//sorts a given array implementing divide and conquer
//once sort left and right then merge them together and have fully sorted array
void mergeSort(int* array, int low, int high){
    if(low < high) {
        int mid = (low + high) / 2;

        mergeSort(array, low, mid);       //do the left side
        mergeSort(array, mid + 1, high);  //do the right side
        merge(array, low, mid + 1, high); //merge the left and right
    }
}

//where the sort really takes place
//note must check if counter is still in a valid location of array or else big trouble
void merge(int* array, int low, int middle, int high){
    int* tempArr;
    int i, length, arr1Counter, arr2Counter, tempArrCounter;

    //allocate proper amount of space for array
    length = high - low + 1;
    tempArr = calloc(length, sizeof(int)); //zero out the space

    //counters for arrays
    arr1Counter = low;
    arr2Counter = middle;
    tempArrCounter = 0;


    //loop until reach end of both sub arrays
    while((arr1Counter < middle) || (arr2Counter <= high)) {
        //if 1st array element less than 2nd array element or at end of 2nd array insert
        if((arr2Counter > high) || ((arr1Counter < middle) && (array[arr1Counter] <= array[arr2Counter]))) {
            tempArr[tempArrCounter] = array[arr1Counter]; //insert
            tempArrCounter++; //increment counters
            arr1Counter++;
        } else { //else copy rest of 2nd array into tempArray
            tempArr[tempArrCounter] = array[arr2Counter];
            tempArrCounter++;
            arr2Counter++;
        }
    }
    //copy tempArray elements into given array
    for(i = low; i <= high; i++) array[i] = tempArr[i - low];
    //free up space
    free(tempArr);
}

//compares sorted defense and sorted attack array
//use linear search principle to find max possible wins
int maxDefWins(int defender[], int attacker[], int length) {

    int defenseCounter = 0, attackCounter = 0, win = 0; //set counters to zero
    //continue as long as haven't reached end of either of defender or attacker array
    while(defenseCounter < length && attackCounter <= length) {
        //if defender at given index is equal to or greater than attackers value then win
        //increment all counters so go to next army to battle against
        if(defender[defenseCounter] >= attacker[attackCounter]) {
            win++;
            attackCounter++;
            defenseCounter++;
        } else { //if defender's army is less than attacker's army
            defenseCounter++; //increment to next stronger defense army
        }
    }
    return win;
}
