//Functions to evaluate tree

Tnode * Build_Uneval_Tree(char * filename, int * validity);
Tnode * Build_From_Preorder(int * pre, char * children, int * preIndex, int size);
Tnode * addNode(int keyVal);
int isBST(Tnode * tree);
int check_balance(Tnode * tree);
