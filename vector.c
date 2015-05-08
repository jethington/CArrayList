#define VECTOR_DEBUG

#ifdef VECTOR_DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "vector.h"

static void expand(vector *list);
static void shrink(vector *list);

/**********/
/* PUBLIC */
/**********/

// note: storing array will never go smaller than size 2

/* constructor */
vector new_vector() {
  vector a;
  a.elements = 0;
  a.array_size = STARTING_SIZE;
  a.data = (TYPE_T*)malloc(STARTING_SIZE * sizeof(TYPE_T));
#ifdef VECTOR_DEBUG
  if (a.data == NULL) {
    fprintf(stderr, "Error allocating memory during new_vector().\n");
  }
#endif
  return a;
}

/* copy constructor, performs a deep copy of the underlying data */
vector copy_vector(const vector* to_copy) {
  vector a;
  a.elements = to_copy->elements;
  a.array_size = to_copy->array_size;
  a.data = (TYPE_T*)malloc(STARTING_SIZE * sizeof(TYPE_T));
  memcpy(a.data, to_copy->data, to_copy->elements * sizeof(TYPE_T)); // destination, source
  return a;
}

/* destructor */
/* note: ALWAYS call this once you're done with the vector to free the memory being used */
void delete_vector(vector *list) {
  free((void*)list->data);
  list->elements = 0;
  list->array_size = 0;
}

/* returns a pointer to the item at the specified index, or NULL if there is an error */
TYPE_T* at(const vector* list, int index) {
  if ((index >= list->elements) || (index < 0)) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error: index to read item at is not valid.\n");
#endif
    return NULL;
  }
  return &(list->data[index]);
}

/* add an item to the vector (adds to the end) */
void push_back(vector* list, TYPE_T to_add) {
  if (list->elements == list->array_size) {
    expand(list);
  }
  list -> data[list->elements] = to_add;
  list -> elements++;
}

/* remove an item from the vector (removes the last one) */
/* returns 0 if the function was successful, or -1 if there was an error */
int pop_back(vector* list) {
  if (list->elements == 0) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error: tried to remove from an empty list.\n");
#endif
    return -1;
  }
  if ((list->elements) - 1 < (list->array_size) / 4) {
    shrink(list);
  }
  list->elements--;
  return 0;
}

/* add an item to the list at the index specified */
/* the item already at that index and all following items are shifted up one index */
/*void addItemAt(vector* list, TYPE_T to_add, int index) {
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
  list->data[index] = to_add;
  list->elements++;
}*/

/* add an item to the vector at the index specified */
/* returns 0 if the function was successful, or -1 if there was an error */
int insert_one(vector* list, TYPE_T to_add, int index) {
  if ((index > list->elements) || (index < 0)) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error: index to add item at is not valid.\n");
#endif
    return -1;
  }
  if (list->elements == list->array_size) {
    expand(list);
  }
  for (int i = (list->elements) - 1; i >= index; i--) {
    list->data[i+1] = list->data[i];
  }
  list->data[index] = to_add;
  list->elements++;

  return 0;
}

/* removes all items in the range [first, last), including first but not last */
/* returns 0 if the function was successful, or -1 if there was an error */
int erase(vector* list, int first, int last) {
  if ((first < 0) || (last <= 0)) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error: index can't be negative.\n");
#endif
    return -1;
  }
  if (first >= last) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error: first index is higher than or equal to last index.\n");
#endif
    return -1;
  }
  if ((first > list->elements) || (last > list->elements)) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error: one of the arguments is too large.\n");
#endif
    return -1;
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
  return 0;
}

/* removes one item at the specified index */
/* returns 0 if the function was successful, or -1 if there was an error */
int erase_one(vector* list, int index) {
  return erase(list, index, index+1);
}

void clear(vector* list) {
  list->elements = 0;
}

/***********/
/* PRIVATE */
/***********/

/* double the size of the storing array */
/* called when the storing array is full */
static void expand(vector* list) {
  int new_size = (list->array_size) * 2;
  list->data = (TYPE_T*)realloc((void*)list->data, new_size * sizeof(TYPE_T));
  if (list->data == 0) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error allocating memory during expand().\n");
#endif
  }
  list->array_size = new_size;
}

/* cut the size of the storing array in half */
/* called when the storing array is less than 1/4 full */
static void shrink(vector* list) {
  int new_size = (list->array_size) / 2;
  list->data = (TYPE_T*)realloc((void*)list->data, new_size * sizeof(TYPE_T));
  if (list->data == 0) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error allocating memory during shrink().\n");
#endif
  }
  list->array_size = new_size;
}
