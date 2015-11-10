/**
 John McNelly
 Mr. Thibodeaux
 ATCS
 3/11/14
 This AVLTree program (aka. "Herb") allows users to create an AVLTree by inputting data points (words) manually
 or through a text file.  As more nodes are added to the tree, the tree is rebalanced so that it only takes log(N)
 steps to reach any particular node.  The tree is automatically balanced, but all functions can be manually done as
 well.
 **/

#include "herb.h"

int main(int argc, const char * argv[])
{
    char data[80];

    printf("Hello. I'm Herb.\n");
    menu();
    return 0;
}

//menu-driven interface that allows user to utilize manual program funcitons
int menu()
{
    struct AVLTree *tree = (struct AVLTree *)malloc(sizeof(struct AVLTree));
    tree->root = NULL;
    
    printf("Menu Initialized \n");
    int input = 1;
          
    while(input != QUIT)
    {
        printf("-------------------------------------------------------------------------\n");
        printf("Options: 1 = insert word; 2 = print list; 3 = erase list; 4 = delete node; 5 = read text file; 6 = balance; 0 = QUIT\n");
        printf("Enter choice: ");
        scanf("%d", &input);

        //insert a node
        if(input == INSERT)
        {
            printf("INSERT...\n");
            char word[80];
            printf("Enter word: ");
            scanf("%s", word);
            printf("you entered \"%s\"\n", word);
            insert(tree, word);
        }
        //print the tree
        else if(input == PRINT)
        {
            printf("PRINT...\n");
            printTree(tree);
        }
        //erase the tree
        else if(input == ERASE)
        {
            printf("ERASE...\n");
            clearTree(tree);
        }
        //delete a node -- not implemented
        else if(input == DELETE)
        {
            printf("DELETE...\n");
        }
        //read in input from the text file
        else if(input == READ)
        {
            printf("READ...\n");
            int returnCode = readFile(tree);
            if(returnCode == 1)
                printf("File not found.  Bummer :/");
        }
        //manually balance the tree
        else if(input == BALANCE)
        {
            printf("BALANCE...\n");
            AVLBalance(tree);
        }
        else
        {
            printf("INVALID INPUT!\n");
        }
    }
    return 0;
}

//reads in words from a file
int readFile(struct AVLTree *tree)
{
    FILE *fptr;
    char word[80];
    fptr = fopen("/Users/jkailimcnelly/Dropbox/Computer Science/C Programs/Herb/Herb/words.txt", "r");
    
    if(fptr == NULL)
    {
        return 1;
    }
    
    while(fgets(word, 80, fptr) != NULL)
    {
        insert(tree, word);
        printf("%s\n", word);
        AVLBalance(tree);
    }
    
    fclose(fptr);
    return 0;
}

//creates a new AVLNode
struct AVLNode *newNode(char word[80])
{
    struct AVLNode *temp = (struct AVLNode *)malloc(sizeof(struct AVLNode));
    strcpy(temp->word, word);
    strcpy(temp->balanceFactor, "E");
    temp->count = 1;
    temp->left = NULL;
    temp->right = NULL;
    
    printf("node created with word: %s\n", temp->word);
    return temp;
}

//inserts node into tree
int insert(struct AVLTree *tree, char word[80])
{
    //tree is empty
    if(tree->root == NULL)
    {
        struct AVLNode *temp = newNode(word);
        tree->root = temp;
    }
    //tree has at least 1 node
    else
    {
        auxInsert(tree->root,word);
    }
    
    
    return 0;
}

//recursive insert helper method, inserts 1 if it increments (node already exists), 0 otherwise
int auxInsert(struct AVLNode *root, char word[80])
{
    int inserted = 0;
    
    //word is before root->word
    if(strcmp(word, root->word) < 0)
    {
        if(root->left == NULL)
        {
            struct AVLNode *temp = newNode(word);
            root->left = temp;
            adjustBalanceFactor(temp);
            inserted = 1;
        }
        else
        {
            inserted = auxInsert(root->left, word);
        }
        
        //increase left bf if insert occurs (new node created)
        if(inserted == 1)
        {
            increaseLeftBalanceFactor(root);
        }
    }
    //word is after root->word
    else if(strcmp(word, root->word) > 0)
    {
        if(root->right == NULL)
        {
            struct AVLNode *temp = newNode(word);
            root->right = temp;
            adjustBalanceFactor(temp);
            inserted = 1;
            
        }
        else
        {
            inserted = auxInsert(root->right, word);
        }
        
        //increase right bf if insert occurs (new node created)
        if(inserted == 1)
        {
            increaseRightBalanceFactor(root);
        }
        
    }
    //word is equal to root-> word
    else
    {
        root->count++;
        inserted = 0;
    }
    
    return inserted;
}

//prints words in tree
int printTree(struct AVLTree *tree)
{
//    printf("max depth is: %d\n", maxTreeDepth(tree));
    if(tree == NULL)
    {
        printf("tree does not exist\n");
    }
    else if(tree->root == NULL)
    {
        printf("tree is empty\n");
    }
    else
    {
        auxPrintTree(tree->root);
        printf("Print Completed\n");
    }
    return 0;
}

