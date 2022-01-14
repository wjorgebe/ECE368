#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "shell_array.h"
#include "shell_list.h"

static void destroyList(Node *list);

int main(int argc, char *argv[]) {

  //Insufficient arguments
  if (argc != 4) {
    return EXIT_FAILURE;
  }

  // Strings for selecting operation
  const char *str1 = "-a";
  const char *str2 = "-l";

  if (strcmp(argv[1], str1) == 0) {
    long *array;
    int size = 0;
    long n_comp = 0; // Number of comparisons
    //int num_written;

    array = Array_Load_From_File(argv[2], &size);
    if (array == NULL) {
      return EXIT_FAILURE;
    }

    Array_Shellsort(array, size, &n_comp);
    Array_Save_To_File(argv[3], array, size);
    fprintf(stdout, "%ld\n", n_comp);

    // Freeing memory
    free(array);
  }

  if (strcmp(argv[1], str2) == 0) {
    Node * list;
    long n_comp;
    //int num_written;

    list = List_Load_From_File(argv[2]);
    List_Shellsort(list, &n_comp);
    List_Save_To_File(argv[3], list);
    fprintf(stdout, "%ld\n", n_comp);

    // Freeing memory
    destroyList(list);
  }

  return EXIT_SUCCESS;
}

static void destroyList(Node *list) {
  Node * temp_Node;

  temp_Node = list;

  while(list != NULL) {
    temp_Node = list->next;
    free(list);
    list = temp_Node;
  }
}
