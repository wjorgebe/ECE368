#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hbt.h"
#include "eval_tree.h"
#include "build_tree.h"

void freeTree(Tnode * tree);
//void printInorder(Tnode * node);

int main(int argc, char *argv[]) {

  // Strings for selecting operation
  const char *str1 = "-b";
  const char *str2 = "-e";

  //Insufficient arguments
  if (strcmp(argv[1], str1) == 0) {
    if (argc != 4) {
      return EXIT_FAILURE;
    }

    Tnode * fullTree;
    int validity;
    int size;

    fullTree = buildTree(argv[2], &validity, &size);

    //printInorder(fullTree);

    fprintf(stdout, "%d\n", validity);

    if (validity == -1 || validity == 0) {
      return EXIT_FAILURE;
    }

    Write_Tree_to_File(argv[3], fullTree, size);
    freeTree(fullTree);
    return EXIT_SUCCESS;
  }
  else if (strcmp(argv[1], str2) == 0) {
    if (argc != 3) {
      return EXIT_FAILURE;
    }

    //Code to evaluate tree
    Tnode * uneval_tree;
    int file_validity = 0;
    int isBST_flag = 0;
    int isBal_flag = 0;
    //int isBST = 0;

    uneval_tree = Build_Uneval_Tree(argv[2], &file_validity);

    if (uneval_tree != NULL) {
      file_validity = 1;
    }

    if (file_validity == -1 || file_validity == 0) {
      return EXIT_FAILURE;
    }

    isBST_flag = isBST(uneval_tree);
    isBal_flag = check_balance(uneval_tree);

    fprintf(stdout, "%d,%d,%d\n", file_validity, isBST_flag, isBal_flag);
    //printInorder(uneval_tree);
    freeTree(uneval_tree);
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}

/*
void printInorder(Tnode * node)
{
    if (node == NULL)
        return;
    printInorder(node->left);
    fprintf(stdout, "%d ", node->key);
    printInorder(node->right);
}
*/

void freeTree(Tnode * tree) {
  if (tree == NULL)
    return;

  freeTree(tree->left);
  freeTree(tree->right);

  free(tree);
}
