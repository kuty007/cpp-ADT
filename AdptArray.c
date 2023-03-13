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
    array->array = NULL;//initialize the array to NULL

    return array;
}

Result SetAdptArrayAt(PAdptArray array, int index, PElement element) {
    if (array == NULL) {//if the array is null
        return FAIL;
    }
    if (index >= array->size) {//if the index is bigger than the size of the array
        void **newArray = (void **) calloc(index + 1, sizeof(void *));//allocate space for a new array
        if (newArray == NULL) {
            printf("Error: malloc has failed\n");
            return FAIL;
        }
        for (int i = 0; i < array->size; i++) {//for each index in the array
            newArray[i] = array->array[i];//copy the old array to the new array
        }
        free(array->array);//free the old array
        array->array = newArray;//set the array to the new array
        array->size = index + 1;//set the size to the index + 1
    }
    if (array->array[index] != NULL) {//if the index is not null
        array->freeElement(array->array[index]);//delete the element at the index
    }
    array->array[index] = array->copyElement(element);//copy the element to the index
    return SUCCESS;
}

//get a copy of the element at the index

PElement GetAdptArrayAt(PAdptArray array, int index) {
    if (index >= array->size) {//if the index is bigger than the size of the array
        return NULL;
    }
    if (array->array[index] == NULL) {//if the index is null
        return NULL;
    }
    return array->copyElement(array->array[index]);//copy_person or copy_book
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
                array->freeElement((array->array)[i]);//delete_person or delete_book
            }
        }
        free(array->array);//free the array
    }
    free(array);//free the AdptArray struct
}
