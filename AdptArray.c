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

///
/// \param copyElement  copy function book or person
/// \param freeElement  delete function book or person
/// \param printElement  print function book or person
/// \return new AdptArray or NULL if malloc failed
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

/// \param array AdptArray
/// \param index index to set the element at
/// \param element the element to set
/// \return  SUCCESS if the element was set, FAIL otherwise
Result SetAdptArrayAt(PAdptArray array, int index, PElement element) {
    if (array == NULL) {//if the array is null
        return FAIL;
    }
    if (index < 0) {//if the index is negative
        printf("Error: index is negative\n");
        return FAIL;
    }
    if (index >= array->size) {//if the index is bigger than the size of the elements array
        void **newArray = (void **) calloc(index + 1, sizeof(void *));//allocate space for a new elements array
        if (newArray == NULL) {
            printf("Error: calloc has failed\n");
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

///
/// \param array AdptArray
/// \param index index of the element
/// \return the element at the index if the index is valid, NULL otherwise
PElement GetAdptArrayAt(PAdptArray array, int index) {
    if (index >= array->size || array->Earray[index] == NULL || index <
                                                                0) {//if the index is bigger than the size of the elements array or the index is null or the index is negative
        return NULL;
    }
    return array->copyElement(array->Earray[index]);//copy_person or copy_book
}

/// \brief Get the Adpt Array Size object
/// \param array
/// \return  the size of the array
int GetAdptArraySize(PAdptArray array) {
    return array->size;//return the size of the array
}

/// \brief Print the Adpt Array elements
/// \param array
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

/// \brief Delete the Adpt Array
/// \param array
void DeleteAdptArray(PAdptArray array) {
    if (array->Earray != NULL) {// if the elements array is not null
        for (int i = 0; i < array->size; i++) {//for each index in the array
            if (array->Earray[i] != NULL) {//if the index is not null
                array->freeElement((array->Earray)[i]);//delete_person or delete_book
            }
        }
        free(array->Earray);//free the elements array
    }
    free(array);//free the AdptArray struct
}
