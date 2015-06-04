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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "vector.h"

void insert_test();
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
/*int main(void) {
  insert_test();
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

void insert_test() {
  vector a = new_vector();
  for (int i = 0; i <= 3; i++) {
    push_back(&a, i);
  }
  vector b = new_vector();
  for (int i = 0; i <= 2; i++) {
    push_back(&b, i + 10);
  }

  int ret = insert(&a, &b, 2);
  assert(ret == 0);
  assert(*at(&a, 2) == 10);
  assert(*at(&a, 5) == 2);
  assert(a.elements == 7);

  // insert b larger than a; force a to expand multiple times
  clear(&b);
  for (int i = 0; i < 20; i++) {
    push_back(&b, i + 100);
  }
  ret = insert(&a, &b, 5);
  assert(ret == 0);
  assert(*at(&a, 2) == 10);
  assert(*at(&a, 5) == 100);
  assert(*at(&a, 24) == 119);
  assert(*at(&a, 25) == 2);
  assert(a.elements == 27);
  assert(a.array_size == 32);

  // insert starting after end of the other array, leaving a gap: should fail
  ret = insert(&a, &b, 28);
  assert(ret == -1);

  // inserting at a negative index: should fail
  ret = insert(&a, &b, -1);
  assert(ret == -1);

  vector c = new_vector();
  push_back(&c, 0);
  push_back(&c, 0);
  push_back(&c, 0);

  // insert at very start and very end; both should work
  ret = insert(&a, &c, 0);
  assert(ret == 0);
  ret = insert(&a, &c, a.elements);
  assert(ret == 0);

  // this should never happen anyways since you would run out of memory first, but just to be safe...
  c.elements = INT_MAX - 1;
  ret = insert(&a, &c, 0);
  assert(ret == -1);
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

// prints the elements of the list
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
}*/