//recursive print helper method
int auxPrintTree(struct AVLNode *root)
{
    //base case
    if(root == NULL)
    {
        return 0;
    }
    //recursive calls to left and right
    else
    {
        auxPrintTree(root->left);
        printf("%d x %s..............bf:%s\n", root->count, root->word, root->balanceFactor);
        auxPrintTree(root->right);
    }
    return 0;
}

//adjusts balance factor of an AVLNode
//NOTE: needs to be called from bottom-up (bases bf on nodes below)
int adjustBalanceFactor(struct AVLNode *root)
{
    if(root == NULL)
    {
        printf("ERROR IN adjustBalanceFactor!");
        return 1;
    }
    
    //printf("in adjustBalanceFactor for node with data %s\n", root->word);
    
    //no nodes on left
    if(root->left == NULL)
    {
        //no nodes below
        if(root->right == NULL)
        {
            strcpy(root->balanceFactor, "E");
        }
        //nodes on right but not left
        else
        {
            strcpy(root->balanceFactor, root->right->balanceFactor);
            increaseRightBalanceFactor(root);
        }
        
    }
    //nodes on left
    else
    {
        //nodes on left but not right
        if(root->right == NULL)
        {
            strcpy(root->balanceFactor, root->left->balanceFactor);
            increaseLeftBalanceFactor(root);
        }
        //nodes on right and left
        else
        {
            int leftBalanceFactor = convertBalanceFactor(root->right->balanceFactor);
            int rightBalanceFactor = convertBalanceFactor(root->right->balanceFactor);
            
            int balanceFactor = rightBalanceFactor-leftBalanceFactor;
            //balanceFactor is right-heavy
            if(balanceFactor > 0)
            {
                for(int i = 0; i < balanceFactor; i ++)
                {
                    increaseRightBalanceFactor(root);
                }
            }
            //balanceFactor is left-heavy
            else if(balanceFactor < 0)
            {
                for(int i = 0; i < -balanceFactor; i ++)
                {
                    increaseLeftBalanceFactor(root);
                }
            }
            else
            {
                strcpy(root->balanceFactor, "E");
            }
        }
    }
    
    return 0;
}

//converts balance factor into number -- Rn returns n, Ln returns -n, E returns 0
int convertBalanceFactor(char balanceFactor[3])
{
    if(balanceFactor == NULL)
    {
        printf("Crap went down in convertBalanceFactor");
        return 1;
    }
    
    int number;
    
    //THROWING EXC_BAD_ACCESS here for balanceFactor
    if(balanceFactor[0] == 'R')
    {
        number = balanceFactor[1] - '0';
    }
    else if(balanceFactor[0] == 'L')
    {
        number = -(balanceFactor[1] - '0');
    }
    else
    {
        number = 0;
    }
    
    return number;
}

//increases left balance factor of root
int increaseLeftBalanceFactor(struct AVLNode *root)
{
    //E becomes L1
    if(root->balanceFactor[0] == 'E')
    {
        strcpy(root->balanceFactor, "L1");
    }
    //L(n) becomes L(n+1)
    else if(root->balanceFactor[0] == 'L')
    {
        root->balanceFactor[1] = ++(root->balanceFactor[1]);
    }
    //R(n) becomes R(n-1)
    else if(root->balanceFactor[0] == 'R')
    {
        root->balanceFactor[1] = --(root->balanceFactor[1]);
        if(root->balanceFactor[1] == '0')
        {
            strcpy(root->balanceFactor, "E");
        }
    }
    
    return 0;
}

//increases right balance factor of root
int increaseRightBalanceFactor(struct AVLNode *root)
{
    //E becomes R1
    if(root->balanceFactor[0] == 'E')
    {
        strcpy(root->balanceFactor, "R1");
    }
    //R(n) becomes R(n+1)
    else if(root->balanceFactor[0] == 'R')
    {
        root->balanceFactor[1] = ++(root->balanceFactor[1]);
    }
    //L(n) becomes L(n-1)
    else if(root->balanceFactor[0] == 'L')
    {
        root->balanceFactor[1] = --(root->balanceFactor[1]);
        if(root->balanceFactor[1] == '0')
        {
            strcpy(root->balanceFactor, "E");
        }
    }
    
    return 0;
}

//clears the tree, then sets root to null
int clearTree(struct AVLTree *tree)
{
    auxClearTree(tree->root);
    tree->root = NULL;
    return 0;
}

//recursive helper function for clearTree
int auxClearTree(struct AVLNode *root)
{
    if(root != NULL)
    {
        //recursive calls to left and right, then kills itself
        auxClearTree(root->left);
        auxClearTree(root->right);
        free(root);
    }
    return 0;
}

//wrapper function for AVL balancing
int AVLBalance (struct AVLTree *tree)
{
    if(tree->root == NULL)
    {
        printf("Tried to balance an empty tree");
        return 1;
    }
    
    struct AVLNode *root = tree->root;
    auxAVLBalance(&root);
    
    //implement changes
    tree->root = root;
    return 0;
}

