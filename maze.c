/*
 * Name: Lucas Ryan
 * Filename: maze.c V2
 * Date: March 24, 2016
 * Goal: implement Queue with BFS
 * to find a way out of a maze
*/

#include <stdio.h>
#include <stdlib.h>

#define WALL -2 //represent X's
#define ORIGIN 0 //represents S
#define DEFAULT -1 //represents -'s
#define BORDER -3 //represents ~'s
#define NO_WAY_OUT 999999
#define MOVE_SIZE 4

//struct nodes for queue
struct node {
    int row;
    int column;
    struct qq* nextNode;
};

//struct for queue data structure
struct qq {
    struct node * front;
    struct node * back;
};

int enqueue(struct qq* queue, int row, int column);
struct node dequeue(struct qq* queue);
int isEmpty(struct qq* queue);
int** readInput(int row, int column);
int solve(int** matrix, int row, int column);
int isValidLocation(int maxX, int maxY, int x, int y);
void freeMatrix(int** matrix, int size);

//to move around neighbors
int DX[MOVE_SIZE] = {-1, 0, 0, 1};
int DY[MOVE_SIZE] = {0, -1, 1, 0};

int glob_x = 0;     //COLUMN FOR STARTING POINT IN MAZE
int glob_y = 0;     //ROW FOR STARTING POINT IN MAZE

int main() {
    int numCases;
    scanf("%d", &numCases);
    int loop;
    for(loop = 0; loop < numCases; loop++) {
        int numRows, numCols;
        scanf("%d%d", &numRows, &numCols);
        int** matrix = readInput(numRows, numCols);     // generate maze
        int ans = solve(matrix, numRows, numCols);      // solve maze
        if (ans > NO_WAY_OUT) printf("-1\n");           // if ans > NO_WAY_OUT then no way out
        else printf("%d\n", ans);
        freeMatrix(matrix, numRows);                    // free up space
    }
    return 0;
}

// converts input with characters to integer representation of maze
// X: WALL, represent as -2 (not valid location)
// ~: DEFAULT, represent as -1 (has not been visited yet)
// S: ORIGIN, represent as 0
int** readInput(int row, int column) {
    int ** matrix = malloc(sizeof(int*) * row); // borrow memory for amount of rows in maze
    int row_i, col_j;
    for(row_i = 0; row_i < row; row_i++) {
        char line[column + 1];                  // store input for each row

        scanf("%s", line);
        matrix[row_i] = malloc(sizeof(int) * column);   // store amount of columns for each row in maze
        for(col_j = 0; col_j < column; col_j++) {
            if(line[col_j] == 'X'){             // if character is X then store -2 in matrix
                matrix[row_i][col_j] = WALL;
            } else if(line[col_j] == 'S') {     // if character is S then store 0 and save its coordinates
                glob_x = col_j;
                glob_y = row_i;
                matrix[row_i][col_j] = ORIGIN;
            } else if(line[col_j] == '~'){      // if character is S then store -3, reached end of maze
                matrix[row_i][col_j] = BORDER;
            }else {
                matrix[row_i][col_j] = DEFAULT; // else it's possible move locations
            }
        }
    }
    return matrix;  // return matrix each element represents "weight" to take it's path
}

// returns number moves to reach outside
// if return -1 then could not find way out
int solve(int** matrix, int row, int column) {

    struct qq* queue = (struct qq*) malloc(sizeof(struct qq));
    queue->front = NULL;            // initialize queue front to null
    queue->back = NULL;             // initialize queue front to null

    int min = NO_WAY_OUT;           // default to no way out
    int DX_i = 0, DY_i = 0;         // iterate through possible neighbors

    enqueue(queue, glob_y, glob_x); // save starting node onto the queue
    while(!isEmpty(queue)) {        // continue until the queue is empty
        struct node tmpNode = dequeue(queue); // get first node off the stack

        for(DX_i = DY_i = 0; DX_i < MOVE_SIZE; DX_i++, DY_i++) {
            int nextDX = tmpNode.column + DX[DX_i], nextDY = tmpNode.row + DY[DY_i];    // get neighbor spaces

            // if not valid location in matrix or reached the border/exit
            if(!isValidLocation(column, row, nextDX, nextDY) || matrix[nextDY][nextDX] == BORDER) {
                // find min amount of moves to get out of maze
                min = (matrix[tmpNode.row][tmpNode.column] < min) ? matrix[tmpNode.row][tmpNode.column] : min;
                while(!isEmpty(queue)) dequeue(queue);  // free up space
                break;      // break out of while loop
            }

            // if wall continue
            if(matrix[nextDY][nextDX] == WALL) {continue;}

            // if haven't visited location yet
            if(matrix[nextDY][nextDX] == DEFAULT) {
                // set flag as number of moves to get to location
                matrix[nextDY][nextDX] = matrix[tmpNode.row][tmpNode.column] + enqueue(queue, nextDY, nextDX);
                min++; // increment minimum amount of moves
            }
        }
    }
    free(queue->front);     // free up front node of queue
    free(queue->back);      // free up back node of queue
    free(queue);            // free up queue

    return min + 1;
}

// return 1 if saved successfully into queue, -1 if not
int enqueue(struct qq* queue, int row, int column) {
    //create node pointer
    struct node * newNode = (struct node*)malloc(sizeof(struct node));
    if(newNode == NULL) return -1;  // if not created then return -1 as warning

    newNode->row = row;             // set location in matrix[row][]
    newNode->column = column;       // set location in matrix[][column]
    newNode->nextNode = NULL;       // set next node pointing to to NULL

    if(queue->back != NULL)         // if queue not empty
        queue->back->nextNode = newNode;    // have last previous point to new last

    queue->back = newNode;          // back of queue is now newNode

    if(queue->front == NULL)        // if queue is empty
        queue->front = newNode;     // have front of queue be newNode
    return 1;
}

// returns struct node  of next node to be freed
// if queue is empty send default node of -1 and NUlL
struct node dequeue(struct qq* queue) {
    //if empty dequeue return struct node that represents empty
    if(queue->front == NULL) {
        struct node retNode = {-1, -1, NULL};
        return retNode;
    }

    struct node* freeNode = queue->front; //delete from front of queue
    struct node retNode = (*(*queue).front); //store data in front node to return
    queue->front = queue->front->nextNode; //point to next node in queue

    if(queue->front == NULL)    //if front is null then empty
        queue->back = NULL;     //set back to empty also

    free(freeNode);             //return memory back to computer
    return retNode;             //return front freed node's data
}

//checks if queue is empty: returns 1 if empty
int isEmpty(struct qq* queue) {
    return (queue->front == NULL);
}

//if valid location on maze
int isValidLocation(int maxX, int maxY, int x, int y) {
    return (0 <= x && x < (maxX) && 0 <= y && y < (maxY));
}

void freeMatrix(int** matrix, int size) {
    int i;
    for(i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);
}
