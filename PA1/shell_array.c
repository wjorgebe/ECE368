#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "shell_array.h"

static int find_maxK(int size);

long *Array_Load_From_File(char *filename, int *size)
{
  FILE * fptr;
  int file_count = 0;
  long * temp;
  int num_read;

  temp = malloc(sizeof(long));
  fptr = fopen(filename, "rb");

  // Failure to open file or allocate memory
  if(fptr == NULL || temp == NULL) {
    size[0] = 0;
    return NULL;
  }

  //  Reading longs from binary input file
  while(fread(temp, sizeof(long), 1, fptr)) {
    file_count += 1;
  }

  // If file is empty
  if(file_count == 0) {
    long * empty_array = 0;
    size[0] = 0;
    return empty_array;
  }

  // Returning to beginning of file
  fseek(fptr, 0, SEEK_SET);

  // Creating array
  long * loaded_array = malloc(sizeof(long) * file_count);
  num_read = fread(loaded_array, sizeof(long), file_count, fptr);

  if(num_read != file_count) {
    size[0] = 0;
    return NULL;
  }

  free(temp);
  fclose(fptr);
  size[0] = num_read;
  return loaded_array;
}

int Array_Save_To_File(char *filename, long *array, int size)
{
  FILE * fptr;
  int num_written = 0; // Value being returned

  // Create filen, no matter empty array
  fptr = fopen(filename, "wb");

  // Writing array to file
  if(size == 0 || array == NULL) {
    num_written = 0;
  }
  else {
    num_written = fwrite(array, sizeof(long), size, fptr);
    if(num_written != size) {
      num_written = 0;
    }
  }

  fclose(fptr);
  return num_written;
}

void Array_Shellsort(long *array, int size, long *n_comp) {
  int inner;
  int outer;
  long temp;

  // Determining max k
  int maxK;
  maxK = find_maxK(size);

  while (maxK > 0) {
    for (outer = maxK; outer < size; outer++) {
      temp = array[outer];
      inner = outer;

      while (inner > (maxK - 1) && array[inner - maxK] >= temp) {
        array[inner] = array[inner - maxK];
        inner -= maxK;
        n_comp[0]++;
      }

      array[inner] = temp;

    }

    if (maxK != 1) {
      maxK = (maxK - 1) / 3;
    }
    else {
      maxK = 0;
    }
  }
}

static int find_maxK(int size) {
  int maxK = 1;

  while (maxK <= size/3) {
    maxK = maxK * 3 + 1;
  }

  return maxK;
}
