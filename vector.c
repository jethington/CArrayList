#define ARRAYLIST_DEBUG

#ifdef ARRAYLIST_DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>

#include "ArrayList.h"

static void expand(ArrayList *list);
static void shrink(ArrayList *list);

/**********/
/* PUBLIC */
/**********/

// note: storing array will never go smaller than size 2

/* constructor */
ArrayList newArrayList() {
  ArrayList a;
  a.elements = 0;
  a.array_size = STARTING_SIZE;
  a.data = (TYPE_T*)malloc(STARTING_SIZE * DATA_SIZE);
#ifdef ARRAYLIST_DEBUG
  if (a.data == NULL) {
    fprintf(stderr, "Error allocating memory during newArrayList().\n");
  }
#endif
  return a;
}

/* copy constructor */
ArrayList copy_array_list(ArrayList* to_copy) {
  ArrayList a;
  a.elements = to_copy->elements;
  a.array_size = to_copy->array_size;
  a.data = (TYPE_T*)malloc(STARTING_SIZE * DATA_SIZE);
}

/* destructor */
/* note: ALWAYS call this once you're done with the ArrayList to free the memory being used */
void deleteArrayList(ArrayList *list) {
  free((void*)list->data);
  list->elements = 0;
  list->array_size = 0;
}

TYPE_T* at(ArrayList* list, int index) {
  if ((index >= list->elements) || (index < 0)) {
#ifdef ARRAYLIST_DEBUG
    fprintf(stderr, "Error: index to read item at is not valid.\n");
#endif
    return NULL;
  }
  return &(list->data[index]);
}

/* add an item to the list (adds to the end) */
void push_back(ArrayList* list, TYPE_T toAdd) {
  if (list->elements == list->array_size) {
    expand(list);
  }
  list -> data[list->elements] = toAdd;
  list -> elements++;
}

/* add an item to the list at the index specified */
/* the item already at that index and all following items are shifted up one index */
void addItemAt(ArrayList* list, TYPE_T toAdd, int index) {
  if ((index > list->elements) || (index < 0)) {
    fprintf(stderr, "Error: index to add item at is not valid.\n");
    return;
  }
  if (list->elements == list->array_size) {
    expand(list);
  }
  for (int i = (list->elements) - 1; i >= index; i--) {
    list->data[i+1] = list->data[i];
  }
  list->data[index] = toAdd;
  list->elements++;
}

/* remove an item from the list (removes the last one) */
TYPE_T pop_back(ArrayList* list) {
  if (list->elements == 0) {
    fprintf(stderr, "Error: tried to remove from an empty list.\n");
  }
  if ((list->elements) - 1 < (list->array_size) / 4) {
    shrink(list);
  }
  TYPE_T result = (list->data)[(list->elements) - 1];
  list->elements--;
  return result;
}

/* removes all items in the range [first, last), including first but not last */
/* returns true if the function was successful, or false if there was an error */
// return 0 on success instead? -> return an error enum
bool erase(ArrayList* list, int first, int last) {
  if ((first < 0) || (last <= 0)) {
#ifdef ARRAYLIST_DEBUG
    fprintf(stderr, "Error: index can't be negative.\n");
#endif
    return false;
  }
  if (first >= last) {
#ifdef ARRAYLIST_DEBUG
    fprintf(stderr, "Error: first index is higher than or equal to last index.\n");
#endif
    return false;
  }
  if ((first > list->elements) || (last > list->elements)) {
#ifdef ARRAYLIST_DEBUG
    fprintf(stderr, "Error: one of the arguments is too large.\n");
#endif
    return false;
  }
  if ((first + last) > list->elements) { // first + last == elements -> error?
#ifdef ARRAYLIST_DEBUG
    fprintf(stderr, "Error: range extends past last index.\n");
#endif
    return false;
  }

  int remove_length = last - first;
  int new_size = list->elements - (last - first); // remove(0, 5) should remove 0..4, so 5 elements total

  for (int i = first; i < new_size; i++) {
    list->data[i] = list->data[i+remove_length];
  }

  if (new_size < (list->array_size) / 4) {
    shrink(list);
  }
  list->elements = new_size;
  return true;
}

/* removes one item at the specified index */
/* returns true if the function was successful, or false if there was an error */
bool erase_one(ArrayList* list, int index) {
  return erase(list, index, index+1);
}

/***********/
/* PRIVATE */
/***********/

/* double the size of the storing array */
/* called when the storing array is full */
static void expand(ArrayList* list) {
  int new_size = (list->array_size) * 2;
  list->data = (TYPE_T*)realloc((void*)list->data, new_size * DATA_SIZE);
  if (list->data == 0) {
#ifdef ARRAYLIST_DEBUG
    fprintf(stderr, "Error allocating memory during expand().\n");
#endif
  }
  list->array_size = new_size;
}

/* cut the size of the storing array in half */
/* called when the storing array is less than 1/4 full */
static void shrink(ArrayList* list) {
  int new_size = (list->array_size) / 2;
  list->data = (TYPE_T*)realloc((void*)list->data, new_size * DATA_SIZE);
  if (list->data == 0) {
#ifdef ARRAYLIST_DEBUG
    fprintf(stderr, "Error allocating memory during shrink().\n");
#endif
  }
  list->array_size = new_size;
}
