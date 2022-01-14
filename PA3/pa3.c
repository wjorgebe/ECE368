#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pa3.h"

void countPath(short int * parent, int j, int * path);
void findPath(short int * parent, int j, short int * path, int * counter);

int main(int argc, char * argv[]) {

  if (argc != 5)
    return EXIT_FAILURE;

  struct Graph * graph;
  short int rows, columns;
  graph = Read_From_File(argv[1], argv[2], &rows, &columns);

  short int ** distances = malloc(sizeof(short int *) * columns);
  int * minTimes = malloc(sizeof(int) * columns);
  short int ** paths = malloc(sizeof(short int *) * columns);
  int minTime;

  for (int i = 0; i < columns; i++) {
    paths[i] = malloc((rows+1) * columns * sizeof(short));
  }

  //Finding minimum time required out of all entry points
  for (int i = 0; i < columns; i++) {
    distances[i] = dijkstra(graph, i, paths[i]);
    for (int j = 0; j < columns; j++) {
      short int element = (rows * columns) - (columns - j);
      if (j == 0) {
        minTime = graph->array[element].head->weight + distances[i][element];
      }
      else {
        if (graph->array[element].head->weight + distances[i][element] < minTime) {
          minTime = graph->array[element].head->weight + distances[i][element];
        }
      }
    minTimes[i] = minTime;
    }
  }

  //Finding exit spot from minimum time entry
  short int min_index_entry = 0;
  short int min_index_time = minTimes[0];

  for (int i = 1; i < columns; i++) {
    if (minTimes[i] < min_index_time) {
      min_index_time = minTimes[i];
      min_index_entry = i;
    }
  }

  short int min_index_exit = 0;
  short int min_index_time2 = distances[min_index_entry][rows*columns];

  for (int i = 1; i < columns; i++) {
    if (distances[min_index_entry][rows*columns+i] < min_index_time2) {
      min_index_time2 = distances[min_index_entry][rows*columns+i];
      min_index_exit = i;
    }
  }

  //Populating second file
  Print_Fastest_Times(argv[3], minTimes, columns);

  //Parameters required to print path
  int path_count = 0;
  int counter = 0;
  countPath(paths[min_index_entry], rows*columns+min_index_exit, &path_count);

  //Populate path file from extracted data
  FILE * fptr = fopen(argv[4], "wb");
  int shortest_time = distances[min_index_entry][rows*columns+min_index_exit];
  fwrite(&shortest_time, sizeof(int), 1, fptr);
  fwrite(&path_count, sizeof(int), 1, fptr);
  fwrite(&graph->array[min_index_entry].row, sizeof(short), 1, fptr);
  fwrite(&graph->array[min_index_entry].col, sizeof(short), 1, fptr);
  short int * path = malloc((rows * columns) * sizeof(short));
  findPath(paths[min_index_entry], rows*columns+min_index_exit, path, &counter);
  for (int i = 0; i < (path_count -1); i++) {
    fwrite(&graph->array[path[i]].row, sizeof(short), 1, fptr);
    fwrite(&graph->array[path[i]].col, sizeof(short), 1, fptr);
  }
  fclose(fptr);

  //Freeing distances array
  for (int i = 0; i < columns; i++) {
    free(distances[i]);
    free(paths[i]);
  }
  free(distances);
  free(paths);
  free(path);

  //Freeing graph
  struct AdjListNode * tempNode;
  for (int i = 0; i < ((rows+1) * columns); i++) {
    while(graph->array[i].head != NULL) {
      tempNode = graph->array[i].head;
      graph->array[i].head = graph->array[i].head->next;
      free(tempNode);
    }
  }

  //Freeing everything else
  free(minTimes);
  free(graph->array);
  free(graph);

  return EXIT_SUCCESS;
}


void countPath(short int * parent, int j, int * path)
{

    // Base Case : If j is source
    if (parent[j] < 0)
        return;

    countPath(parent, parent[j], path);

    path[0]++;
}

void findPath(short int * parent, int j, short int * path, int * counter) {
  if (parent[j] < 0)
    return;

    findPath(parent, parent[j], path, counter);

    path[counter[0]] = j;
    counter[0]++;
}
