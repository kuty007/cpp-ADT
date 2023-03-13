#include "AdptArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct AdptArray_ {
    void **array;
    int size;
    COPY_FUNC copyElement;
    DEL_FUNC freeElement;
    PRINT_FUNC printElement;
};

typedef struct AdptArray_ AdptArray_;
typedef AdptArray_ *PAdptArray;

PAdptArray CreateAdptArray(COPY_FUNC copyElement, DEL_FUNC freeElement, PRINT_FUNC printElement) {
    PAdptArray array = (PAdptArray) malloc(sizeof(AdptArray_));//allocate space for the array
    if (array == NULL) {
        printf("Error: malloc has failed\n");
        exit(1);
    }
    array->size = 0;//initialize the size to 0
    array->copyElement = copyElement;//initialize the copy function
    array->freeElement = freeElement;//initialize the delete function
    array->printElement = printElement;//initialize the print function
    array->array = NULL;//initialize array to NULL
    return array;
}

Result SetAdptArrayAt(PAdptArray array, int index, PElement element) {
    if (index >= array->size) {//if the index is bigger than the size of the array
        array->array = (void **) realloc(array->array, sizeof(void *) * (index + 1));//allocate space for the new index
        if (array->array == NULL) {
            printf("Error: realloc has failed\n");
            exit(1);
        }
        for (int i = array->size; i < index; i++) {//initialize the new indexes to NULL
            array->array[i] = NULL;//NULL
        }
        array->size = index + 1;//update the size of the array
    }
    if (array->array[index] != NULL) {//if the index is not null
        array->freeElement(array->array[index]);//delete_person or delete_book
    }
    array->array[index] = array->copyElement(element);//copy_person or copy_book
}

PElement GetAdptArrayAt(PAdptArray array, int index) {
    if (index >= array->size) {//if the index is bigger than the size of the array
        return NULL;//NULL
    }
    return array->array[index];//return the element in the index
}

int GetAdptArraySize(PAdptArray array) {
    return array->size;//return the size of the array
}

void PrintDB(PAdptArray array) {
    for (int i = 0; i < array->size; i++) {//for each index in the array
        if (array->array[i] != NULL) {//if the index is not null
            array->printElement(array->array[i]);//print_person or print_book
        }
    }
}

void DeleteAdptArray(PAdptArray array) {
    if (array->array != NULL) {
        for (int i = 0; i < array->size; i++) {//for each index in the array
            if (array->array[i] != NULL) {//if the index is not null
                array->freeElement(array->array[i]);//delete_person or delete_book
            }
        }
        free(array->array);//free the array
    }
    free(array);//free the AdptArray struct
}
