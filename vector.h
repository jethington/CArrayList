#ifndef VECTOR_H_
#define VECTOR_H_

#define TYPE_T uint8 // change this to the type you want in your list
#define STARTING_SIZE 8

typedef unsigned char uint8;

typedef struct vector_struct {
  int elements; // number of elements in array
  int array_size; // size of containing array (user should never need this)
  TYPE_T *data; // actual elements of the vector
} vector;

vector new_vector();
vector copy_vector(const vector* to_copy);
void delete_vector(vector* list);
TYPE_T* at(const vector* list, int index);
void push_back(vector* list, TYPE_T to_add);
int insert_one(vector* list, TYPE_T to_add, int start_index);
int pop_back(vector* list);
int erase(vector* list, int first, int last);
int erase_one(vector* list, int index);
void clear(vector* list);

#endif
