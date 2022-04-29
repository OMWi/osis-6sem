#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int* arr;
int* sortedArr;
int size = 0;
int threadNum = 0;
int threadSize = 0;

void print(int* arr, int size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int getMin(size_t** partIndices) {
    int minIndex = (*partIndices)[0];
    for (size_t i = 0; i < threadNum; i++) {
        int curIndex = (*partIndices)[i];
        if (curIndex >= (i+1)*threadSize) {
            if (curIndex == minIndex) 
                minIndex = (*partIndices)[i+1];
            continue;
        }
        if (arr[curIndex] < arr[minIndex]) {
            minIndex = curIndex;
        }
    }
    int partIndex = minIndex / threadSize;
    (*partIndices)[partIndex]++;
    return arr[minIndex];
}

void finalMerge() {
    int isEmpty = 0;
    size_t* partIndices = malloc(sizeof(size_t) * threadNum);
    for (size_t i = 0; i < threadNum; i++) {
        partIndices[i] = i*threadSize;
    }
    for (size_t i = 0; i < threadSize * threadNum; i++) {
        sortedArr[i] = getMin(&partIndices);
    }
}

void merge(int left, int middle, int right) {
    int leftSize = middle - left + 1;
    int rightSize = right - middle;
    int* leftArr = malloc(sizeof(int) * leftSize);
    int* rightArr = malloc(sizeof(int) * rightSize);
    for (int i = 0; i < leftSize; i++) {
        leftArr[i] = arr[left+i];
    }
    for (int i = 0; i < rightSize; i++) {
        rightArr[i] = arr[middle+i+1];
    }

    int index = left, leftIndex = 0, rightIndex = 0;
    while (leftIndex < leftSize && rightIndex < rightSize) {
        arr[index++] = leftArr[leftIndex] < rightArr[rightIndex] ? leftArr[leftIndex++] : rightArr[rightIndex++];
    }
    
    while (leftIndex < leftSize) {
        arr[index++] = leftArr[leftIndex++];
    }
    while (rightIndex < rightSize) {
        arr[index++] = rightArr[rightIndex++];
    }
    // print(arr+left, leftSize + rightSize);
    // printf("\n");
    free(leftArr);
    free(rightArr);    
}

struct params
{
    int left;
    int right;
};


void* mergeSort(void* args) {
    int left = ((struct params*)args)->left;
    int right = ((struct params*)args)->right;

    if (left < right) {
        int middle = left + (right - left)/2;
        struct params *args1 = (struct params *)malloc(sizeof(struct params));
        args1->left = left; args1->right = middle;
        mergeSort((void *)args1);
        // free(args1);
        struct params *args2 = (struct params *)malloc(sizeof(struct params));
        args2->left = middle + 1; args2->right = right;
        mergeSort((void *)args2);
        // free(args2);
        merge(left, middle, right);
    }
}


int main()
{
    printf("Input array size: ");
    scanf("%d", &size);
    arr = malloc(sizeof(int) * size);
    sortedArr = malloc(sizeof(int) * size);
    for (size_t i = 0; i < size; i++) {
		arr[i] = rand() % 100;
    }
    printf("Array: ");
    print(arr, size);
    printf("Input number of threads: ");
    scanf("%d", &threadNum);	

	clock_t start = clock();

	pthread_t threads[threadNum];
    struct params **args = (struct params **)malloc(sizeof(struct params*) * threadNum);
    threadSize = size / threadNum;
	for (size_t i = 0; i < threadNum; i++) {
        args[i] = (struct params *)malloc(sizeof(struct params));
        args[i]->left = threadSize * i;
        args[i]->right = threadSize * (i + 1) - 1;
		pthread_create(&threads[i], NULL, mergeSort, (void *)args[i]);
    }
	for (size_t i = 0; i < threadNum; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Before final: ");
    print(arr, size);
    finalMerge();
    for (size_t index = threadNum * threadSize; index < size; index++) {
        for (size_t sIndex = index - 1; sIndex > 0; sIndex--) {
            if (sortedArr[sIndex] > arr[index]) {
                sortedArr[sIndex+1] = sortedArr[sIndex];
            }
            else {
                sortedArr[sIndex + 1] = arr[index];
                break;
            }
        }
    }

	clock_t end = clock();
    printf("Execution time: %f s\n", (end - start) / (double)CLOCKS_PER_SEC);

    printf("Sorted array: ");
    print(sortedArr, size);

	return 0;
}
