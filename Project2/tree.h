// Tree One: Tree of IDs
// Nodes include Articles and index by thier ID

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ARTICLES{
    char ID [32];
    char title[256];
    char authors[256];
    char abstract[4096];

}Articles;

typedef struct NODE
{
    Articles data; //node will store an integer
    struct NODE* right; // right child
    struct NODE* left; // left child
}wordNode;

//function to create a node
wordNode* newNode(Articles x)
{
    wordNode* p;
    p = malloc(sizeof(wordNode));
    p -> data = x;
    p -> left = NULL;
    p -> right = NULL;

    return p;
}

wordNode* search(wordNode *root, Articles x)
{
    if(root == NULL || strcmp((root -> data).ID, x.ID) == 0){ //if root -> data is x then the element is found
        return root;
    }
    else if(strcmp(x.ID, (root -> data).ID) == 1){ // x is greater, so we will search the right subtree
        return search(root -> right, x);
    }
    else{ //x is smaller than the data, so we will search the left subtree
        return search(root -> left, x);
    }
}

wordNode* insert(wordNode *root, Articles x)
{
    //searching for the place to insert
    if(root == NULL){
        return newNode(x);
    }
    else if(strcmp(x.ID, (root -> data).ID) == 1){ // x is greater. Should be inserted to right
        root -> right = insert(root -> right, x);
    }
    else{ // x is smaller should be inserted to left
        root -> left = insert(root -> left, x);
    }
    return root;
}
