/*
 * Name: Lucas Ryan
 * Filename: avlTree.c
 * Date: April 12, 2016
*/

#include <stdio.h>
#include <stdlib.h>


typedef struct binTreeNode {
    int value;
    int height;
    struct binTreeNode* left;
    struct binTreeNode* right;
} binaryNode;
int isBalanced; // flag for if tree is balanced

char* runCase(int* data, int size);     // test case
binaryNode* insert(binaryNode* root, int data);    // insertNode into tree and check if balanced tree
void freeMemory(binaryNode* root);

int main() {
    int loop, numCases;
    scanf("%d", &numCases);
    for( loop = 0; loop < numCases; loop++) {
        int size, i;
        int* numbers;

        scanf("%d", &size);
        numbers = calloc(size, sizeof(int));
        for(i = 0; i < size; i++) {
            scanf("%d", &numbers[i]);
        }
        printf("Tree #%d: %s\n", loop + 1, runCase(numbers, size));
        free(numbers);
    }
    return 0;
}

// run each case for inserting nodes into a tree,
// stop inserting nodes once hit first unbalance
// since would be wast to continue inserting
char* runCase(int* data, int size) {
    binaryNode* root = NULL;
    int i;
    isBalanced = 1;     // set to default of tree balanced every case
    for(i = 0; i < size; i++) {
        // insert node into tree
        root = insert(root, data[i]);
        if(isBalanced == 0) {   // once hit our first unbalanced node stop
            freeMemory(root);   // give back memory
            return "REMOVE";    // return not balanced
        }
    }
    freeMemory(root);   // give back memory
    return "KEEP";  // return balanced tree
}

// insert node into binary search tree after
// insertion check to see if the current node
// is unbalanced, if so then notify the user
binaryNode* insert(binaryNode* root, int data) {
    binaryNode* temp;
    if(root == NULL) {  // if empty location (leaf node's child) create new node and return
        temp = malloc(sizeof(binaryNode));
        temp->height = -1;
        temp->left = NULL;
        temp->right = NULL;
        temp->value = data;
        return temp;
    }
    if(data <= root->value) {
        root->left = insert(root->left, data);      // insert on left side of node
        // update to max height if left height is greater than
        root->height = (root->height < root->left->height + 1) ? root->left->height + 1: root->height;
    } else {
        root->right = insert(root->right, data);    // insert on right side of node
        // update to max height if right height is greater than
        root->height = (root->height < root->right->height + 1) ? root->right->height + 1: root->height;
    }

    if(root->left == NULL) {
        if(root->right->height > -1) isBalanced = 0;   // set as unbalanced if no children on left and right has children
    } else if(root->right == NULL) {
        if(root->left->height > -1) isBalanced = 0;    // set as unbalanced if no children on right and left has children
    } else {
        int diffHeight = root->left->height - root->right->height;  // find difference in height between left and right
        if(diffHeight < -1 || 1 < diffHeight) isBalanced = 0;  // set as unbalanced if greater than difference of 1
    }
    return root;
}

// free up tree using post order traversal
void freeMemory(binaryNode* root) {
    if(root != NULL) {  // free memory if an actual location in tree
        freeMemory(root->left);
        freeMemory(root->right);
        free(root);
    }
}
