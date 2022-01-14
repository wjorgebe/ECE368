#ifndef __LIST_OF_LIST_H__
#define __LIST_OF_LIST_H__

#include "shell_list.h"

typedef struct _List {
   Node *node;
   struct _List *next;
} List;

#endif

