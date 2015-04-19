#include <stdlib.h>
#include <stdio.h>

#include "ArrayList.h"

static void expand(ArrayList *list);
static void shrink(ArrayList *list);

// ERROR HANDLING:

// at: return pointer to value, return null if error
// remove: delete a range of values, not just one.  Return error info.
// pop_back: return error info

/**********/
/* PUBLIC */
/**********/

// note: storing array will never go smaller than size 2

/* constructor */
ArrayList newArrayList() {
  ArrayList a;
  a.elements = 0;
  a.arraySize = STARTING_SIZE;
  a.data = (TYPE_T*)malloc(STARTING_SIZE * DATA_SIZE);
  if (a.data == 0) {
    fprintf(stderr, "\nError allocating memory.");
    exit(EXIT_FAILURE);
  }
  return a;
}

/* destructor */
/* note: ALWAYS call this once you're done with the ArrayList to free the memory being used */
void deleteArrayList(ArrayList *list) {
  free((void*)list->data);
}

/* read the element at the argument index */
/*TYPE_T get(ArrayList *list, int index) {
  if ((index > list->elements) || (index < 0)) {
    fprintf(stderr, "\nError: index to read item at is not valid.");
    exit(EXIT_FAILURE);
  }
  return list->data[index];
}*/
TYPE_T* at(ArrayList* list, int index) {
  if ((index > list->elements) || (index < 0)) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "\nError: index to read item at is not valid.");
#endif
    return NULL;
  }
  return &(list->data[index]);
}

/* add an item to the list (adds to the end) */
void addItem(ArrayList* list, TYPE_T toAdd) {
  if (list->elements == list->arraySize) {
    expand(list);
  }
  list -> data[list->elements] = toAdd;
  list -> elements++;
}

/* add an item to the list at the index specified */
/* the item already at that index and all following items are shifted up one index */
void addItemAt(ArrayList* list, TYPE_T toAdd, int index) {
  int i;
  if ((index > list->elements) || (index < 0)) {
    fprintf(stderr, "\nError: index to add item at is not valid.");
    exit(1);
  }
  if (list->elements == list->arraySize) {
    expand(list);
  }
  for (i = (list->elements) - 1; i >= index; i--) {
    list->data[i+1] = list->data[i];
  }
  list->data[index] = toAdd;
  list->elements++;
}

/* remove an item from the list (removes the last one) */
TYPE_T removeItem(ArrayList* list) {
  if (list->elements == 0) {
    fprintf(stderr, "\nError: tried to remove from an empty list.");
    exit(EXIT_FAILURE);
  }
  if ((list->elements) - 1 < (list->arraySize) / 4) {
    shrink(list);
  }
  TYPE_T result = (list->data)[(list->elements) - 1];
  list->elements--;
  return result;
}

/* remove an item at the specified index from the list */
/* items at a higher index are shifted down one */
TYPE_T removeItemAt(ArrayList* list, int index) {
  if ((index >= list->elements) || (index < 0)) {
    fprintf(stderr, "\nError: index to remove item from is not valid.");
    exit(EXIT_FAILURE);
  }
  int i;
  if ((list->elements) - 1 < (list->arraySize) / 4) {
    shrink(list);
  }
  TYPE_T result = (list->data)[index];
  for (i = index; i < (list->elements) - 1; i++) {
    list->data[i] = list->data[i+1];
  }
  // quicker to store data in function?
  list->elements--;
  return result;
}

/***********/
/* PRIVATE */
/***********/

/* double the size of the storing array */
/* called when the storing array is full */
static void expand(ArrayList* list) {
  int newSize = (list->arraySize) * 2;
  list->data = (TYPE_T*)realloc((void*)list->data, newSize * DATA_SIZE);
  if (list->data == 0) {
    fprintf(stderr, "\nError allocating memory.");
    exit(EXIT_FAILURE);
  }
  list->arraySize = newSize;
}

/* cut the size of the storing array in half */
/* called when the storing array is less than 1/4 full */
static void shrink(ArrayList* list) {
  int newSize = (list->arraySize) / 2;
  list->data = (TYPE_T*)realloc((void*)list->data, newSize * DATA_SIZE);
  if (list->data == 0) {
    fprintf(stderr, "\nError allocating memory.");
    exit(EXIT_FAILURE);
  }
  list->arraySize = newSize;
}
