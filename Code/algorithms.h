#pragma once
#include <iostream>
#include <stack>
#include <thread>
#include <vector>
#include <chrono> //remember kids never ever write your own time libraries
#include <random>
#include <climits>
#include <algorithm>
#include <fstream>

using namespace std;
const int NUM_CORES = 6;//thread::hardware_concurrency()/2; //thread::hardware_concurrency() measures the total number of threads the system supports divide by 2 for the number of logical cores,which is the faster implemenation. Becuase hyperthreading doesn't actually make anything faster it just allows every core to handle two threads at around half speed.


thread* threads = new thread[NUM_CORES]; //NUM_CORES is how many cores your system supports

void genThreads(void (*func)(int*,unsigned long),int* arr,unsigned long arrLen) {
	for(int i=0;i<NUM_CORES;i++)
		threads[i] = thread(func,arr+(i*(arrLen/NUM_CORES)),arrLen/NUM_CORES+(arrLen%NUM_CORES));
	return;
}

void closeThreads() {
	for(int i=0;i<NUM_CORES;i++) {
		if(threads[i].joinable())
			threads[i].join();
	}
}

void bubSort(int* arr,unsigned long arrLen) {
	int temp;
	for(int i=0;i<arrLen-1;i++) {
		for(int j=0;j<(arrLen-i-1);j++) {
			if(arr[j+1]<arr[j]) {
				temp=arr[j+1];
				arr[j+1]=arr[j];
				arr[j]=temp;
			}
		}
	}

}
void cppSort(int* arr,unsigned long arrLen) {
	sort(arr,arr+arrLen); //a test of the default cpp sorting library
}

void inSort(int* arr,unsigned long arrLen) {
	int k,j;
	for(int i=1;i<arrLen;i++) {
	k=arr[i];
	j=i-1;
		for(int h=0;j>=0&&arr[j]>k;j--)
			arr[j+1]=arr[j];
		arr[j+1]=k;
	}

}

void mergeSort(int* arr,unsigned long arrLen) {
	if(arrLen==1)
		return;
	mergeSort(arr,arrLen/2);
	mergeSort(arr+(arrLen/2),(arrLen/2)+(arrLen%2));

	inplace_merge(arr,arr+arrLen/2,arr+arrLen); //i realized over the course of writing my own merge function that cpp's is definetly better and i needed to use merge for another purpose so why not use the better one
}

void printArr(int* arr,unsigned long arrLen) {
	for(int i=0;i<arrLen;i++)
		cout << arr[i] << " ";
	cout << endl;
}

bool checkArr(int* arr,unsigned long arrLen,int* arr2, unsigned long arrLen2) {
	if(arrLen!=arrLen2)
		return false;
	for(int i=0;i<arrLen;i++) {
		if(arr[i]!=arr2[i])
			return false;
	}
	return true;
}

void mergeWrapper(int* start1,int* end1,int* end2) { //since inplace_merge is a cpp library you can't use a function pointer with it so I just made a wrapper
	inplace_merge(start1,end1,end2);
}

auto multiThread(void (*func)(int*,unsigned long),int* arr,unsigned long arrLen,bool threaded) { //none of the sorting programs are in place just because that makes test benching more space efficient
	auto start = std::chrono::high_resolution_clock::now();
	if(threaded) {
		genThreads(func,arr,arrLen); //use a divide and conquer scheme with threads
		closeThreads();
		int t = NUM_CORES/2;
		for(int j=t;j>0;j--) {
			int* start1;
			int* end2;
			int* end1;
			for(int i=0;i<j;i++) {
				start1 = arr+((i)*(2*(t-j+1))*(arrLen/NUM_CORES));
				end2 = start1+((2*(t-j+1))*(arrLen/NUM_CORES))+((i+1>=j)*(arrLen%NUM_CORES)); //mod to account for uneven arrays
				end1 = end2-(((t-j+1-(j==1)))*(arrLen/NUM_CORES))-((i+1>=j)*(arrLen%NUM_CORES));
				if((arr+arrLen)<end1)
				threads[i] = thread(mergeWrapper,start1,end1,end2);

			}
			closeThreads();
		}
	}
	else {
	    func(arr,arrLen);
    }
	auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double>(end - start).count();
}

void heapify(int arr[], int n, int i);

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void buildMaxHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

void heapSort(int arr[], unsigned long n) {
    buildMaxHeap(arr, n);

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

void buildMaxHeapSegment(int arr[], int n, int start, int end) {
    for (int i = end; i >= start; i--) {
        heapify(arr, n, i);
    }
}