//recursive AVL balance function
int auxAVLBalance (struct AVLNode **rootPtr)
{
    struct AVLNode *root = *rootPtr;
    
    //recursive calls
    //base case
    if(root == NULL)
    {
        //printf("hit a null root in auxAVLBalance\n");
        return 0;
    }
    else
    {
        //printf("in auxAVLBalance for node with data %s\n", (*rootPtr)->word);
        struct AVLNode *lnode = root->left;
        struct AVLNode *rnode = root->right;
        auxAVLBalance(&lnode);
        auxAVLBalance(&rnode);
        
        //implement changes
        root->left = lnode;
        root->right = rnode;
    }

    int balanceFactor = convertBalanceFactor(root->balanceFactor);
    //root balanced
    if(balanceFactor >=-1 && balanceFactor <=1)
    {
        return 0;
    }
    //root is left-heavy
    if(balanceFactor < -1)
    {
        int leftBalanceFactor = convertBalanceFactor(root->left->balanceFactor);
        //left-left case
        if(leftBalanceFactor < 0)
        {
            //printf("left-left case\n");
            //rotate nodes right
            struct AVLNode *left = root->left;
            struct AVLNode *leftLeft = root->left->left;
            struct AVLNode *leftRightChildren = left->right;
            
            //left gets root as right child
            left->right = root;
            //root gets left's right children as left children (had none before)
            root->left = leftRightChildren;
            
            //root now points to left
            root = left;
            
            //refresh balance factor from BOTTOM-UP
            //refresh balance factors of right child of root
            adjustBalanceFactor(root->right);
            //refresh balance factor of root
            adjustBalanceFactor(root);
        }
        //left-right case
        else
        {
            //printf("left-right case\n");
            //want to turn into left-left case
            struct AVLNode *left = root->left;
            struct AVLNode *leftRight = root->left->right;
            
            //assign left children from leftRight to become right children of left
            struct AVLNode *leftRightLeftChildren = leftRight->left;
            left->right = leftRightLeftChildren;
            
            //make leftRight into root's left child
            root->left = leftRight;
            
            //make left into leftRight's left child
            leftRight->left = left;
            
            //refresh balance factors from BOTTOM-UP
            //adjust balance factors of first 2 left children of root
            adjustBalanceFactor(root->left->left);
            adjustBalanceFactor(root->left);
            
            //results in left-left case: change tree and reassign root
            //printf("left-left case\n");
            //rotate nodes right
            left = root->left;
            struct AVLNode *leftLeft = root->left->left;
            struct AVLNode *leftRightChildren = left->right;
            
            //left gets root as right child
            left->right = root;
            //root gets left's right children as left children (had none before)
            root->left = leftRightChildren;
            
            //root now points to left
            root = left;
            
            //refresh balance factor from BOTTOM-UP
            //refresh balance factors of right child of root
            adjustBalanceFactor(root->right);
            //refresh balance factor of root
            adjustBalanceFactor(root);
        }
        
    }
    //root is right-heavy
    else
    {
        int rightBalanceFactor = convertBalanceFactor(root->right->balanceFactor);
        //right-left case
        if(rightBalanceFactor < 0)
        {
            //printf("right-left case\n");
            //want to turn into right-right case
            struct AVLNode *right = root->right;
            struct AVLNode *rightLeft = root->right->left;
            
            //assign right children from rightLeft to become left children or right
            struct AVLNode *rightLeftRightChildren = rightLeft->right;
            right->left = rightLeftRightChildren;
            
            //make rightleft into root's right child
            root->right = rightLeft;
            
            //make right into rightLeft's right child
            rightLeft->right = right;
            
            //refresh balance factors from BOTTOM-UP
            //adjust balance factors of first 2 right children of root
            adjustBalanceFactor(root->right->right);
            adjustBalanceFactor(root->right);
            
            //results in right-right: change tree and reassign root
            //printf("right-right case\n");
            //rotate nodes left
            right = root->right;
            struct AVLNode *rightRight = root->right->right;
            struct AVLNode *rightLeftChildren = root->right->left;
            
            //make root left child of right
            right->left = root;
            //root gets left children of right as right child
            root->right = rightLeftChildren;
            
            //root now points to right
            root = right;
            
            //refresh balance factor from BOTTOM-UP
            //refresh balance factors of left child of root
            adjustBalanceFactor(root->left);
            //refresh balance factor of root
            adjustBalanceFactor(root);
        }
        //right-right case
        else
        {
            //printf("right-right case\n");
            //rotate nodes left
            struct AVLNode *right = root->right;
            struct AVLNode *rightRight = root->right->right;
            struct AVLNode *rightLeftChildren = root->right->left;
            
            //make root left child of right
            right->left = root;
            //root gets left children of right as right child
            root->right = rightLeftChildren;
            
            //root now points to right
            root = right;
            
            //refresh balance factor from BOTTOM-UP
            //refresh balance factors of left child of root
            adjustBalanceFactor(root->left);
            //refresh balance factor of root
            adjustBalanceFactor(root);
        }
    }
    
    //reassign *rootPtr to root to implement changes
    *rootPtr = root;
    
    return 0;
}

