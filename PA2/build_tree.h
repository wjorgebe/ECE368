//Functions to build tree
Tnode * buildTree(char * filename, int * validity, int * size);
Tnode * Build_Tree_From_Inst(int * keys, char * instructions, int size);
Tnode * insertNode(Tnode * node, int key);
Tnode * deleteNode(Tnode * node, int key);
Tnode * leftRotate(Tnode * node);
Tnode * rightRotate(Tnode * node);
void Write_Tree_to_File(char * filename, Tnode * tree, int size);
void Fill_Arrays(Tnode * tree, int * keys, char * children, int * index, int size);
