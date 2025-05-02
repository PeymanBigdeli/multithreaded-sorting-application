#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


// function prototypes
 
void* multithreadedSort(void* arg);
void* multithreadedDivide(void* arg);
void* multithreadedMerge(void* arg);

// input array and the sorted array declaration

int array[] = {1,4,2,5,-7,62,19,41,12,7,0,-4,13,0,123,-51,51};
int sortedArray[sizeof(array) / sizeof(array[0])] = {0};


// structures to work with thread functions arguments

typedef struct 
{
    int start;
    int end;
} rangeIndex;

typedef struct {
    int mid;
    int length1;
    int length2;
} mergeData;


int main() {

    // main thread to sort the array craeted

    pthread_t mainThread;
    pthread_create(&mainThread , NULL , multithreadedDivide , NULL);
    pthread_join(mainThread , NULL);            // wating for the main thread to finish

}
 
void* multithreadedDivide(void* arg) {

    // making the parameters passed to thread1 and thread2 functions

    rangeIndex *arr1 = (rangeIndex *) malloc(sizeof(rangeIndex));
    arr1->start  = 0;
    arr1->end = ((sizeof(array) / sizeof(array[0])) - 1) / 2;
    rangeIndex *arr2 = (rangeIndex *) malloc(sizeof(rangeIndex));
    arr2->start  = (((sizeof(array) / sizeof(array[0])) - 1) / 2) + 1;
    arr2->end = (sizeof(array) / sizeof(array[0])) - 1;

    pthread_t thread1 , thread2 , mergeThread;
    
    // copying the original array into the sorted one to avoid changes to the original array
    for(int i=0 ; i < sizeof(array) / sizeof(array[0]); ++i) sortedArray[i] = array[i];         

    // thread1 and thread2 craeted to sort the two halves of the array 

    pthread_create(&thread1 , NULL , multithreadedSort , (void*)arr1);
    pthread_create(&thread2 , NULL , multithreadedSort , (void*)arr2);

    // waiting for the two threads to finish 

    pthread_join(thread1 , NULL);
    pthread_join(thread2 , NULL);

    // making the parameter passed to the merge thread function 

    mergeData * mergeArrData = (mergeData *) malloc(sizeof(mergeData));
    mergeArrData->mid = arr2->start;
    mergeArrData->length1 = arr1->end - arr1->start + 1;
    mergeArrData->length2 = arr2->end - arr2->start + 1;

    // creating the merge thread to merge the to sorted halves

    pthread_create(&mergeThread , NULL , multithreadedMerge , (void*)mergeArrData);

    // wainting for the merge thread to finish 

    pthread_join(mergeThread, NULL);

    // printing the sorted array as the output in the main and parent thread
    printf("\033[31moriginal array : [");
    for(int i=0 ; i < sizeof(array) / sizeof(array[0]); ++i) {
        printf("%d ",array[i]);
        if(i != (sizeof(array) / sizeof(array[0])) - 1) printf(", ");
    }
    printf("]\n\033[39m");
    
    printf("\033[32msorted array : [");
    for(int i=0 ; i < sizeof(array) / sizeof(array[0]); ++i) {
        printf("%d ",sortedArray[i]);
        if(i != (sizeof(array) / sizeof(array[0])) - 1) printf(", ");
    }
    printf("]\n\033[39m");
}


// function to sort the two halves of the array using bubblesort algorithm 

void* multithreadedSort(void* arg) {
    rangeIndex * rangeIndexData = (rangeIndex *)arg;
    int length = rangeIndexData->end - rangeIndexData->start + 1;
    for(int i=rangeIndexData->start;i < length + rangeIndexData->start ;++i) {
        for(int j=rangeIndexData->start ; j < length + rangeIndexData->start - 1 ;++j){
            if(sortedArray[j] > sortedArray[j+1]){
                int temp = sortedArray[j];
                sortedArray[j] = sortedArray[j+1];
                sortedArray[j+1] = temp;
            }
        }
    }
}


// function to merge the two sorted halves of the array 

void* multithreadedMerge(void* arg) {
    mergeData * mergeArrData = (mergeData *)arg;
    int i = 0;
    int j = mergeArrData->mid;
    int k = 0;
    int tempArr[mergeArrData->length1 + mergeArrData->length2];

    while(1) {
        if(mergeArrData->length1 == 0 || mergeArrData->length2 == 0) break;
        if(i >= mergeArrData->length1 || j >= mergeArrData->length2 + mergeArrData->mid) break;

        if(sortedArray[i] > sortedArray[j]) {
            tempArr[k] = sortedArray[j];
            j++;
        } 
        else {
            tempArr[k] = sortedArray[i];
            i++;
        }
        k++;
    }
    if(i < mergeArrData->length1 ) {
        for(int m=i; i < mergeArrData->length1;++m) {
            tempArr[k] =sortedArray[i];
            i++;
            k++;
        }
    }
    if(j < mergeArrData->length2 + mergeArrData->mid) {
        for(int m=j; j < mergeArrData->length2 + mergeArrData->mid;++m) {
            tempArr[k] =sortedArray[j];
            j++;
            k++;
        }
    }
    
    for(int m=0; m < mergeArrData->length1 + mergeArrData->length2 ;++m) {
        sortedArray[m] = tempArr[m];
    }
}