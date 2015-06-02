/* Copyright (c) 2014, Jason Ethington
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#define VECTOR_DEBUG

#ifdef VECTOR_DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "vector.h"

static int expand(vector *list);
static int shrink(vector *list);

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
/* returns 0 if the function was successful, or -1 if there was an error */
int push_back(vector* list, TYPE_T to_add) {
  if (list->elements == list->array_size) {
    int result = expand(list);
    if (result == -1) { // expand failed
      return -1;
    }
  }
  list -> data[list->elements] = to_add;
  list -> elements++;
  return 0;
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

/* inserts one vector into another, starting at the index specified */
/* ex: Vector A = [0, 1, 2, 3], Vector B = [10, 11, 12] */
/*     insert(A, B, 2) results in Vector A containing [0, 1, 10, 11, 12, 2, 3] */
/* returns 0 if the function was successful, or -1 if there was an error */

int insert(vector* add_to, vector* add_from, int start_index) {
  if (start_index < 0) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error: cannot insert at a negative index.\n");
#endif
    return -1;
  }
  if (start_index > add_to->elements) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error: start_index is out of bounds.\n");
#endif
    return -1;
  }

	int new_elements = add_to->elements + add_from->elements;
	if (new_elements < add_to->elements) {
#ifdef VECTOR_DEBUG
		fprintf(stderr, "Error: overflow during insert.\n");
#endif
		return -1;
	}

	// expand until add_to vector is large enough to hold all elements
	// allows exactly filled to capacity, is that a problem?  I think its ok
	while (add_to->array_size < new_elements) {
		int result = expand(add_to);
		if (result == -1) {
			return -1;
		}
	}

  // shift old elements up the appropriate amount
  int shift = add_from->elements;
  for (int i = start_index; i < add_to->elements; i++) {
    add_to->data[i + shift] = add_to->data[i];
  }

  // fill in new elements
  for (int i = 0; i < add_from->elements; i++) {
    add_to->data[start_index + i] = add_from->data[i];
  }

  add_to->elements = new_elements;

	/*int shift = add_from->elements;
	for (int i = start_index; i < shift; i++) {
		add_to->data[start_index+i+shift] = add_to->data[start_index+i];
		add_to->data[start_index+i] = add_from->data[i]
	}*/

  return 0;
}

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
    int result = expand(list);
    if (result == -1) { // expand failed
      return -1;
    }
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

/* returns true if the list is empty and false otherwise */
bool is_empty(vector* list) {
  return list->elements == 0;
}

/* removes all elements from the list */
/* unlike delete_vector, this function holds onto the already-allocated buffer, so delete_vector still needs to be */
/* called when you are done with this vector */
void clear(vector* list) {
  list->elements = 0;
}

/***********/
/* PRIVATE */
/***********/

/* double the size of the storing array */
/* called when the storing array is full */
static int expand(vector* list) {
  if (list->array_size == 0) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error: attempting to use vector after it has been freed.\n");
#endif
    return -1;
  }
  int new_size = (list->array_size) * 2;
  list->data = (TYPE_T*)realloc((void*)list->data, new_size * sizeof(TYPE_T));
  if (list->data == NULL) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error allocating memory during expand().\n");
#endif
    return -1;
  }
  list->array_size = new_size;
  return 0;
}

/* cut the size of the storing array in half */
/* called when the storing array is less than 1/4 full */
static int shrink(vector* list) {
  int new_size = (list->array_size) / 2;
  list->data = (TYPE_T*)realloc((void*)list->data, new_size * sizeof(TYPE_T));
  if (list->data == NULL) {
#ifdef VECTOR_DEBUG
    fprintf(stderr, "Error allocating memory during shrink().\n");
#endif
    return -1;
  }
  list->array_size = new_size;
  return 0;
}
