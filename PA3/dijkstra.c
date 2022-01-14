#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "pa3.h"

struct MinHeapNode * newMinHeapNode(short int v, short int dist) {
  struct MinHeapNode * minHeapNode = malloc(sizeof(struct MinHeapNode));
  minHeapNode->v = v;
  minHeapNode->dist = dist;
  return minHeapNode; //return allocated node
}

struct MinHeap * createMinHeap(short int capacity) {
  struct MinHeap * minHeap = malloc(sizeof(struct MinHeap));
  minHeap->position = malloc(capacity * sizeof(short));
  minHeap->size = 0;
  minHeap->capacity = capacity;
  minHeap->array = malloc(capacity * sizeof(struct MinHeapNode *));

  return minHeap; //return allocated minHeap
}

void swapMinHeapNode(struct MinHeapNode ** a, struct MinHeapNode ** b) {
  struct MinHeapNode * t = *a;  //temporary node
  *a = *b;
  *b = t;
}

void minHeapify(struct MinHeap* minHeap, short int idx)
{
    short int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;

    if (smallest != idx)
    {
        //Min heap nodes to be used and swapped
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        //Swapping positions of selected nodes
        minHeap->position[smallestNode->v] = idx;
        minHeap->position[idxNode->v] = smallest;

        //Swapping said nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        //Heapify the list to get smallest weight at top
        minHeapify(minHeap, smallest);
    }
}

void decreaseKey(struct MinHeap* minHeap, short int v, short int dist)
{
    //Get index
    short int i = minHeap->position[v];

    //Change distance value
    minHeap->array[i]->dist = dist;


    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        //Swap node with parent node
        minHeap->position[minHeap->array[i]->v] = (i-1)/2;
        minHeap->position[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        //Evaluate at parent index
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap *minHeap, short int v)
{
   if (minHeap->position[v] < minHeap->size)
     return true; //Element not in heap or was removed
   return false;  //Element is in heap
}

short int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;  //true is minHeap is empty, use ! to falsify
}

struct MinHeapNode* extractMin(struct MinHeap * minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    //Update positions
    minHeap->position[root->v] = minHeap->size-1;
    minHeap->position[lastNode->v] = 0;

    //Reduce size and heapify minHeap
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

short int * dijkstra(struct Graph* graph, short int src, short int * parent)
{

    //V represents rows*columns, full capacity of graph
    short int V = graph->V;

    //distance value
    short int * dist = malloc(V * sizeof(short));

    //minHeap for edge
    struct MinHeap* minHeap = createMinHeap(V);

    for (short int v = 0; v < V; ++v)
    {
      if (v != src) {
        dist[v] =  SHRT_MAX;
        parent[v] = -1;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->position[v] = v;
      }
      else {
        minHeap->array[src] = newMinHeapNode(src, dist[src]);
        minHeap->position[src] = src;
        dist[src] = 0;
        parent[src] = -1;
        decreaseKey(minHeap, src, dist[src]);
      }
    }

    minHeap->size = V;

    //Dijkstra evalutation with heap
    while (!isEmpty(minHeap))
    {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);

        //Vertex number, not V
        short int u = minHeapNode->v;

        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            short int v = pCrawl->dest;

            if (isInMinHeap(minHeap, v) && dist[u] != SHRT_MAX && pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
                parent[v] = u;

                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }

        free (minHeapNode);
    }

    //Freeing everything used
    free(minHeap->array);
    free(minHeap->position);
    free(minHeap);
    return dist;
}
