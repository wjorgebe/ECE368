#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "hbt.h"
#include "eval_tree.h"

static int findHeight(Tnode * tree);
static int max(int a, int b);

Tnode * Build_Uneval_Tree(char * filename, int * validity) {

  FILE * fptr;
  int * tempKey;
  char * tempChar;
  int file_count = 0;

  fptr = fopen(filename, "rb");

  //If file cannot be opened
  if (fptr == NULL) {
    validity[0] = -1;
    return NULL;
  }

  tempKey = malloc(sizeof(int));
  tempChar = malloc(sizeof(char));

  //Assuring correct format, obtaining size
  fseek(fptr, 0, SEEK_END);

  file_count = ftell(fptr) / (sizeof(int) + sizeof(char))
;
  // Returning to beginning of file
  fseek(fptr, 0, SEEK_SET);

  int * key_array = malloc(sizeof(int) * file_count);
  char * char_array = malloc(sizeof(char) * file_count);
  int i = 0;

  //Building arrays with data
  while(fread(tempKey, sizeof(int), 1, fptr) && fread(tempChar, sizeof(char), 1, fptr)) {
      key_array[i] = tempKey[0];
      char_array[i] = tempChar[0];
      i++;
  }

  if (i != file_count) {
    validity[0] = 0;
    return NULL;
  }

  //Creating valid tree
  int preIndex = 0;
  Tnode * root = Build_From_Preorder(key_array, char_array, &preIndex, file_count);

  fclose(fptr);
  free(tempKey);
  free(tempChar);
  free(key_array);
  free(char_array);
  return root;
}

Tnode * Build_From_Preorder(int * pre, char * children, int * preIndex, int size) {

  if (preIndex[0] >= size) {
    return NULL;
  }

  char currentIdx = children[preIndex[0]];

  Tnode * root = addNode(pre[preIndex[0]]);

  if (currentIdx == 3 || currentIdx == 2) {
    preIndex[0]++;
    root->left = Build_From_Preorder(pre, children, preIndex, size);
  }

  if (currentIdx == 1 || currentIdx == 3) {
    preIndex[0]++;
    root->right = Build_From_Preorder(pre, children, preIndex, size);
  }

  return root;
}

Tnode * addNode(int keyVal) {
  Tnode * temp = malloc(sizeof(Tnode));

  temp->key = keyVal;
  temp->height = 0;
  temp->left = NULL;
  temp->right = NULL;

  return temp;
}

int isBST(Tnode * tree)
{
    static Tnode * prev = NULL;

    // traverse the tree in inorder fashion and keep track of prev node
    if (tree)
    {
        if (!isBST(tree->left))
          return 0;

        if (prev != NULL && tree->key <= prev->key)
          return 0;

        prev = tree;

        return isBST(tree->right);
    }

    return 1;
}

int check_balance(Tnode * tree) {
  int lh;
  int rh;

  if (tree == NULL) {
    return 1;
  }

  lh = findHeight(tree->left);
  rh = findHeight(tree->right);
  tree->height = lh - rh;

  if (abs(lh - rh) <= 1 && check_balance(tree->left) && check_balance(tree->right))
    return 1;

  return 0;
}

static int findHeight(Tnode * tree) {
  if (tree == NULL)
    return 0;

  return 1 + max(findHeight(tree->left), findHeight(tree->right));
}

static int max(int a, int b) {

  return (a >= b) ? a : b;
}
