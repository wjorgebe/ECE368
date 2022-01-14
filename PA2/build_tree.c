#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "hbt.h"
#include "build_tree.h"

Tnode * createNode(int key);
Tnode * findMinNode(Tnode * node);
static int getBalance(Tnode * node);
static int max(int a, int b);
int findHeight(Tnode * node);

Tnode * buildTree(char * filename, int * validity, int * size) {

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

  Tnode * fullTree;
  fullTree = Build_Tree_From_Inst(key_array, char_array, file_count);

  fclose(fptr);
  free(tempKey);
  free(tempChar);
  free(key_array);
  free(char_array);
  validity[0] = 1;
  size[0] = file_count;
  return fullTree;
}

Tnode * Build_Tree_From_Inst(int * keys, char * instructions, int size) {

    Tnode * root = NULL;
    int i;

    for (i = 1; i <= size; i++) {
      if (instructions[i - 1] == 'i') {
        //Calling insertion Function
        root = insertNode(root, keys[i - 1]);
      }
      else if (instructions[i - 1] == 'd') {
        //Calling deletion function
        root = deleteNode(root, keys[i - 1]);
      }
    }

    return root;
}

Tnode * insertNode(Tnode * node, int key) {

  if (node == NULL)
    return createNode(key);

  if (key < node->key)
    node->left = insertNode(node->left, key);
  else if (key > node->key)
    node->right = insertNode(node->right, key);
  else
    return node;

  if (node->left != NULL && node->right != NULL)
    node->height = 1 + max(node->left->height, node->right->height);

  if (node->left != NULL && node->right == NULL)
    node->height = 1 + max(node->left->height, 0);

  if (node->left == NULL && node->right != NULL)
    node->height = 1 + max(0, node->right->height);

  if (node->left == NULL && node->right == NULL)
    node->height = 1;

  int balance = getBalance(node);

  //4 cases
  if (balance > 1 && key < node->left->key)
    return rightRotate(node);

  if (balance < -1 && key > node->right->key)
    return leftRotate(node);

  if (balance > 1 && key > node->left->key)
  {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && key < node->right->key)
  {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;

}

Tnode * deleteNode(Tnode * node, int key) {
  if (node == NULL)
    return node;

  if (key < node->key)
    node->left = deleteNode(node->left, key);
  else if (key > node->key)
    node->right = deleteNode(node->right, key);
  else
  {
    if ((node->left == NULL) || (node->right == NULL))
    {
      Tnode * temp = node->left ? node->left : node->right;

      if (temp == NULL)
      {
        temp = node;
        node = NULL;
      }
      else
        node = temp;

      free(node);
    }
    else
    {
      Tnode * temp = findMinNode(node->right);
      node->key = temp->key;
      node->right = deleteNode(node->right, temp->key);
    }
  }

  if (node == NULL)
    return node;

  if (node->left != NULL && node->right != NULL)
    node->height = 1 + max(node->left->height, node->right->height);

  if (node->left != NULL && node->right == NULL)
    node->height = 1 + max(node->left->height, 0);

  if (node->left == NULL && node->right != NULL)
    node->height = 1 + max(0, node->right->height);

  if (node->left == NULL && node->right == NULL)
    node->height = 1;

  int balance = getBalance(node);

  if (balance > 1 && getBalance(node->left) >= 0)
    return rightRotate(node);

  if (balance > 1 && getBalance(node->left) < 0)
  {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && getBalance(node->right) <= 0)
    return leftRotate(node);

  if (balance < -1 && getBalance(node->right) > 0)
  {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;

}

Tnode * createNode(int key) {
  Tnode * node = malloc(sizeof(Tnode));

  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;

  return node;
}

static int getBalance(Tnode * node) {
  if (node == NULL)
    return 0;

  return findHeight(node->left) - findHeight(node->right);
}

int findHeight(Tnode * node)
{
  if (node == NULL)
    return 0;

  return node->height;
}

static int max(int a, int b) {

  return (a >= b) ? a : b;
}

Tnode * leftRotate(Tnode * node) {
  Tnode * temp1 = node->right;
  Tnode * temp2 = temp1->left;

  temp1->left = node;
  node->right = temp2;

  node->height = 1 + max(findHeight(node->left), findHeight(node->right));
  temp1->height = 1 + max(findHeight(temp1->left), findHeight(temp1->right));

  return temp1;
}

Tnode * rightRotate(Tnode * node) {
  Tnode * temp1 = node->left;
  Tnode * temp2 = temp1->right;

  temp1->right = node;
  node->left = temp2;

  node->height = 1 + max(findHeight(node->left), findHeight(node->right));
  temp1->height = 1 + max(findHeight(temp1->left), findHeight(temp1->right));

  return temp1;
}

Tnode * findMinNode(Tnode * node) {
  Tnode * temp;

  temp = node;

  while (temp->left != NULL)
    temp = temp->left;

  return temp;
}

void Write_Tree_to_File(char * filename, Tnode * tree, int size)
{
  FILE * fptr;

  // Create filen, no matter empty array
  fptr = fopen(filename, "wb");

  int * key_array = malloc(sizeof(int) * size);
  char * char_array = malloc(sizeof(char) * size);
  int i = 0;

  Fill_Arrays(tree, key_array, char_array, &i, size);

  int j;
  int written = 0;
  int tempKey;
  char tempChar;

  for (j = 1; j <= size; j++)
  {
    tempKey = key_array[j - 1];
    tempChar = char_array[j - 1];
    //printf("%d ", tempKey);
    //printf("%c\n", tempChar);
    written += fwrite(&tempKey, sizeof(int), 1, fptr);
    //written += fwrite(" ", sizeof(char), 1, fptr);
    written += fwrite(&tempChar, sizeof(char), 1, fptr);
    //written += fwrite("\n", sizeof(char), 1, fptr);
  }
  //fprintf(stdout, "%d\n", written);
  free(key_array);
  free(char_array);
  //free(tempKey);
  //free(tempChar);
  fclose(fptr);
}

void Fill_Arrays(Tnode * tree, int * keys, char * children, int * index, int size)
{
  if (tree == NULL)
    return;

  if (index[0] < size) {
    keys[index[0]] = tree->key;
    if (tree->left != NULL && tree->right != NULL)
      children[index[0]] = '3';
    else if (tree->left != NULL && tree->right == NULL)
      children[index[0]] = '2';
    else if (tree->left == NULL && tree->right != NULL)
      children[index[0]] = '1';
    else
      children[index[0]] = '0';
  }

  index[0]++;
  Fill_Arrays(tree->left, keys, children, index, size);
  Fill_Arrays(tree->right, keys, children, index, size);
}
