//
//  herb.h
//  Herb
//
//  Created by John McNelly on 2/27/14.
//  Copyright (c) 2014 John McNelly. All rights reserved.
//

#ifndef Herb_herb_h
#define Herb_herb_h
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define QUIT 0
#define INSERT 1
#define PRINT 2
#define ERASE 3
#define DELETE 4
#define READ 5
#define BALANCE 6

struct AVLTree
{
    struct AVLNode *root;
};

struct AVLNode
{
    char word[80];
    char balanceFactor[3];
    int count;
    struct AVLNode *left;
    struct AVLNode *right;
};

int menu();
int insert(struct AVLTree *tree, char word[80]);
struct AVLNode *newNode(char word[80]);
int auxInsert(struct AVLNode *root, char word[80]);
int readFile(struct AVLTree *tree);//, FILE *fptr);

int printTree(struct AVLTree *tree);
int auxPrintTree(struct AVLNode *root);

int clearTree(struct AVLTree *tree);
int auxClearTree(struct AVLNode *root);

int maxTreeDepth(struct AVLTree *tree);
int maxDepth(struct AVLNode *root);
int auxMaxDepth(struct AVLNode *root, int depth);

int increaseLeftBalanceFactor(struct AVLNode *root);
int increaseRightBalanceFactor(struct AVLNode *root);
int adjustBalanceFactor(struct AVLNode *root);
int convertBalanceFactor(char balanceFactor[3]);

int AVLBalance(struct AVLTree *tree);
int auxAVLBalance(struct AVLNode **rootPtr);
