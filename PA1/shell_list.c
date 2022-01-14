#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "shell_list.h"

static int find_maxK(int size);

Node *List_Load_From_File(char *filename) {
  FILE * fptr;
  int file_count = 0;
  long temp;
  Node * temp_Node = NULL;
  Node * fullList = NULL;

  // Allocating memory for temporary variables and full list
  fullList = malloc(sizeof(Node));
  temp_Node = malloc(sizeof(Node));

  fptr = fopen(filename, "rb");

  if (fread(&temp, sizeof(long), 1, fptr)) {
    fullList->value = temp;
    fullList->next = NULL;
  }

  temp_Node = fullList;

  while(fread(&temp, sizeof(long), 1, fptr)) { // Until end of file
    file_count += 1;
    Node * newNode = malloc(sizeof(Node));
    newNode->value = temp;
    temp_Node->next = newNode;
    temp_Node = newNode;
  }
   // Memory freeing
  fclose(fptr);
  free(temp_Node);

  return fullList;  // Full linked list
}

int List_Save_To_File(char *filename, Node *list) {
  FILE * fptr;
  int num_written = 0;
  long temp_val;
  Node * temp_Node = NULL;

  // Memory allocation of temporary variables
  temp_Node = malloc(sizeof(Node));
  temp_Node = list;

  fptr = fopen(filename, "wb");

  while (temp_Node != NULL) {
    temp_val = temp_Node->value;
    num_written += fwrite(&temp_val, sizeof(long), 1, fptr);
    temp_Node = temp_Node->next;
  }

  // Memory freeing of temporary variables
  fclose(fptr);
  free(temp_Node);

  return num_written; // Number of values written to file
}

Node *List_Shellsort(Node *list, long *n_comp) {
  int maxK;
  int size_of_list = 0;
  Node * inner = list;
  Node * outer = list;
  Node * pseudo = NULL;
  Node * tempNode = NULL;
  int i;

  tempNode = list;
  while (tempNode != NULL) {
    size_of_list++;
    tempNode = tempNode->next;
  }
  tempNode = NULL;

  maxK = find_maxK(size_of_list);

  // Empty list
  if (list == NULL) {
    return NULL;
  }

  // List has only one element
  if (list->next == NULL) {
    return list;
  }

  Node * last = outer;
  inner = list->next;
  outer = inner->next;

  while (maxK > 0) {
    inner = list->next;
    outer = inner->next;

    for (i = 0; i < maxK - 1; i++) {
      outer = outer->next;
    }

    tempNode = outer;
    pseudo = list;

    while (tempNode != last) {
      if (inner->value > tempNode->value) {
        outer->next = tempNode->next;
        tempNode->next = inner;
        pseudo->next = tempNode;
        tempNode = outer->next;
        n_comp[0]++;
      }
      else {
        tempNode = tempNode->next;
        outer = outer->next;
        inner = inner->next;
        pseudo = pseudo->next;
        n_comp[0]++;
      }
    }

    if (maxK != 1) {
      maxK = (maxK - 1) / 3;
    }
    else {
      maxK = 0;
    }
  }

  return list;
}

static int find_maxK(int size) {
  int maxK = 1;

  while (maxK <= size/3) {
    maxK = maxK * 3 + 1;
  }

  return maxK;
}
