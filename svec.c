/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "svec.h"

svec*
make_svec()
{
    //allocate and initialize values
    svec* sv = malloc(sizeof(svec));
    sv->data = malloc(2 * sizeof(char*));
    sv->size = 0;
    sv->capacity = 2;
    return sv;
}

void
free_svec(svec* sv)
{
    //only occupied space is within range [0,size)
    //don't have to free unnocupied space
    for(int ii = 0; ii < sv->size; ii++){
        free(sv->data[ii]);
    }
    free(sv->data);
    free(sv);
}

char*
svec_get(svec* sv, int ii)
{
    assert(ii >= 0 && ii < sv->size);
    //get the data since it has to be within bounds
    return sv->data[ii];
}

void
svec_put(svec* sv, int ii, char* item)
{
    assert(ii >= 0 && ii < sv->size);
    //use strdup to ensure ownership
    sv->data[ii] = strdup(item);
}

void
svec_push_back(svec* sv, char* item)
{
    int ii = sv->size;

    //if at maxmum capacity, reallocate memory
    if (sv->size == sv->capacity) {
        sv->capacity = sv->capacity * 2;
        //this function keeps data in sv->data, but increases capacity
        sv->data = realloc(sv->data, sv->capacity * sizeof(char*));
    }
    sv->size = ii + 1; //increase the size of the vector
    svec_put(sv, ii, item);

}

void
svec_swap(svec* sv, int ii, int jj)
{
    assert(ii >= 0 && ii < sv->size);
    assert(jj >= 0 && jj < sv->size);
    
    
    //if the indices are equal, don't waste the processing power
    if (ii == jj) {
        return;
    }
    
    //manually modifying the array didn't cause any memory leaks
    char* first_copy = sv->data[ii];
    sv->data[ii] = sv->data[jj];
    sv->data[jj] = first_copy;
    

}

svec* 
subsvec(svec* sv, int ii, int jj)
{
    //printf("ii: %d | jj: %d\n", ii, jj);
    
    assert(ii >= 0 && ii < sv->size);
    assert(jj >= 0 && jj < sv->size);
    assert(jj >= ii);
    
    svec* subsvec;
    subsvec = make_svec();
    
    for(ii; ii <= jj; ++ii) {
        svec_push_back(subsvec, svec_get(sv, ii));
    }
    return subsvec;
    
}

//returns -1 if the svec doesn't contain the item, otherwise it
//returns the first location in the svec that the item occurs in 
int 
svec_contains(svec*sv, char* item)
{
    for (int ii = 0; ii < sv->size; ++ii) {
        if (strcmp(svec_get(sv, ii), item) == 0) {
            return ii;
        }
    }
    return -1;
}

//prints the vector
void 
print_svec(svec* sv)
{
    for (int ii = 0; ii < sv->size; ++ii) {
        printf("%s ", svec_get(sv, ii));
    }
    printf(";\n");
}
