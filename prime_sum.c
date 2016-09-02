// Name: Lucas Ryan
// FileName: prime_sum.c
// Date: Jan. 21, 2016
// Lab2.cpp : Defines the entry point for the console application.
// Implement Sieve of Eratosthene and memoization

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define NUM_PRIMES 10000

int* generatePrimeNumbers(); //generates first 10000 prime numbers
void appendNewPrimeSum(int* primeSumNumbers, int* primeNumbers, int position); //use memoization to store sum of prime numbers to lower runtime

int main() {

	//dynamic array to store prime numbers
    int* numPrimes = generatePrimeNumbers();
	//dynamic array to store prime number sums
	int* sumPrimes = (int*)calloc(NUM_PRIMES, sizeof(int)); //zero all the entries
	sumPrimes[0] = 2; //must set initial condition


	int numCases, i;
	scanf("%d", &numCases); //get number of cases
	for(i = 0; i < numCases; i++) { //foreach case
		int numPrimesToSum;
		scanf("%d", &numPrimesToSum); //sum of how many primes

		//check if already have the requested sum stored if not then get it and store any sum upto previous sum
		if(sumPrimes[numPrimesToSum - 1] == 0) appendNewPrimeSum(sumPrimes, numPrimes, numPrimesToSum);

		printf("%d\n", sumPrimes[numPrimesToSum - 1]); //print out requested sum of primes

	}

	//free up space stole
	free(numPrimes);
	free(sumPrimes);

    return 0;
}



//implement Sieve of Eratosthenes alg to find first 10,000 prime numbers
//primeNumbers to store prime numbers
//primeNumbersBool to say if a given index is prime or not
//return dynamic integer array of prime numbers
int* generatePrimeNumbers() {

	int* primeNumbers = (int*)malloc(sizeof(int) * NUM_PRIMES); //represents prime numbers
	int* primeNumbersBool = (int*)malloc(sizeof(int) * 500000);  //represents positive numbers
	int i, j;

	//initial assume every number is prime
	for(i = 0; i < 500000; i++) {
		primeNumbersBool[i] = 1;
	}

	//first prime is 2, if start with 0 then have infinte loop in nest for loop, and everything is a multiple of 1
	//only iterate through sqrt(x) of list since everything after sqrt(x) has already been noted
	for (i = 2; i < sqrt(500000); i++) {
		//if the number is a prime
		if (primeNumbersBool[i] == 1) {
			//flag multiples of the prime as a composite
			for (j = 0; ((i * i) + (j * i)) < 500000; j++) { //go to every multiple of i
				primeNumbersBool[(i * i) + (j * i)] = -1;
			}
		}
	}

    //if value at index i is 1 then the value of i is a prime number
	for (i = 2, j = 0; (j < NUM_PRIMES); i++) {
		if (primeNumbersBool[i] == 1) {
			primeNumbers[j] = i; //store value of i in array that stores first 10000 prime numbers
			j++;
		}
	}

	free(primeNumbersBool); //return space to computer
	return primeNumbers;
}


//primeSumNumbers to store sum of prime numbers
//position to say where should be stored
void appendNewPrimeSum(int* primeSumNumbers, int* primeNumbers, int position) {
	//add up to last position saved from primeNumbers
	int i;
	//once reach a 0 then fill it up
	for (i = 0; i < position; i++)
		if (primeSumNumbers[i] == 0) primeSumNumbers[i] = primeSumNumbers[i - 1] + primeNumbers[i];
}
