#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "vector.h"

void use_after_delete_test();
void insert_one_test();
void clear_test();
void push_back_test();
void pop_back_test();
void erase_test();
void erase_one_test();
void at_test();
void print_vector(vector *list); // this won't work for all types; may have to change it for your type

/* main method used for debugging */
int main(void) {
  use_after_delete_test();
  insert_one_test();
  clear_test();
  at_test();
  erase_test();
  erase_one_test();
  push_back_test();
  pop_back_test();

  printf("\nSuccess, all tests passed.\n");
  return EXIT_SUCCESS;
}

void use_after_delete_test() {
  vector myList = new_vector();
  for (int i = 0; i < 10; i++) {
    push_back(&myList, i+5);
  }
  delete_vector(&myList);

  // try removing from empty list, should fail
  int result = pop_back(&myList);
  assert(result == -1);
  result = erase_one(&myList, 0);
  assert(result == -1);

  // try reading an empty list, should fail
  assert(at(&myList, 0) == NULL);
  assert(at(&myList, 1) == NULL);

  // try adding to an empty list, should fail
  result = push_back(&myList, 42); // crashes
  assert(result == -1);
  result = insert_one(&myList, 42, 0);
  assert(result == -1);
}

void insert_one_test() {
  vector myList = new_vector();
  for (int i = 0; i < 10; i++) {
    push_back(&myList, i);
  }

  // insert at the end, in the middle, and the beginning
  insert_one(&myList, 42, 10); 
  assert(myList.elements == 11);
  assert(*at(&myList, 10) == 42);
  insert_one(&myList, 43, 5);
  assert(myList.elements == 12);
  assert(*at(&myList, 5) == 43);
  insert_one(&myList, 44, 0);
  assert(myList.elements == 13);
  assert(*at(&myList, 0) == 44);
  
  // insert at invalid index, should fail
  int result = insert_one(&myList, -1, 45);
  assert(result == -1);
  result = insert_one(&myList, 15, 45);
  assert(result == -1);

  delete_vector(&myList);
}

void clear_test() {
  vector myList = new_vector();
  for (int i = 0; i < 10; i++) {
    push_back(&myList, i+5);
  }

  clear(&myList);
  assert(myList.elements == 0);
  assert(at(&myList, 0) == NULL);

  // try removing from empty list, should fail
  int result = pop_back(&myList);
  assert(result == -1);
  result = erase_one(&myList, 0);
  assert(result == -1);

  push_back(&myList, 42);
  push_back(&myList, 43);
  push_back(&myList, 44);
  assert(*at(&myList, 1) == 43);
  //print_vector(&myList);

  delete_vector(&myList);
}

void push_back_test() {
  vector myList = new_vector();
  for (int i = 0; i < 10; i++) {
    push_back(&myList, i);
    assert(myList.data[i] == i);
  }
  delete_vector(&myList);
}

void pop_back_test() {
  vector myList = new_vector();
  for (int i = 0; i < 10; i++) {
    push_back(&myList, i);
  }

  pop_back(&myList);
  assert(myList.elements == 9);
  assert(at(&myList, 9) == NULL);
  assert(*at(&myList, 8) == 8);

  pop_back(&myList);
  assert(myList.elements == 8);
  assert(at(&myList, 8) == NULL);
  assert(*at(&myList, 7) == 7);

  //print_vector(&myList);

  delete_vector(&myList);
}

void at_test() {
  vector myList = new_vector();
  int max = 22; // insert the numbers 0 to max-1 into the vector
  for (int i = 0; i < max; i++) {
    push_back(&myList, i);
  }

  for (int i = 0; i < max; i++) {
    assert(*at(&myList, i) == i);
  }
  assert(myList.array_size == 32);
  assert(myList.elements == max);
  assert(at(&myList, -1) == NULL);
  assert(at(&myList, max) == NULL);

  delete_vector(&myList);
}

void erase_test() {
  vector myList = new_vector();

  for (uint8 i = 0; i < 22; i++) {
    push_back(&myList, i);
  }
  //print_vector(&myList);

  erase(&myList, 0, 7); // erase first 7 elements
  assert(myList.elements == (22-7));
  assert(myList.array_size == 32); // not small enough to reallocate yet
  assert(*at(&myList, 0) == 7);

  //print_vector(&myList);

  erase(&myList, 3, 12);
  assert(myList.elements == (22-7-9));
  assert(myList.array_size == 16); // down to 6 elements, now it should reallocate
  assert(*at(&myList, 3) == 19);

  //print_vector(&myList);

  erase(&myList, 0, 3);
  assert(myList.elements == (22-7-9-3));
  assert(myList.array_size == 8); // down to 3 elements, should reallocate again
  assert(*at(&myList, 0) == 19);

  //print_vector(&myList);

  erase(&myList, 0, 2);
  assert(myList.elements == 1);
  assert(myList.array_size == 4); // down to 1 element, should reallocate again
  assert(*at(&myList, 0) == 21);

  //print_vector(&myList);

  erase(&myList, 0, 1);
  assert(myList.elements == 0);
  assert(myList.array_size == 2); // empty, should reallocate again

  //print_vector(&myList);

  int b = erase(&myList, 0, 1); // try erasing from an empty list, shouldn't break anything
  assert(b); // should return -1 since there was an error
  assert(myList.elements == 0);
  assert(myList.array_size == 2); // empty, should reallocate again

  push_back(&myList, (uint8) 42);
  push_back(&myList, (uint8) 43);
  push_back(&myList, (uint8) 44);
  push_back(&myList, (uint8) 45);
  b = erase(&myList, 1, 1);
  assert(b);
  assert(myList.elements == 4);
  b = erase(&myList, INT_MAX, INT_MAX + 2); // error is "Index can't be negative"
  assert(b);
  b = erase(&myList, 2, INT_MAX);
  assert(b);
  assert(myList.elements == 4);

  //print_vector(&myList);

  b = erase(&myList, 0, 5);
  assert(b);
  b = erase(&myList, 1, 5);
  assert(b);
  erase(&myList, 0, 4);
  assert(myList.elements == 0);

  delete_vector(&myList);
}

void erase_one_test() {
  vector myList = new_vector();
  int max = 22; // insert the numbers 0 to max-1 into the vector
  for (int i = 0; i < max; i++) {
    push_back(&myList, i);
  }
  //print_vector(&myList);
  erase_one(&myList, 13);
  assert(*at(&myList, 12) == 12);

  assert(*at(&myList, 13) == 14);
  assert(myList.elements == max-1);
  //print_vector(&myList);

  delete_vector(&myList);
}

/* prints the elements of the list */
void print_vector(vector *list) {
  printf("VECTOR CONTENTS: ");
  int i;
  int elements = list->elements;
  if (elements != 0) {
    printf("[");
    for (i = 0; i < (elements-1); i++) {
      printf("%d, ", (int)(list->data[i]));
    }
    printf("%d]\n", (int)(list->data[i]));
  }
  else {
    printf("\n");
  }
  printf("ELEMENTS:      %d\n", list->elements);
  printf("ARRAY SIZE:    %d\n\n", list->array_size);
}
