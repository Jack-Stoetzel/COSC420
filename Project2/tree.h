// Tree One: Tree of IDs
// Nodes include Articles and index by thier ID

typedef struct ARTICLES{
    char ID [32];
    char title[256];
    char authors[256];
    char abstract[4096];

}Articles;

typedef struct NODE
{
    Articles data; //node will store an integer
    Node* right; // right child
    Node* left; // left child
} Node;

//function to create a node
Node* newNode(Article x)
{
    Node *p;
    p = malloc(sizeof(Node));
    p -> data = x;
    p -> left = NULL;
    p -> right = NULL;

    return p;
}

Node* search(Node *root, Articles x)
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

Node* insert(Node *root, int x)
{
    //searching for the place to insert
    if(root == NULL)
        return newNode(x);
    else if(x > root -> data) // x is greater. Should be inserted to right
        root -> right = insert(root -> right, x);
    else // x is smaller should be inserted to left
        root -> left = insert(root -> left, x);
    return root;
}
