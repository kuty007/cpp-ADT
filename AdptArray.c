#include "AdptArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct AdptArray_ {
    void **Earray;
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
        return NULL;
    }
    array->size = 0;//initialize the size to 0
    array->copyElement = copyElement;//initialize the copy function
    array->freeElement = freeElement;//initialize the delete function
    array->printElement = printElement;//initialize the print function
    array->Earray = NULL;//initialize the elements array to NULL

    return array;
}

Result SetAdptArrayAt(PAdptArray array, int index, PElement element) {
    if (array == NULL) {//if the array is null
        return FAIL;
    }
    if (index >= array->size) {//if the index is bigger than the size of the elements array
        void **newArray = (void **) calloc(index + 1, sizeof(void *));//allocate space for a new elements array
        if (newArray == NULL) {
            printf("Error: malloc has failed\n");
            return FAIL;
        }
        for (int i = 0; i < array->size; i++) {//for each index in the elements array
            newArray[i] = array->Earray[i];//copy the old elements array to the new elements array
        }
        free(array->Earray);//free the old elements array
        array->Earray = newArray;//set the array to the new elements array
        array->size = index + 1;//set the size to the index + 1
    }
    if (array->Earray[index] != NULL) {//if the index is not null
        array->freeElement(array->Earray[index]);//delete the element at the index
    }
    array->Earray[index] = array->copyElement(element);//copy the element to the index
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray array, int index) {
    if (index >= array->size || array->Earray[index] == NULL) {//if the index is bigger than the size of the elements array
        return NULL;
    }
    return array->copyElement(array->Earray[index]);//copy_person or copy_book
}


int GetAdptArraySize(PAdptArray array) {
    return array->size;//return the size of the array
}

void PrintDB(PAdptArray array) {
    if (array == NULL) {//if the array is null
        return;
    }
    for (int i = 0; i < array->size; i++) {//for each index in the array
        if (array->Earray[i] != NULL) {//if the index is not null
            array->printElement(array->Earray[i]);//print_person or print_book
        }
    }
}

void DeleteAdptArray(PAdptArray array) {
    if (array->Earray != NULL) {//
        for (int i = 0; i < array->size; i++) {//for each index in the array
            if (array->Earray[i] != NULL) {//if the index is not null
                array->freeElement((array->Earray)[i]);//delete_person or delete_book
            }
        }
        free(array->Earray);//free the elements array
    }
    free(array);//free the AdptArray struct
}
