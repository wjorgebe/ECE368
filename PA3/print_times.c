#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "pa3.h"

void Print_Fastest_Times(char * filename, int * minTimes, short int columns) {

  FILE * fptr = fopen(filename, "wb");

  fwrite(&columns, sizeof(short), 1, fptr);
  fwrite(minTimes, sizeof(int), columns, fptr);

  fclose(fptr);
}
