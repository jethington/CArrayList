#include <stdlib.h>
#include <stdio.h>

#include "ArrayList.h"

void printList(ArrayList *list); // this won't work for all types; may have to change it for your type

/* main method used for debugging */
int main(int argc, char **argv) {
  ArrayList myList = newArrayList();
  uint8 i;

  for (i = 0; i < 22; i++) {
    addItem(&myList, i);
  }
  printList(&myList);

  removeItemAt(&myList, 0);
  //removeItemAt(&myList, 15);
  printList(&myList);
  
  deleteArrayList(&myList);
  //printf("test3\n");
  return EXIT_SUCCESS;
}

/* prints the elements of the list */
void printList(ArrayList *list) {
  printf("LIST CONTENTS: ");  
  int i;
  int elements = list -> elements;
  printf("[");
  for (i = 0; i < (elements-1); i++) {
    printf("%d, ", (int)(list -> data[i]));
  }
  printf("%d]\n", (int)(list -> data[i]));
  printf("ELEMENTS:      %d\n", list->elements);
  printf("ARRAY SIZE:    %d\n\n", list->arraySize);
}
