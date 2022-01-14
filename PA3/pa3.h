//Structure Definitions

//For graph
struct AdjListNode {
  short int dest;
  short int weight;
  struct AdjListNode * next;
};

struct AdjList {
  short int row;
  short int col;
  struct AdjListNode * head;
};

struct Graph {
  short int V;
  struct AdjList * array;
};

//For Min Heap
struct MinHeapNode {
  short int v;
  short int dist;
};

struct MinHeap {
  short int size;
  short int capacity;
  short int * position;
  struct MinHeapNode ** array;
};

//Function Declarations
  //pa3.c
void Print_Fastest_Times(char * filename, int * minTimes, short int columns);
struct Graph * Read_From_File(char * input, char * output, short int * r, short int * col);

  //make_list.c
struct Graph * createGraph(short int V);
struct AdjListNode * newAdjListNode (short int dest, short int weight);
void addEdge(struct Graph * graph, short int src, short int dest, short int weight, int row, int col);

//   //dijkstra.c
struct MinHeapNode * newMinHeapNode(short int v, short int dist);
struct MinHeap * createMinHeap(short int capacity);
void swapMinHeapNode(struct MinHeapNode ** a, struct MinHeapNode ** b);
void minHeapify(struct MinHeap* minHeap, short int idx);
void decreaseKey(struct MinHeap* minHeap, short int v, short int dist);
bool isInMinHeap(struct MinHeap *minHeap, short int v);
short int * dijkstra(struct Graph* graph, short int src, short int * parent);
short int isEmpty(struct MinHeap* minHeap);
struct MinHeapNode* extractMin(struct MinHeap * minHeap);
