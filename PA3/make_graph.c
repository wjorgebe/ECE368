#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "pa3.h"

//short int findMinDistance(short int * dist, bool * sptSet, short int dim);

struct Graph * Read_From_File(char * input, char * output, short int * r, short int * col)
{
  FILE * fptr1;
  FILE * fptr2;
  short int rows, columns;

  fptr1 = fopen(input, "rb");
  fptr2 = fopen(output, "w");

  if (fptr1 == NULL)
    return 0;

  if (fptr2 == NULL)
    return 0;

  fread(&rows, sizeof(short), 1, fptr1);
  fread(&columns, sizeof(short), 1, fptr1);
  fprintf(fptr2, "%hd %hd\n", rows, columns);

  short int ** tempGrid = malloc(sizeof(short int *) * (rows+1));

  short int i;
  short int j;

  for (i = 0; i < rows; i++) {
    tempGrid[i] = malloc(sizeof(short) * columns);
    fread(tempGrid[i], sizeof(short), columns, fptr1);
  }

  tempGrid[rows] = malloc(sizeof(short) * columns);
  for (i = 0; i < columns; i++) {
    tempGrid[rows][i] = 0;
  }

  struct Graph * graph = createGraph((rows+1) * columns);
  short int value, location, above, below;

  for (i = 0; i <= rows; i++) {
    for (j = 0; j < columns; j++) {
      value = tempGrid[i][j];
      location = i * columns + j;
      above = location - columns;
      below = location + columns;

      if (i != rows) {
        if (j == columns - 1) {
          fprintf(fptr2, "%hd\n", tempGrid[i][j]);
        }
        else {
          fprintf(fptr2, "%hd ", tempGrid[i][j]);
        }

        addEdge(graph, location, below, value, i, j);

        // if (below < (rows * columns) && i < rows - 1) {
        //
        // }

        if ((location + 1) < (rows * columns) && j < columns - 1) {
          addEdge(graph, location, location + 1, value, i, j);
        }

        if ((location - 1) >= 0 && j > 0) {
          addEdge(graph, location, location - 1, value, i, j);
        }

        if (above >= 0 && i > 0) {
          addEdge(graph, location, above, value, i, j);
        }
      }
      else {
        addEdge(graph, location, above, value, i, j);
      }

    }
  }

  for (i = 0; i < rows + 1; i++) {
    free(tempGrid[i]);
  }
  free(tempGrid);
  fclose(fptr1);
  fclose(fptr2);
  r[0] = rows;
  col[0] = columns;
  return graph;
}

struct Graph * createGraph(short int V) {
  struct Graph * graph = malloc(sizeof(struct Graph));
  graph->V = V;

  graph->array = malloc(V * sizeof(struct AdjList));

  int i;
  for (i = 0; i < V; i++) {
    graph->array[i].head = NULL;
  }

  return graph;
}

struct AdjListNode * newAdjListNode (short int dest, short int weight) {
  struct AdjListNode * newNode = malloc(sizeof(struct AdjListNode));

  newNode->dest = dest;
  newNode->weight = weight;
  newNode->next = NULL;
  return newNode;
}

void addEdge(struct Graph * graph, short int src, short int dest, short int weight, int row, int col) {

  struct AdjListNode * newNode = newAdjListNode(dest, weight);
  newNode->next = graph->array[src].head;
  graph->array[src].head = newNode;
  graph->array[src].row = row;
  graph->array[src].col = col;
}
