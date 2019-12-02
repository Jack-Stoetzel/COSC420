// Tree One: Tree of IDs
// Nodes include Articles and index by thier ID

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum COLOR{
	RED, BLACK
}Color;

typedef struct ARTICLES{
    char ID [32];
    char title[256];
    char authors[256];
    char abstract[4096];

}Article;

typedef struct ANODE
{
    Article* article;
    Color color;
    struct ANODE *left, *right, *parent;
}ArticleNode;

void leftRotate(ArticleNode *root, ArticleNode *node)
{
    ArticleNode *y = node -> right;

    node -> right = y -> left;

    if (node -> left != NULL){
        node -> left -> parent = node;
    }

    y -> parent = node -> parent;

    if(node -> parent != NULL){
    	if(node == node -> parent -> left){
    		node -> parent -> left = y;
    	}
    	else{
    		node -> parent -> right = y;
    	}
    }
    else{
    	root = y;
    }

    y -> left = node;
    node -> parent = y;
}

void rightRotate(ArticleNode *root,ArticleNode *node)
{
    ArticleNode *y = node -> left;

    node -> left = y -> right;

    if (y -> right != NULL){
        y -> right -> parent = node;
    }

    y -> parent =node -> parent;

    if(node -> parent != NULL){
		if(node == node -> parent -> left){
			node -> parent -> left = y;
		}
		else{
			node -> parent -> right = y;
		}
	}
	else{
		root = y;
	}
    y -> right = node;
    node -> parent = y;
}

void insertFixUp(ArticleNode *root, ArticleNode *node)
{
    while(node -> parent -> color == RED)
    {
        ArticleNode *y;
        if (node -> parent == node -> parent -> parent -> left){
            y = node -> parent -> parent -> right;
                if (y -> color == RED){
                    y -> color = BLACK;
                    node -> parent -> color = BLACK;
                    node -> parent -> parent -> color = RED;
                    node = node -> parent -> parent;
                }
                else{
                    if(node == node -> parent -> right){
                        node = node -> parent;
                        leftRotate(root, node);
                    }
                    node -> parent -> color = BLACK;
                    node -> parent -> parent -> color = RED;
                    rightRotate(root, node -> parent -> parent);
                }
            }
        else
        {
            y = node -> parent -> parent -> left;
            if( y -> color == RED){
                node -> parent -> color = BLACK;
				y -> color = BLACK;
				node -> parent -> parent -> color = RED;
				node = node -> parent -> parent;
			}
			else{
				if(node == node -> parent -> left){
					node = node -> parent;
					rightRotate(root, node);
				}
				node -> parent -> color = BLACK;
				node -> parent -> parent -> color = RED;
				leftRotate(root, node -> parent -> parent);
			}
        }
    }
    root -> color = BLACK;
}


void insert(ArticleNode *root, Article* article)
{
	// Initialize newNode and it's members
    ArticleNode *newNode = (ArticleNode*) malloc(sizeof(ArticleNode));
	newNode -> article = (Article*) malloc(sizeof(article));
    newNode -> left = (ArticleNode*) malloc(sizeof(ArticleNode));
	newNode -> right = (ArticleNode*) malloc(sizeof(ArticleNode));
	newNode -> parent = (ArticleNode*) malloc(sizeof(ArticleNode));

	// Copy info from Article objetc to new nodde
    strcpy(newNode -> article -> ID, article -> ID);
    strcpy(newNode -> article -> title, article -> title);
    strcpy(newNode -> article -> authors, article -> authors);
    strcpy(newNode -> article -> abstract, article -> abstract);
    newNode -> left = NULL;
    newNode -> right = NULL;
    newNode -> parent = NULL;

    // If the tree is empty
    if (root == NULL)
    {
        newNode -> color = BLACK;
        root = newNode;
    }
    else
    {
        ArticleNode *pointer = NULL;
        ArticleNode *cursor = root;

        while (cursor != NULL)
        {
            pointer = cursor;
            if (strcmp(newNode -> article -> ID, cursor -> article -> ID) == -1){
                cursor = cursor -> left;
            } else if (strcmp(newNode -> article -> ID, cursor -> article -> ID) == 1) {
                cursor = cursor -> right;
            }
        }
        newNode -> parent = pointer;
        if(pointer == NULL){
            root = newNode;
        }
        else if (strcmp(newNode -> article -> ID, pointer -> article -> ID) == -1){
            pointer -> left = newNode;
        }
        else if (strcmp(newNode -> article -> ID, pointer -> article -> ID) == 1){
            pointer -> right = newNode;
        }
        newNode -> color = RED;
        newNode -> left = NULL;
        newNode -> right = NULL;
        insertFixUp(root, newNode);
        free(cursor);
        free(pointer);
        free(newNode);
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
