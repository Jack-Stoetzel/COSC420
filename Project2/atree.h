// Tree One: Tree of IDs
// Nodes include Articles and index by thier ID
#ifndef ATREE_H
#define ATREE_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum COLOR
{
	RED, BLACK
}Color;

typedef struct ARTICLE
{
    char* ID;
	int IDSize;

    char* title;
	int titleSize;

    char* author;
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
	else if(node == node -> parent -> left){
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


void articleInsert(ArticleNode **root, Article* data)
{
	// Initialize newNode and it's members
	ArticleNode* newNode;
	newNode = (ArticleNode*) malloc(sizeof(ArticleNode));
	newNode -> article = (Article*) malloc(sizeof(data));

	newNode -> article -> ID = (char*) calloc(data -> IDSize, sizeof(char));
    //printf("%d =? %lu\n",data -> IDSize, strlen(newNode -> article -> ID));

	newNode -> article -> title = (char*) calloc(data -> titleSize, sizeof(char));
    //printf("%d =? %lu\n",data -> titleSize, strlen(newNode -> article -> title));

	newNode -> article -> author = (char*) calloc(data -> authorSize, sizeof(char));
	//printf("Does %lu == %lu ???\n", data -> authorSize, strlen(data -> author));
    //printf("%d =? %lu\n",data -> authorSize, strlen(newNode -> article -> author));

    strcpy(newNode -> article -> ID, data -> ID);
    //printf("%lu\t - \t %s", strlen(newNode -> article -> ID), newNode -> article -> ID);

    strcpy(newNode -> article -> title, data -> title);
	//printf("%lu\t - \t %s", strlen(newNode -> article -> title), newNode -> article -> title);

	//strcpy(newNode -> article -> author, data -> author);
	//printf("%lu\t - \t %s", strlen(newNode -> article -> author), newNode -> article -> author);


	newNode -> color = RED;
	newNode -> left = NULL;
	newNode -> right = NULL;
	newNode -> parent = NULL;

    // If the tree is empty
	if (*root == NULL)
    {
        newNode -> color = BLACK;
        (*root) = newNode;
    }
    else
    {
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
        free(cursor);
    }
}

void printInfoSearch(ArticleNode** root, char* ID)
{
	ArticleNode* curr = (*root);
	while(curr != NULL)
	{
		if(strcmp(ID, curr -> article -> ID) == 0)
		{
			printf("%s \n", curr -> article -> title);
			//printf("%s \n", curr -> article -> authors);
			return ;
		}
		else if(strcmp(ID, curr -> article -> ID) < 0)
		{
			curr = curr -> left;
		}
		else if(strcmp(ID, curr -> article -> ID) > 0)
		{
			curr = curr -> right;
		}
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

#endif
