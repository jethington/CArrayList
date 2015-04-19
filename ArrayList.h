#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#define TYPE_T uint8 // change this to the type you want in your list
#define DATA_SIZE sizeof(TYPE_T)
#define STARTING_SIZE 8

typedef unsigned char uint8;

typedef struct ArrayList_struct {
    int elements; // number of elements in array
    int arraySize; // size of containing array (user should never need this)
    TYPE_T *data; // actual elements of the arraylist
} ArrayList;

ArrayList newArrayList();
void deleteArrayList(ArrayList *list);
//TYPE_T get(ArrayList* list, int index);
TYPE_T* at(ArrayList* list, int index);
void addItem(ArrayList* list, TYPE_T toAdd);
void addItemAt(ArrayList* list, TYPE_T toAdd, int index);
TYPE_T removeItem(ArrayList* list);
TYPE_T removeItemAt(ArrayList* list, int index);

#endif
