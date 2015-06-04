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

#ifndef VECTOR_H_
#define VECTOR_H_

#define TYPE_T uint8 // change this to the type you want in your list
#define STARTING_SIZE 8

#include <stdbool.h>

// for running the main tests, use an integer type
// for use in an application, struct types are fine as well
typedef unsigned char uint8;

// for running the struct test, which is just to prove that all the assignment operators still work with structs
// note that struct assignment only performs a shallow copy
/*typedef struct point {
  int x;
  int y;
} point;*/

typedef struct vector_struct {
  int elements; // number of elements in array
  int array_size; // size of containing array (user should never need this)
  TYPE_T *data; // actual elements of the vector
} vector;

vector new_vector();
vector copy_vector(const vector* to_copy);
void delete_vector(vector* list);
TYPE_T* at(const vector* list, int index);
int push_back(vector* list, TYPE_T to_add);
int insert(vector* add_to, vector* add_from, int start_index);
int insert_one(vector* list, TYPE_T to_add, int start_index);
int pop_back(vector* list);
int erase(vector* list, int first, int last);
int erase_one(vector* list, int index);
bool is_empty(vector* list);
void clear(vector* list);

#endif
