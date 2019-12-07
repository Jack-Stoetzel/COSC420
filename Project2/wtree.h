/* 	Tree of words
	The goal is to have a BST that is indexed by words from arxiv article abstracts.
	Each node will contain a word, and a linked list of article IDs whos abstracts contain that word.

    Setting root to merosymmetrical (middle of the /usr/share/dict/words file)
*/
#ifndef WTREE_H
#define WTREE_H

typedef struct INODE
{
    struct INODE* next;
    char* ID;
}IDNode;

typedef struct WNODE
{
    char* word;
    IDNode* IDList; // List of article IDs
    struct WNODE* left;
    struct WNODE* right;
}WordNode;

WordNode* makeRoot(WordNode** root)
{
    (*root) = (WordNode*) malloc(sizeof(WordNode));
    (*root) -> word = (char*) calloc(16, sizeof(char));
    strcpy((*root) -> word, "merosymmetrical");
    (*root) -> IDList = NULL;
    (*root) -> left = NULL;
    (*root) -> right = NULL;
}

WordNode* checkWord(WordNode** root, char* word, int* pos)
{
    WordNode* curr = *root;
    WordNode* prev = NULL;

    while(curr != NULL)
    {
        prev = curr;
        if(strcmp(curr -> word, word) == 0)
        {
            (*pos) = 0;
            return curr;
        }
        else if(strcmp(curr -> word, word) < 0)
        {
            (*pos) = -1;
            curr = curr -> left;
        }
        else if(strcmp(curr -> word, word) > 0)
        {
            (*pos) = 1;
            curr = curr -> right;
        }
    }
    return prev;
}

void wordInsert(WordNode** root, Article* data, char* word, int wordSize)
{
    // First need to check if word is already in the tree
    // Position will be used to determine if a new node is need
    int pos;
    WordNode* newNode;
    WordNode* temp = checkWord(root, word, &pos);
    if(pos != 0)
    {   // If the word is not yet in the tree, make a new tree node and save the word to it
        WordNode* prev = temp;
        newNode = (WordNode*) malloc(sizeof(WordNode));
        newNode -> word = (char*) calloc(wordSize, sizeof(int));
        strcpy(newNode -> word, word);
        newNode -> right = NULL;
        newNode -> left = NULL;

        // Search for new position of the wordNode

        if(pos == -1)
        {
            prev -> left = newNode;
        }
        else if (pos == 1)
        {
            prev -> right = newNode;
        }
    }
    else
    {
        newNode = temp;
    }
    // Make a LLnode for the ID
    IDNode* newListNode = (IDNode*) malloc(sizeof(IDNode));
    newListNode -> next = NULL;
    newListNode -> ID = (char*) calloc(data -> IDSize, sizeof(char));
    strcpy(newListNode -> ID, data -> ID);

    IDNode* curr = newNode -> IDList;
    if(curr == NULL)
    {
        curr = newListNode;
    }
    else
    {
        while(curr -> next != NULL && strcmp(curr -> ID, newListNode -> ID) != 0)
        {
            curr = curr -> next;
        }
        if(strcmp(curr -> ID, newListNode -> ID) == 0)
        {
            free(newListNode);
            return ;
        }
        else
        {
            curr -> next = newListNode;
        }
    }
}

#endif
