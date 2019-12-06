// Tree One: Tree of IDs
// Nodes include Articles and index by thier ID

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum COLOR{
	RED, BLACK
}Color;

typedef struct ARTICLE{
    char* ID;
	int IDSize;

    char* title;
	int titleSize;

    char* authors;
	int authorSize;

}Article;

typedef struct ANODE
{
    Article* article;
    Color color;
    struct ANODE *left, *right, *parent;
}ArticleNode;


void leftRotate(ArticleNode **root, ArticleNode *node)
{
	if (node == NULL || node -> right == NULL){
        return ;
	}

    ArticleNode *y = node -> right;

    node -> right = y -> left;

    if (node -> right != NULL){
        node -> right -> parent = node;
    }

    y -> parent = node -> parent;

    if(node -> parent == NULL){
		(*root) = y;
	}
	else if(node == node -> parent -> left){
		node -> parent -> left = y;
	}
    else{
    	node -> parent -> right = y;
    }

    y -> left = node;
    node -> parent = y;
}

void rightRotate(ArticleNode **root, ArticleNode *node)
{
	if (node == NULL || node -> left == NULL){
        return ;
	}

    ArticleNode *y = node -> left;

    node -> left = y -> right;

    if (y -> right != NULL){
        y -> right -> parent = node;
    }

    y -> parent =node -> parent;

	if(y -> parent == NULL){
		(*root) = y;
	}
	else if(node = node -> parent -> left){
		node -> parent -> left = y;
	}
	else{
		node -> parent -> left = y;
	}

    y -> right = node;
    node -> parent = y;
}

void insertFixUp(ArticleNode **root, ArticleNode *node)
{
	//puts("Outside");
    while (node != (*root) && node != (*root) -> left && node != (*root) -> right && node -> parent -> color == RED)
    {
		ArticleNode *y;

	   	if (node -> parent && node -> parent -> parent && node -> parent == node -> parent -> parent -> left)
		{
			y = node -> parent -> parent -> right;
		}
		else
		{
			y = node -> parent -> parent -> left;
		}
		if (y == NULL)
		{
			node = node -> parent -> parent;
		}
		else if (y -> color == RED)
		{
			y -> color = BLACK;
			node -> parent -> color = BLACK;
			node -> parent -> parent -> color = RED;
			node = node -> parent -> parent;
		}

		else
		{
			if (node -> parent == node -> parent -> parent -> left && node == node -> parent -> left)
			{
				Color c = node -> parent -> color ;
				node -> parent -> color = node -> parent -> parent -> color;
				node -> parent -> parent -> color = c;
				rightRotate(root, node -> parent -> parent);
			}
			if (node -> parent && node -> parent -> parent && node -> parent == node -> parent -> parent -> left && node == node -> parent -> right)
			{
				Color c = node -> color ;
				node -> color = node -> parent -> parent -> color;
				node -> parent -> parent -> color = c;
				leftRotate(root, node -> parent);
				rightRotate(root, node -> parent -> parent);
			}
			if (node -> parent && node -> parent -> parent && node -> parent == node -> parent -> parent -> right && node == node -> parent -> right)
			{
				Color c = node -> parent -> color ;
				node -> parent -> color = node -> parent -> parent -> color;
				node -> parent -> parent -> color = c;
				leftRotate(root, node -> parent -> parent);
			}
			if (node -> parent && node -> parent -> parent && node -> parent == node -> parent -> parent -> right && node == node -> parent -> left)
			{
				Color c = node -> color ;
				node -> color = node -> parent -> parent -> color;
				node -> parent -> parent -> color = c;
				rightRotate(root, node -> parent);
				leftRotate(root, node -> parent -> parent);
			}
		}
	}
    (*root) -> color = BLACK; //keep root always black
}


void insert(ArticleNode **root, Article* data)
{
	// Initialize newNode and it's members
	ArticleNode* newNode;
	newNode = (ArticleNode*) malloc(sizeof(ArticleNode));
	newNode -> article = (Article*) malloc(sizeof(data));
	// printf("%d - %s\n", sizeof(data -> title), data -> title);
	newNode -> article -> ID = (char*) malloc(data -> IDSize * sizeof(char));
	newNode -> article -> title = (char*) malloc(data -> titleSize * sizeof(char));
	newNode -> article -> authors = (char*) malloc(data -> authorSize * sizeof(char));

	strcpy(newNode -> article -> ID, data -> ID);
	strcpy(newNode -> article -> title, data -> title);
	printf("%d =? %d\n",data -> authorSize, sizeof(newNode -> article -> authors));
	strcpy(newNode -> article -> authors, data -> authors);
	puts("HEre");

	newNode -> color = RED;
	newNode -> left = NULL;
	newNode -> right = NULL;
	newNode -> parent = NULL;

    // If the tree is empty
	if (*root == NULL)
    {
		//puts("Inseting, but root is NULL.");
        newNode -> color = BLACK;
        (*root) = newNode;
		//printf("Insert root = %s \n", root -> article -> ID);
    }
    else
    {
		//puts("Inseting, but root is not NULL.");
        ArticleNode *pointer = NULL;
        ArticleNode *cursor = (*root);

        while (cursor != NULL)
        {
            pointer = cursor;
            if (strcmp(newNode -> article -> ID, cursor -> article -> ID) < 0){
				//printf("%s < %s\n", newNode -> article -> ID, cursor -> article -> ID);
                cursor = cursor -> left;
            } else{
				//printf("%s > %s\n", newNode -> article -> ID, cursor -> article -> ID);
                cursor = cursor -> right;
            }
        }
        newNode -> parent = pointer;
        // if(pointer == NULL){
        //     (*root) = newNode;
        // }
        if (strcmp(newNode -> article -> ID, pointer -> article -> ID) < 0){
            pointer -> left = newNode;
        }
        else if (strcmp(newNode -> article -> ID, pointer -> article -> ID) > 0){
            pointer -> right = newNode;
        }
        newNode -> color = RED;
        newNode -> left = NULL;
        newNode -> right = NULL;
		insertFixUp(root, newNode);
        //free(cursor);
        //free(pointer);
        //free(newNode);
    }
}

void inorder(ArticleNode *root)
{
    if (root != NULL){
    	inorder(root -> left);
    	printf("%s ", root -> article -> ID);
    	inorder(root -> right);
	}

}
