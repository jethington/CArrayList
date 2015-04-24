#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ArrayList.h"

void erase_test();
void at_test();
void print_list(ArrayList *list); // this won't work for all types; may have to change it for your type

/* main method used for debugging */
int main(void) {
  at_test();
  erase_test();
  return EXIT_SUCCESS;
}

void at_test() {
  ArrayList myList = newArrayList();
  int max = 22; // insert the numbers 0 to max-1 into the arraylist
  for (int i = 0; i < max; i++) {
    addItem(&myList, i);
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
    addItem(&myList, i);
  }
  print_list(&myList);

  //removeItemAt(&myList, 0);
  //removeItemAt(&myList, 15);

  erase(&myList, 0, 7); // erase first 7 elements
  assert(myList.elements == (22-7));
  assert(myList.array_size == 32); // not small enough to reallocate yet
  assert(*at(&myList, 0) == 7);

  print_list(&myList);

  erase(&myList, 3, 12);
  assert(myList.elements == (22-7-9));
  assert(myList.array_size == 16); // down to 6 elements, now it should reallocate
  assert(*at(&myList, 3) == 19);

  print_list(&myList);

  erase(&myList, 0, 3);
  assert(myList.elements == (22-7-9-3));
  assert(myList.array_size == 8); // down to 3 elements, should reallocate again
  assert(*at(&myList, 0) == 19);

  print_list(&myList);

  erase(&myList, 0, 2);
  assert(myList.elements == 1);
  assert(myList.array_size == 4); // down to 1 element, should reallocate again
  assert(*at(&myList, 0) == 21);

  print_list(&myList);

  erase(&myList, 0, 1);
  assert(myList.elements == 0);
  assert(myList.array_size == 2); // empty, should reallocate again

  print_list(&myList);

  bool b = erase(&myList, 0, 1); // try erasing from an empty list, shouldn't break anything
  assert(!b); // should return false since there was an error
  print_list(&myList);
  //assert(myList.elements == 0);



  deleteArrayList(&myList);
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
