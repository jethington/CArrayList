#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
//#include <cstdarg.h>

#include "ArrayList.h"

void erase_test();
void erase_one_test();
void at_test();
void print_list(ArrayList *list); // this won't work for all types; may have to change it for your type

/* main method used for debugging */
int main(void) {
  at_test();
  erase_test();
  erase_one_test();
  return EXIT_SUCCESS;
}

void at_test() {
  ArrayList myList = newArrayList();
  int max = 22; // insert the numbers 0 to max-1 into the arraylist
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

  deleteArrayList(&myList);
}

void erase_test() {
  ArrayList myList = newArrayList();

  for (uint8 i = 0; i < 22; i++) {
    push_back(&myList, i);
  }
  //print_list(&myList);

  erase(&myList, 0, 7); // erase first 7 elements
  assert(myList.elements == (22-7));
  assert(myList.array_size == 32); // not small enough to reallocate yet
  assert(*at(&myList, 0) == 7);

  //print_list(&myList);

  erase(&myList, 3, 12);
  assert(myList.elements == (22-7-9));
  assert(myList.array_size == 16); // down to 6 elements, now it should reallocate
  assert(*at(&myList, 3) == 19);

  //print_list(&myList);

  erase(&myList, 0, 3);
  assert(myList.elements == (22-7-9-3));
  assert(myList.array_size == 8); // down to 3 elements, should reallocate again
  assert(*at(&myList, 0) == 19);

  //print_list(&myList);

  erase(&myList, 0, 2);
  assert(myList.elements == 1);
  assert(myList.array_size == 4); // down to 1 element, should reallocate again
  assert(*at(&myList, 0) == 21);

  //print_list(&myList);

  erase(&myList, 0, 1);
  assert(myList.elements == 0);
  assert(myList.array_size == 2); // empty, should reallocate again

  //print_list(&myList);

  bool b = erase(&myList, 0, 1); // try erasing from an empty list, shouldn't break anything
  assert(!b); // should return false since there was an error
  assert(myList.elements == 0);
  assert(myList.array_size == 2); // empty, should reallocate again

  push_back(&myList, (uint8) 42);
  push_back(&myList, (uint8) 43);
  push_back(&myList, (uint8) 44);
  push_back(&myList, (uint8) 45);
  b = erase(&myList, 1, 1);
  assert(!b);
  assert(myList.elements == 4);
  b = erase(&myList, INT_MAX, INT_MAX + 2); // error is "Index can't be negative"
  assert(!b);
  b = erase(&myList, 2, INT_MAX); // this one fails, things break
  assert(!b);
  assert(myList.elements == 4);

  //print_list(&myList);

  b = erase(&myList, 0, 5);
  assert(!b);
  b = erase(&myList, 1, 5);
  assert(!b);
  erase(&myList, 0, 4);
  assert(myList.elements == 0);

  deleteArrayList(&myList);
}

void erase_one_test() {
  ArrayList myList = newArrayList();
  int max = 22; // insert the numbers 0 to max-1 into the arraylist
  for (int i = 0; i < max; i++) {
    push_back(&myList, i);
  }

  erase_one(&myList, 13);
  assert(*at(&myList, 12) == 12);
  assert(*at(&myList, 13 == 14));
  assert(myList.elements == max-1);
  print_list(&myList);
}

/* prints the elements of the list */
void print_list(ArrayList *list) {
  printf("LIST CONTENTS: ");
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
